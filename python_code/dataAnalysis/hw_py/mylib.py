import numpy as np


class XQYLib:
    """
    3月14日计算物理作业（第二次作业），内容如下
    1. 把dz/dt写出稳定演化算法
    2. 最小二乘法拟合算法
    3. 写一个多项式函数
    """

    def __init__(self):
        pass

    @staticmethod
    def stable_update(z0: float = 10.0, omega: float = 1.0, dt: float = 0.01, nsteps: int = 1000) -> list:
        """
        对应第一个作业
        dz / dt = i omega z 的稳定演化版本，返回不同时间点的演化的值
        :param z0: 矢量的长度
        :param omega: 矢量旋转的角频率
        :param dt: 时间步长
        :param nsteps: 迭代次数
        :return: 包括所有时间点的演化值的list
        """
        z_arr = [z0]
        tmp = z0
        for i in range(nsteps):
            tmp = ((1 + dt * omega * 1.0j / 2) / (1 - dt * omega * 1.0j / 2)) * tmp
            z_arr.append(tmp)
        return z_arr

    @staticmethod
    def least_squares_method(x: np.ndarray | list, y: np.ndarray | list, n: int = 2) -> list:
        """
        最小二乘法拟合函数
        对应第二个作业
        :param x: 待拟合数据点的x坐标
        :param y: 待拟合数据点的y坐标
        :param n: 使用多项式的阶数
        :return: 包含n + 1个参数的list，顺序为从a0到an
        """
        x = np.array(x)
        y = np.array(y)
        if x.ndim != 1 or y.ndim != 1:
            raise ValueError('x和y应该是1d array')

        if len(x) != len(y):
            raise ValueError("x和y维度不等")

        if not isinstance(n, int) or n < 0:
            raise ValueError('n的阶数应该是个整数，并且大于等于0')

        sum_x_pow = [(x**i).sum() for i in range(2 * n + 1)]
        k = np.zeros((n + 1, n + 1))
        for i in range(n + 1):
            for j in range(n + 1):
                k[i][j] = sum_x_pow[i + j]

        b = np.zeros((n + 1, 1))
        for i in range(n + 1):
            b[i][0] = (y * x**i).sum()

        return (np.linalg.inv(k) @ b).tolist()

    @staticmethod
    def polyn(a: list, x: np.ndarray) -> np.ndarray:
        """
        （对应作业3）
        返回一个多项式的计算结果, x是一个vector，会对里面所有的值计算结果
        (sum_{i=0}^n a_i * x**i)
        :param a: 多项式的系数列表，类型为list
        :param x: 数据类型为np.ndarray, shape为一维
        :return: shape和输入x一样的vector，其中元素的值是对应x计算的结果
        """
        if type(a) != list:
            raise TypeError("a的类型应为list")

        if len(x.shape) != 1:
            raise TypeError("x的shape不对")

        # 算出来多项式的每一项的系数然后对第一维求和就可以了
        return np.sum(np.array([a[i] * x ** i for i in range(len(a))]), axis=0)
