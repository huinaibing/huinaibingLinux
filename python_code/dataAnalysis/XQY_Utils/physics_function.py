import math as m
import numpy as np


class PhysicsFunction:
    def __init__(self):
        pass

    @staticmethod
    def lorentz_gamma_factor(beta: float) -> float:
        """
        一个用于计算洛伦兹因子的函数, 输入为速度（自然单位）
        :param beta: 速度（0~1之间）
        :return: gamma 若不合规则返回math.inf
        """
        if not 0 < beta < 1:
            print("error beta value")
            return m.inf
        return 1 / m.sqrt(1 - beta ** 2)

    @staticmethod
    def calculate_tmax(beta):
        """
        计算入射粒子很重，速度不大时的Tmax（详情见粒子探测技术电离能损）自然单位制
        :param beta: 速度
        :return: Tmax
        """
        gamma_squal = 1 / (1 - beta ** 2)
        return 2 * 0.5 * beta ** 2 * gamma_squal

    @staticmethod
    def calculate_beta(particle_energy, particle_mass):
        """
        计算相对论粒子的速度
        :param particle_energy: 能量
        :param particle_mass: 静止质量
        :return: beta速度（0-1）
        """
        return np.sqrt(1 - (particle_mass ** 2 / particle_energy ** 2))

    @staticmethod
    def calculate_derive(func, x, dx=0.01):
        """
        计算一个一维的函数的导数, 误差来源是截断误差（泰勒）
        :param func: 某一个函数
        :param x: 需要导数的那个地方
        :param dx: 步长
        :return: 导数的值
        """
        return (func(x + dx) - func(x)) / dx