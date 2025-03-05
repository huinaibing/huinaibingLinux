import math as m


class PhysicsFunction:
    def __init__(self):
        pass

    @staticmethod
    def lorentz_gamma_factor(beta: float) -> float:
        """
        一个用于计算洛伦兹因子的函数
        :param beta: 速度（0~1之间）
        :return: gamma 若不合规则返回math.inf
        """
        if not 0 < beta < 1:
            print("error beta value")
            return m.inf
        return 1 / m.sqrt(1 - beta ** 2)

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

