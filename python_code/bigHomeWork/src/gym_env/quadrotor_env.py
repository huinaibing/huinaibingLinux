import gymnasium as gym
import numpy as np
import simulator_config.environment_cfg as env_cfg
import quadrotor_simulation.quadrotor as quadrotor


class QuadrotorEnv(gym.Env):
    """
    四旋翼无人机的环境类，继承gym.Env。
    gymnasium官方文档https://gymnasium.farama.org/introduction/create_custom_env/
    根据作业要求，action space和observation space如下
        - observation space：
            - 位置（x, y, z） 连续
            - 姿态角（psi，theta，phi）连续
        - action space:
            - 四个旋翼的旋转角速度（omega1, omega2, omega3, omega4）连续
    """

    def __init__(self):
        super().__init__()
        # 初始化四旋翼无人机
        self.quadrotor = quadrotor.Quadrotor()

        # 设定obs space and action space
        # 也是从config读入
        self.observation_space = gym.spaces.Box(
            low=np.array([
                env_cfg.environment_cfg["xMin"],
                env_cfg.environment_cfg["yMin"],
                env_cfg.environment_cfg["zMin"],
                env_cfg.environment_cfg["psiMin"],
                env_cfg.environment_cfg["thetaMin"],
                env_cfg.environment_cfg["phiMin"],
            ]),
            high=np.array([
                env_cfg.environment_cfg["xMax"],
                env_cfg.environment_cfg["yMax"],
                env_cfg.environment_cfg["zMax"],
                env_cfg.environment_cfg["psiMax"],
                env_cfg.environment_cfg["thetaMax"],
                env_cfg.environment_cfg["phiMax"],
            ]),
            dtype=np.float32
        )
        self.action_space = gym.spaces.Box(
            low=np.array([
                env_cfg.environment_cfg["omegaMin"],
                env_cfg.environment_cfg["omegaMin"],
                env_cfg.environment_cfg["omegaMin"],
                env_cfg.environment_cfg["omegaMin"],
            ]),
            high=np.array([
                env_cfg.environment_cfg["omegaMax"],
                env_cfg.environment_cfg["omegaMax"],
                env_cfg.environment_cfg["omegaMax"],
                env_cfg.environment_cfg["omegaMax"],
            ]),
            dtype=np.float32
        )
        