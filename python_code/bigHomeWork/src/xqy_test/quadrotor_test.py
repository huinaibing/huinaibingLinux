################################################
#
# 测试无人机类的bug
# 显然我软件工程课没认真听讲，再者也没有足够时间做测试，这里随便测试一下就好
#
################################################
import numpy as np
from quadrotor_simulation.quadrotor import Quadrotor
import matplotlib.pyplot as plt


def quadrotor_shape_test():
    quadrotor = Quadrotor()
    assert quadrotor.quad_omega.shape == (3, 1)
    assert quadrotor.get_state().shape == (6,)
    assert quadrotor.location.shape == (3, 1)
    assert quadrotor.get_lift_force_in_body().shape == (4, )
    assert quadrotor.get_f_lab().shape == (3, 1)


def quadrotor_simple_test():
    quadrotor = Quadrotor()
    print(quadrotor.x_max, quadrotor.y_max, quadrotor.z_max)
    print(quadrotor.x_min, quadrotor.y_min, quadrotor.z_min)
    print(quadrotor.phi_max, quadrotor.theta_max, quadrotor.phi_min, quadrotor.theta_min)
    quadrotor.self_check()
    print("---------move-------")
    print(quadrotor.move())
    quadrotor.self_check()


def quadrotor_draw_3d_plot():
    quadrotor = Quadrotor()
    xs = []
    ys = []
    zs = []
    while quadrotor.move() is True:
        x, y, z, _, _, _ = quadrotor.get_state()
        xs.append(x)
        ys.append(y)
        zs.append(z)

    # 预计无人机会是一条直线的飞行轨迹
    # 向z轴正向的直线，因为无人机坐标系z轴向下
    # 使用正弦/余弦函数生成螺旋点

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(xs, ys, zs, c=zs, cmap='viridis', s=20)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()


def quadrotor_theta_test():
    quadrotor = Quadrotor()
    ts = []
    t = 0
    xs = []
    ys = []
    zs = []
    thetas = []
    quadrotor.set_omega(np.array([0, 500, 0, 0]))

    while quadrotor.move() is True:
        t += 1
        ts.append(t)
        x, y, z, _, theta, _ = quadrotor.get_state()
        xs.append(x)
        ys.append(y)
        zs.append(z)
        thetas.append(theta)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(xs, ys, zs, c=zs, cmap='viridis', s=20)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()
    plt.scatter(ts, thetas)
    plt.show()


if __name__ == '__main__':
    quadrotor_shape_test()
    quadrotor_draw_3d_plot()
    quadrotor_theta_test()
