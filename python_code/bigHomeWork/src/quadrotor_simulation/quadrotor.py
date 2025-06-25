import simulator_config.quadrotor_cfg as quad_cfg
import simulator_config.environment_cfg as env_cfg
import numpy as np


class Quadrotor:
    """
    专门用来处理无人机仿真的类
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

        # 旋翼角速度
        # shape = (4,)
        # shape和上面不一样！！！！！！！！！！！！！！
        self.omega = env_cfg.environment_init_cfg["omegaInit"]

    def get_lift_force_in_body(self):
        """
        获取无人机的每一个旋翼的力
        在body坐标系！！！！
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
        更新方程见README
        全代码最重要的一段
        """
