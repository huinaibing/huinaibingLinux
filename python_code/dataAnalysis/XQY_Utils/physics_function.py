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
    def calculate_tmax(beta, electron_mass=0.511):
        """
        计算入射粒子很重，速度不大时的Tmax（详情见粒子探测技术电离能损）自然单位制
        :param beta: 速度
        :return: Tmax MeV
        """
        gamma_squal = 1 / (1 - beta ** 2)
        return 2 * electron_mass * beta ** 2 * gamma_squal

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

    @staticmethod
    def fit(x, y, n):
        K = np.zeros((n + 1, n + 1))
        ks = [(x ** i).sum() for i in range(2 * n + 1)]
        for i in range(n + 1):
            for j in range(n + 1):
                K[i][j] = ks[i + j]
        b = np.zeros((n + 1, 1))
        for i in range(n + 1):
            b[i][0] = (y * x ** i).sum()

        return np.linalg.inv(K) @ b
    
    @staticmethod
    def calculate_beta_from_momentum(inv_mass, momentum):
        """
        计算beta，用动量去算
        """
        return np.sqrt(momentum**2 / (momentum**2 + inv_mass**2))
    
    @staticmethod
    def calculate_I(big_z):
        """
        计算I，就是那个BB公式里面的I
        单位MeV
        """
        return 16 * big_z**0.9 * 1e-6
    
    @staticmethod
    def calculate_BB_eq(small_z, big_z, big_a, beta):
        """
        计算BB公式
        """
        K = 0.3071 # K，公式前面那个，单位MeV*cm2/g
        Tmax = PhysicsFunction.calculate_tmax(beta) # Tmax MeV
        BB_I = PhysicsFunction.calculate_I(big_z) # I MeV
        return K * small_z**2 * (big_z / big_a) * (1 / beta**2) * (np.log(Tmax / BB_I) - beta**2)
    
    @staticmethod
    def calculate_delta_e(dedx, x, rho):
        """
        计算穿过一个介质后损耗能量
        使用的时候注意单位
        """
        return dedx * rho * x
    