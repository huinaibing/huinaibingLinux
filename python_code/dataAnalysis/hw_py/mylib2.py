from tqdm import tqdm
from IPython.display import clear_output
from time import sleep
from scipy.constants import Planck
from scipy.constants import speed_of_light
from scipy.constants import Boltzmann

import matplotlib.pyplot as plt

from .mylib import XQYLib
import numpy as np


class Mylib2(XQYLib):
    @staticmethod
    def least_squares_method(x: np.ndarray | list, y: np.ndarray | list, n: int = 2) -> np.ndarray | list:
        """
        :param x:
        :param y:
        :param n:
        :return:
        """
        x = np.concatenate([np.array(x**i).reshape(1, -1) for i in range(n + 1)], axis=0).T
        y = np.matrix(y).reshape(-1, 1)

        return (np.linalg.inv(x.T @ x) @ x.T @ y).T.tolist()[0]

    @staticmethod
    def gradient_decent(x: np.ndarray | list, y: np.ndarray | list, n: int = 2,
                        lr: float = 0.001, niter: int = 10000,
                        animation=False) -> np.ndarray | list:
        """

        :param animation:
        :param niter:
        :param lr:
        :param x:
        :param y:
        :param n:
        :return:
        """
        x_ori = x
        y_ori = y

        x = np.concatenate([np.array(x**i).reshape(1, -1) for i in range(n + 1)], axis=0).T

        A = np.random.randn(1, n + 1)
        y = np.matrix(y).reshape(-1, 1)

        if animation:
            plt.figure()
        for i in range(niter):
            grad = -2 * x.T @ (y - x @ A.T)
            A = A - lr * grad.T

            if animation:
                plt.plot(x_ori, y_ori, 'ro')
                plt.plot(x_ori, Mylib2.polyn(A.tolist()[0], x_ori))
                plt.show()
                sleep(0.1)
                clear_output()

        return A.tolist()[0]

    @staticmethod
    def planck_black_body(nu: float, temper: float = 2.7):
        """

        :param nu:
        :param temper:
        :return:
        """
        return (2 * Planck * nu**3 / speed_of_light**2) * (1 / (np.exp(Planck * nu / temper / Boltzmann) - 1))

    @staticmethod
    def fit_planck_black_body(nu: np.ndarray, flux: np.ndarray):
        """

        :param nu:
        :param flux:
        :return:
        """
        x = nu
        y = np.log(2 * Planck * nu**3 / (flux * speed_of_light**2) + 1)

        return Mylib2.least_squares_method(x, y, 1)



