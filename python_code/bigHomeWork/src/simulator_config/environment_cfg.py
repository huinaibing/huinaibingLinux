# gym环境配置的config文件
import numpy as np

dt = 0.001  # s, 时间步长

environment_cfg = {
    "xMax": 5.0,  # x轴最大值, m
    "xMin": -5.0,  # x轴最小值
    "yMax": 5.0,  # y轴最大值
    "yMin": -5.0,  # y轴最小值
    "zMax": 20.0,  # z轴最大值
    "zMin": 0.0,  # z轴最小值

    # 避免万向节锁，限制各个角度
    # 下面的角度范围配置最好别改，要不然我更新角度的代码完蛋了
    "psiMax": np.pi,  # psi最大值
    "psiMin": -np.pi,  # psi最小值
    "thetaMax": np.pi * 0.48,  # theta最大值 防止他迭代到90度导致奇点
    "thetaMin": -np.pi * 0.48,  # theta最小值
    "phiMax": np.pi,  # phi最大值
    "phiMin": -np.pi,  # phi最小值

    # 动作空间的限制
    "omegaMax": 1500.0,  # 旋翼最大角速度, rad/s
    "omegaMin": 0,  # 旋翼最小角速度
}

environment_init_cfg = {
    "xInit": 0.0,  # 初始x位置
    "yInit": 0.0,  # 初始y位置
    "zInit": 10.0,  # 初始z位置
    "psiInit": 0.0,  # 初始psi角度
    "thetaInit": 0.0,  # 初始theta角度
    "phiInit": 0.0,  # 初始phi角度

    "omegaInit": np.array([750.0, 750.0, 750.0, 750.0]),  # 初始旋翼角速度

    "vxInit": 0.0,  # 初始x速度, m/s
    "vyInit": 0.0,  # 初始y速度
    "vzInit": 0.0,  # 初始z速度

    "omegaXInit": 0.0,  # 初始x轴角速度, 参考系是body
    "omegaYInit": 0.0,  # 初始y轴角速度
    "omegaZInit": 0.0,  # 初始z轴角速度
}

environment_reward_cfg = {
    "rewardPoint": [(1, 2, 10), (3, 3, 7), (3, 0, 14)],  # 奖励点，坐标
    "reward": 300,  # 达到指定目标点时的奖励的多少
    "rewardRadius": 1,  # 距离奖励点在这个距离内视为获得奖励
    "rewardPerTime": -0.01,  # 每经过一个时间步的奖励，这个数值会乘上距离，以引导无人机前往目标点
    "rewardTruncate": -400,  # 截断（越界导致的）的奖励
    "rewardTerminate": 400,  # 正常截止（收集所有point）的奖励
    "rewardTheta": -0.01  # 稳定性奖励，越不稳定扣除越多
}
