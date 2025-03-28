from matplotlib import pyplot as plt
from deeplearning.predict.base_predict import BasePredict
import numpy as np
import mplhep as hp


class BindEnergyPredict(BasePredict):
    def __init__(self, x_train, y_train, model):
        super(BindEnergyPredict, self).__init__(x_train, y_train, model)
        self.A_data = self.x_train[:, 2].flatten()
        hp.style.use('ALICE')

    def draw_3d(self):
        pass

    def draw_2d(self):
        """
        横坐标是A，纵坐标是结合能
        显然，A的数据在x_train的第三列
        """
        A_data = self.x_train[:, 2].flatten()

        plt.plot(A_data, self.y_train, "ro", ms=1, label="real data")
        plt.plot(A_data, self.predict(self.x_train), "bo", ms=1, label="predict data")

        plt.xlabel("A")
        plt.ylabel("$E_b$")
        plt.legend(loc="best")

        plt.show()

    def draw_error_hist(self):
        """
        画出误差直方图
        """
        plt.hist(self.predict(self.x_train) / self.y_train, bins=100, label="compare")

        plt.legend(loc="best")
        plt.xlabel("predict data / real data")
        plt.ylabel("entries")

        plt.show()

    def draw_relative_error(self):
        """
        画出相对误差
        """
        A_data = self.x_train[:, 2].flatten()

        plt.plot(A_data, np.abs((self.predict(self.x_train) - self.y_train) / self.y_train), "go", label="relative",
                 ms=1)

        plt.legend(loc="best")
        plt.xlabel("A")
        plt.ylabel("|(predict data - real data) / real data|")

        plt.show()

    def draw_hist_relative_error(self):
        """
        画出相对误差的分布
        """
        plt.hist(np.abs((self.predict(self.x_train) - self.y_train) / self.y_train), bins=100, label="relative error")

        plt.legend(loc="best")
        plt.xlabel("|(predict data - real data) / real data|")
        plt.ylabel("entries")

        plt.show()
