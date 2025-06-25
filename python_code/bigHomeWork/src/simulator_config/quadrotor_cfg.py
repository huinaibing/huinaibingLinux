# 这是无人机的各种参数的配置文件
# 参数为接近真实，使用参考链接https://blog.csdn.net/weixin_41869763/article/details/113853365

quadrotor_cfg = {
    "mass": 1.5,  # kg
    "gravity": 9.8,  # m/s^2
    "d": 0.16, # m,  # 四旋翼臂长
    "cT": 1.201e-5, #电机升力常数
    "cM": 1.574e-7, #电机反扭矩常数
    "cD": 0.2, # 空气阻力系数
}
