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
        - 但是位置感觉用不了，我会区别更新，注意这里！！！
    
        
    n. 和gym交互的api
    """
    def __init__(self):
        # 无人机的参数
        self.mass = quad_cfg.quadrotor_cfg["mass"]
        self.gravity = quad_cfg.quadrotor_cfg["gravity"]
        self.d = quad_cfg.quadrotor_cfg["d"] # 四旋翼臂长
        self.cT = quad_cfg.quadrotor_cfg["cT"] # 电机升力常数
        self.cM = quad_cfg.quadrotor_cfg["cM"] # 电机反扭矩常数
        self.cD = quad_cfg.quadrotor_cfg["cD"] # 空气阻力系数

        # 无人机姿态角的限制
        self.psi_max = env_cfg.environment_cfg["psiMax"]
        self.psi_min = env_cfg.environment_cfg["psiMin"]
        self.theta_max = env_cfg.environment_cfg["thetaMax"]
        self.theta_min = env_cfg.environment_cfg["thetaMin"]
        self.phi_max = env_cfg.environment_cfg["phiMax"]
        self.phi_min = env_cfg.environment_cfg["phiMin"]

        # 时间=0的参数
        self.location = np.matrix(
            [
                [env_cfg.environment_init_cfg["xInit"]],
                [env_cfg.environment_init_cfg["yInit"]], 
                [env_cfg.environment_init_cfg["zInit"]]
            ],
            dtype=np.float32
        ) # shape = (3, 1)
        # 位置不使用那个z向下坐标系！！！！

        self.angles = np.matrix(
            [
                [env_cfg.environment_init_cfg["psiInit"]],
                [env_cfg.environment_init_cfg["thetaInit"]],
                [env_cfg.environment_init_cfg["phiInit"]]
            ],
            dtype=np.float32
        ) # shape = (3, 1)

        self.velocity = np.matrix(
            [
                [env_cfg.environment_init_cfg["vxInit"]],
                [env_cfg.environment_init_cfg["vyInit"]],
                [env_cfg.environment_init_cfg["vzInit"]]
            ]
        ) # shape = (3, 1)
        # 速度使用z向下坐标系！

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
    
    #
    # end：计算无人机的各种受力的部分
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
            (self.location.flatten(), self.angles.flatten()),
            axis=0
        )

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
        """

    #
    # end：和gym交互的部分
    #