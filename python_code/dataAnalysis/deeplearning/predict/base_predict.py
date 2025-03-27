import matplotlib.pyplot as plt
import mplhep as mp
import numpy as np


class BasePredict:
    def __init__(self, x_train, y_train, model):
        """
        使用scikit learn中的算法进行预测
        :param x_train: 训练集的输入
        :param y_train: 训练目标，和y是一一对应
        :param model: 模型，使用scikit learn里面的算法
        """
        self.x_train = x_train
        self.y_train = y_train
        self.model = model
        self.model.fit(self.x_train, self.y_train)

    def predict(self, x_data):
        return self.model.predict(x_data)

    def draw_2d_continue(self, curve_accuracy=10000):
        """
        这个是适用于单变量线性拟合的画图
        :param curve_accuracy:
        :return:
        """
        mp.style.use('ALICE')
        plt.scatter(self.x_train, self.y_train)
        x_continuous = np.linspace(min(self.x_train), max(self.x_train), curve_accuracy)
        plt.plot(x_continuous, self.model.predict(x_continuous))
