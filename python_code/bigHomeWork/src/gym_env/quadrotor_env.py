import copy
from typing import Optional

import gymnasium as gym
import numpy as np
import simulator_config.environment_cfg as env_cfg
from quadrotor_simulation.quadrotor import Quadrotor
import xqy_utils.xqy_utils as utils


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
        self.reward_theta = env_cfg.environment_reward_cfg["rewardTheta"]

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
            dtype=np.float64
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
            dtype=np.float64
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
        rwd = 0
        assert action.shape == (4,)
        self.quadrotor.set_omega(action)
        # 我设置当无人机正常运行时返回true
        # 又写反了。。。
        # 设置截断（越界）的奖励
        truncate = not self.quadrotor.move()
        if truncate:
            rwd += self.reward_truncate

        tmp = []
        distances = []
        for idx, point in enumerate(self.reward_point):
            # 如果point和当前位置的距离小于指定半径的话，
            # 获得奖励，并且删除该点
            dis = utils.XQYUtils.calculate_distance(
                self.quadrotor.location[0, 0],
                self.quadrotor.location[1, 0],
                self.quadrotor.location[2, 0],
                point[0],
                point[1],
                point[2]
            )
            distances.append(dis)
            if dis < self.reward_radius:
                rwd += self.reward
            else:
                tmp.append(point)

        self.reward_point = copy.deepcopy(tmp)

        # 每经过一个时间步，扣除奖励
        # 离最近目标点越近，扣除的越少
        rwd += self.reward_per_time * min(distances) ** 2

        # 扣除稳定性奖励
        rwd += self.reward_theta * self.quadrotor.angles[1, 0] ** 2

        if len(self.reward_point) == 0:
            terminate = True
        else:
            terminate = False

        return self.quadrotor.get_state(), rwd, terminate, truncate, {}
