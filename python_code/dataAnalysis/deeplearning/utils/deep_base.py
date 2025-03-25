import numpy as np


class DeepBase:
    def __init__(self):
        pass

    @staticmethod
    def relu(z):
        return np.where(z > 0, z, 0)

    @staticmethod
    def sigmoid(z):
        return 1 / (1 + np.exp(-z))

    @staticmethod
    def swish(z):
        return z * DeepBase.sigmoid(z)

    @staticmethod
    def one_simple_layer(x, dim_in, dim_out, activation=swish):
        """

        :param dim_out:
        :param dim_in:
        :param x:
        :param activation:
        :return:
        """
        return activation(np.random.randn(dim_out, dim_in) @ x + np.random.randn(dim_out, 1))
