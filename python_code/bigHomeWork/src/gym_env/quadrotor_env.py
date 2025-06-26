from typing import Optional

import gymnasium as gym
import numpy as np
import simulator_config.environment_cfg as env_cfg
from quadrotor_simulation.quadrotor import Quadrotor


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
        self.quadrotor = Quadrotor()

        # 读取奖励数据
        self.reward_point = env_cfg.environment_reward_cfg["rewardPoint"]
        self.reward = env_cfg.environment_reward_cfg["reward"]
        self.reward_radius = env_cfg.environment_reward_cfg["rewardRadius"]
        self.reward_per_time = env_cfg.environment_reward_cfg["rewardPerTime"]
        self.reward_truncate = env_cfg.environment_reward_cfg["rewardTruncate"]
        self.reward_terminate = env_cfg.environment_reward_cfg["rewardTerminate"]

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

    def reset(self, seed: Optional[int] = None, options: Optional[dict] = None):
        """
        环境重置函数
        详情见gym参考文档
        参数列出来是因为python的规范要不然pycharm报警
        """
        super().reset()
        self.quadrotor.reset_quadrotor()
        obs = self.quadrotor.get_state()
        # 由于我们需要排除reward point,所以需要重置它
        self.reward_point = env_cfg.environment_reward_cfg["rewardPoint"]

        return obs, {}

    def step(self, action: np.ndarray):
        """
        step函数，设置无人机的四个旋翼
        """
        # 奖励
        # 每经过一个时间步，奖励减一
        rwd = self.reward_per_time
        assert action.shape == (4,)
        self.quadrotor.set_omega(action)
        # 我设置当无人机正常运行时返回true
        # 又写反了。。。
        # 设置截断（越界）的奖励
        truncate = not self.quadrotor.move()
        if truncate:
            rwd += self.reward_truncate

        for idx, point in enumerate(self.reward_point):
            raise NotImplementedError




        raise NotImplementedError
        return self.quadrotor.get_state()

    def get_reward(self):
        """
        如果无人机离奖励点距离小于指定值的时候，视为获得奖励
        """
