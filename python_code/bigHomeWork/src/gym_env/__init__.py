# 不是所有的__init__.py都是空的

import gymnasium
from gym_env.quadrotor_env import QuadrotorEnv

gymnasium.register(
    id="myQuadrotor",
    entry_point="gym_env.quadrotor_env:QuadrotorEnv",
    max_episode_steps=1000000  # 最多1000秒的时间
)
