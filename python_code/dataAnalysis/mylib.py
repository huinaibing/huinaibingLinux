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
    def polyn(a: list, x: np.ndarray) -> np.ndarray:
        """
        （对应作业3）
        返回一个多项式的计算结果, $\sum_0^n a_n \cdot x^n$
        （文档里打latex能识别吗）
        x是一个vector，会对里面所有的值计算结果
        :param a: 多项式的系数列表，类型为list
        :param x: 数据类型为np.ndarray, shape为一维
        :return: shape和输入x一样的vector，其中元素的值是对应x计算的结果
        """
        if type(a) != list:
            raise(TypeError("a的类型应为list"))
        
        if len(x.shape) != 1:
            raise(TypeError("x的shape不对"))

        # 把多项式的每一项的系数算出来然后对第一维求和就可以了
        return np.sum(np.array([a[i] * x**i for i in range(len(a))]), axis=0)