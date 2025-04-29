import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import numpy as np
from torch.autograd import grad

from fnet import FNet
from tqdm import tqdm


class Trainer:
    def __init__(self, model=FNet, optimizer=torch.optim.Adam, loss_function=nn.MSELoss, lr=1e-3):
        self.model = model()
        self.optimizer = optimizer(self.model.parameters(), lr=lr)
        self.loss_function = loss_function()

        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.nsamples = 100
        self.xtrain = torch.linspace(
            0, 2, 100,
            device=self.device, requires_grad=True
        ).reshape(self.nsamples, -1)
        # 这个xtrain相当于数据集

    def plot(self):
        y = self.model(self.xtrain).cpu().detach().numpy()
        x = self.xtrain.detach().cpu().numpy()

        ground_truth = np.exp(-0.2 * x) * np.sin(x)
        # 这个ground truth相当于一般机器学习算法中的优化目标
        plt.plot(x, y * x, label='network')
        plt.plot(x, ground_truth, "--", label='groundtruth')
        plt.legend(loc="best")
        plt.show()

    def train(self, n_iter=150):
        for _ in tqdm(range(n_iter)):
            # 通过输入，预测输出值
            y = self.model(self.xtrain) * self.xtrain
            # 对输入求导
            dydx = grad(y, self.xtrain,
                        grad_outputs=torch.ones_like(y), create_graph=True,
                        )[0]
            left = dydx + 0.2 * y
            right = torch.exp(-0.2 * self.xtrain) * torch.cos(self.xtrain)
            # 上面的代码分别计算了等号左右两边的值
            self.optimizer.zero_grad()
            # 使用平方误差
            loss = self.loss_function(left, right)
            loss.backward()
            self.optimizer.step()


if __name__ == '__main__':
    trainer = Trainer()
    trainer.train()
    trainer.plot()
