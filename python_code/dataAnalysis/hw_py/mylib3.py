from hw_py.mylib2 import Mylib2
import sympy as sp
import matplotlib.pyplot as plt

class Mylib3(Mylib2):
    def __init__(self):
        super().__init__()
        self.x = sp.symbols('x')

    @staticmethod
    def func(x):
        return sp.sin(x) * sp.cos(x)

    @staticmethod
    def finite_diff(f, x, dx=0.01):
        """
        forward
        :param f:
        :param x:
        :param dx:
        :return:
        """
        return (f(x + dx) - f(x)) / dx

    @staticmethod
    def plot(xlist, ylist, xlabel="", ylabel="", title=""):
        plt.plot(xlist, ylist, "-")
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.title(title)
        plt.show()


