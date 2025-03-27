from matplotlib import pyplot as plt
from deeplearning.predict.base_predict import BasePredict


class BindEnergyPredict(BasePredict):
    def draw_3d(self):
        pass

    def draw_2d(self):
        """
        横坐标是A，纵坐标是结合能，和ppt上的图保持一致
        显然，A的数据在x_train的第三列
        """
        A_data = self.x_train[:, 2].flatten()
        plt.plot(A_data, self.y_train / A_data, "ro", ms=1)

        plt.plot(A_data, self.predict(self.x_train) / A_data, "bo", ms=1)
        plt.show()

        # 画出误差直方图
        plt.hist(self.predict(self.x_train) / self.y_train, bins=100)
        plt.show()
