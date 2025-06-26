import simulator_config.quadrotor_cfg as quad_cfg
import simulator_config.environment_cfg as env_cfg
import numpy as np
from xqy_utils.xqy_utils import XQYUtils as utils


class Quadrotor:
    """
    专门用来处理无人机仿真的类
    包含以下要素
    1. 初始化各种参数（init）
    2. 计算无人机的受力
        - 这一部分是在参考系下做的，需要格外注意z轴取向向下
        - 受力、速度、角速度等等通通用这个坐标系，
        - 位置也是！！所以在画图的时候比较麻烦！！！
    3. 迭代计算（显示欧拉法）
    4. 和gym交互的api

    需要在迭代训练中更新的：
    1. 刚体姿态角
    2. 刚体位置
    3. 刚体平动速度
    4. 刚体转动速度（body参考系）
    """

    def self_check(self):
        """
        自我检查类
        把自己的所有状态打印一遍，
        方便我debug
        """
        print("--------state-------")
        print("state", self.get_state())
        print("-------force----------")
        print("空气阻力", self.get_drag_lab())
        print("升力", self.get_f_lab())
        print("重力", self.get_gravity_lab())
        print("-------velocity and omega-------")
        print("平动", self.velocity)
        print("转动", self.quad_omega)
        print("--------机翼---------")
        print("旋翼转速", self.omega)
        print("---------------------")

    def __init__(self):
        # 时间步长
        self.dt = env_cfg.dt

        # 无人机的参数
        self.mass = quad_cfg.quadrotor_cfg["mass"]
        self.gravity = quad_cfg.quadrotor_cfg["gravity"]
        self.d = quad_cfg.quadrotor_cfg["d"]  # 四旋翼臂长
        self.cT = quad_cfg.quadrotor_cfg["cT"]  # 电机升力常数
        self.cM = quad_cfg.quadrotor_cfg["cM"]  # 电机反扭矩常数
        self.cD = quad_cfg.quadrotor_cfg["cD"]  # 空气阻力系数
        self.Ixx = quad_cfg.quadrotor_cfg["Ixx"]  # kg*m^2, 转动惯量
        self.Iyy = quad_cfg.quadrotor_cfg["Iyy"]
        self.Izz = quad_cfg.quadrotor_cfg["Izz"]

        # 无人机姿态角的限制
        self.psi_max = env_cfg.environment_cfg["psiMax"]
        self.psi_min = env_cfg.environment_cfg["psiMin"]
        self.theta_max = env_cfg.environment_cfg["thetaMax"]
        self.theta_min = env_cfg.environment_cfg["thetaMin"]
        self.phi_max = env_cfg.environment_cfg["phiMax"]
        self.phi_min = env_cfg.environment_cfg["phiMin"]

        # 无人机位置限制
        self.x_max = env_cfg.environment_cfg["xMax"]
        self.y_max = env_cfg.environment_cfg["yMax"]
        self.z_max = env_cfg.environment_cfg["zMax"]
        self.x_min = env_cfg.environment_cfg["xMin"]
        self.y_min = env_cfg.environment_cfg["yMin"]
        self.z_min = env_cfg.environment_cfg["zMin"]

        # 时间=0的参数
        self.location = np.matrix(
            [
                [env_cfg.environment_init_cfg["xInit"]],
                [env_cfg.environment_init_cfg["yInit"]],
                [env_cfg.environment_init_cfg["zInit"]]
            ],
            dtype=np.float32
        )  # shape = (3, 1)
        # 位置也使用那个z向下坐标系！！！！

        self.angles = np.matrix(
            [
                [env_cfg.environment_init_cfg["psiInit"]],
                [env_cfg.environment_init_cfg["thetaInit"]],
                [env_cfg.environment_init_cfg["phiInit"]]
            ],
            dtype=np.float32
        )  # shape = (3, 1)

        self.velocity = np.matrix(
            [
                [env_cfg.environment_init_cfg["vxInit"]],
                [env_cfg.environment_init_cfg["vyInit"]],
                [env_cfg.environment_init_cfg["vzInit"]]
            ]
        )  # shape = (3, 1)
        # 速度使用z向下坐标系！

        self.quad_omega = np.matrix(
            [
                [env_cfg.environment_init_cfg["omegaXInit"]],
                [env_cfg.environment_init_cfg["omegaYInit"]],
                [env_cfg.environment_init_cfg["omegaZInit"]]
            ]
        )  # shape = (3, 1)
        # 使用body参考系,这个是机体的角速度，和下面的旋翼角速度是两个玩意！！

        # 旋翼角速度
        # shape = (4,)
        # shape和上面不一样！！！！！！！！！！！！！！
        # 角速度如果为正数，代表升力向上（z轴负方向！！！）
        self.omega = env_cfg.environment_init_cfg["omegaInit"]

    ########################################
    # 计算无人机的各种受力的部分
    ########################################

    def get_lift_force_in_body(self):
        """
        获取无人机的每一个旋翼的力
        在body坐标系！！！！
        注意这里的力为正的话表示向上，但是我们坐标系的z轴是向下的
        在get_f_lab()中会处理这个符号问题
        :return np.array: shape=(4,)
        """
        # 从角速度到升力
        # Fi = cT * omega^2
        # 如果omega是负数，则F需要带一个负号
        f_body = np.zeros_like(self.omega)
        for i in range(4):
            f_body[i] = self.cT * self.omega[i] ** 2
            if self.omega[i] < 0:
                f_body[i] *= -1

        return f_body

    def get_f_lab(self):
        """
        把升力转化到实验室坐标系
        注意z轴是向下的！！！！！！！！！
        注意z轴是向下的！！！！！！！！！
        注意z轴是向下的！！！！！！！！！
        :return np.array: shape=(3, 1) 
        """
        f_body = self.get_lift_force_in_body()
        f_matrix_body = np.zeros((3, 1), dtype=np.float32)
        # 注意下面这里取了负号
        f_matrix_body[2, 0] = -np.sum(f_body)

        # 旋转矩阵
        # 这里要转置矩阵，看README
        R_v_v1_T = utils.get_R_v_v1(self.angles[0, 0]).T
        R_v1_v2_T = utils.get_R_v1_v2(self.angles[1, 0]).T
        R_v2_b_T = utils.get_v2_b(self.angles[2, 0]).T

        return R_v_v1_T * R_v1_v2_T * R_v2_b_T * f_matrix_body

    def get_gravity_lab(self):
        """
        算重力
        z轴是向下的！！！！！！！！！
        z轴是向下的！！！！！！！！
        z轴是向下的！！！！！！！！！
        :return np.array: shape=(3, 1)
        """
        gravity_lab = np.zeros((3, 1), dtype=np.float32)
        gravity_lab[2, 0] = self.mass * self.gravity
        return gravity_lab

    def get_drag_lab(self):
        """
        空气阻力
        :return np.array: shape=(3, 1)
        """
        return -self.cD * self.velocity

    def get_Mx(self):
        """
        计算无人机的x的力矩
        用的body参考系
        :return float:
        """
        T1, T2, T3, T4 = self.get_lift_force_in_body()
        return self.d * (T2 - T4)

    def get_My(self):
        """
        计算y方向的My
        :return float:
        """
        T1, T2, T3, T4 = self.get_lift_force_in_body()
        return self.d * (T3 - T1)

    def get_Mz(self):
        """
        计算z方向的Mz
        :return float:
        """
        T1, T2, T3, T4 = self.get_lift_force_in_body()
        return self.cM * (T1 - T2 + T3 - T4)

    #
    # end：计算无人机的各种受力的部分
    #

    #########################################
    # 状态的迭代（不包含和gym交互）
    #
    # 迭代的方程见README
    #########################################

    def iterate_translation_motion(self):
        """
        迭代速度和位置
        """
        fsum_lab = self.get_drag_lab() + self.get_gravity_lab() + self.get_f_lab()
        vdot = fsum_lab / self.mass

        self.location += self.velocity * self.dt
        self.velocity += vdot * self.dt

    def iterate_rotation(self):
        omegadot = np.zeros((3, 1), dtype=np.float32)
        # omega x dot
        omegadot[0, 0] = (
                                 self.get_Mx() - (self.Izz - self.Iyy) * self.quad_omega[1, 0] * self.quad_omega[2, 0]
                         ) / self.Ixx
        # omega y dot
        omegadot[1, 0] = (
                                 self.get_My() - (self.Ixx - self.Izz) * self.quad_omega[2, 0] * self.quad_omega[0, 0]
                         ) / self.Iyy
        # omega z dot
        omegadot[2, 0] = (
                                 self.get_Mz() - (self.Iyy - self.Ixx) * self.quad_omega[0, 0] * self.quad_omega[1, 0]
                         ) / self.Izz

        # 备份无人机t时刻的omega
        omega_tmp = np.copy(self.quad_omega)
        # 更新无人机omega
        self.quad_omega += self.dt * omegadot
        # 更新姿态角
        # 注意这里，本class里面的迭代方程和README里面的迭代方程符号位置写反了。。。
        # 所以得换个位置
        anglesdot_reversed = utils.get_convert_omegaxyz_to_psithetaphi_dot(
            self.angles[2, 0], self.angles[1, 0]
        ) * omega_tmp

        # 上面的公式算出来的是phi theta psi的顺序，但是这个类里面是psi theta phi，
        # QAQ，写反了，只能将错就错，再反转过去
        tmp = anglesdot_reversed[0, 0]
        anglesdot_reversed[0, 0] = anglesdot_reversed[2, 0]
        anglesdot_reversed[2, 0] = tmp

        # 更新角度
        self.angles += self.dt * anglesdot_reversed

        # 对phi和psi做越界回正
        # 但是不对theta做，因为theta越界直接失败
        self.angles[0, 0] %= 2 * np.pi
        if self.angles[0, 0] > np.pi:
            self.angles[0, 0] -= 2 * np.pi

        self.angles[2, 0] %= 2 * np.pi
        if self.angles[2, 0] > np.pi:
            self.angles[2, 0] -= 2 * np.pi

    #
    # end: 状态的迭代（不包含和gym交互）
    #

    #########################################
    # 和gym交互的部分
    #########################################

    def get_state(self):
        """
        获取无人机的状态
        给gym状态空间用的
        :return np.array: (x, y, z, psi, theta, phi) shape=(6,)
        """
        return np.concatenate(
            [self.location.flatten(), self.angles.flatten()],
            axis=1
        ).A1

    def set_omega(self, omega: np.array):
        """
        设置旋翼角速度
        相当于gym的动作空间
        :param omega: 旋翼角速度，shape=(4,)
        """
        self.omega = omega

    def move(self):
        """
        更新无人机的状态
        根据现在的旋翼角速度和倾斜角来更新
        记得先set_omega再move!!!!!!!!!!!!
        记得先set_omega再move!!!!!!!!!!!!
        记得先set_omega再move!!!!!!!!!!!!
        这里需要做越界检查，如果越界返回false
        :return bool: 越界后返回false，一般返回true
        """
        self.iterate_translation_motion()
        self.iterate_rotation()
        if self.location[0, 0] > self.x_max or \
                self.location[0, 0] < self.x_min or \
                self.location[1, 0] > self.y_max or \
                self.location[1, 0] < self.y_min or \
                self.location[2, 0] > self.z_max or \
                self.location[2, 0] < self.z_min:
            return False

        if self.angles[1, 0] > self.theta_max or self.angles[1, 0] < self.theta_min:
            return False

        return True

    #
    # end：和gym交互的部分
    #
