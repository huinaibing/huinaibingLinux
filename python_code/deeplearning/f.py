import torch
import torch.nn as nn
from torch.autograd import grad
import matplotlib.pyplot as plt
from IPython.display import clear_output


class FNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(1, 32, device="cuda"),
            nn.SiLU(),
            nn.Linear(32, 256, device="cuda"),
            nn.SiLU(),
            nn.Linear(256, 32, device="cuda"),
            nn.SiLU(),
            nn.Linear(32, 1, device="cuda"),
            nn.SiLU()
        )

    def forward(self, x):
        return self.net(x)


nsamples = 100
xtrain = torch.linspace(0.0, 2.0, nsamples,
                        requires_grad=True, device="cuda").reshape(nsamples, 1)

net = FNet()


def plot(epoch, loss):
    y = net(xtrain).detach().cpu().numpy()
    x = xtrain.detach().cpu().numpy()
    ground_truth = torch.exp(-0.2 * xtrain) * torch.sin(xtrain)
    plt.plot(x, y * x, label="network")
    plt.plot(x, ground_truth.detach().cpu().numpy(), '--', label="ground truth")
    plt.legend(loc='best')
    plt.title("epoch:%s, loss=%.2E" % (epoch, loss))
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.show()
    clear_output(wait=True)


optimizer = torch.optim.Adam(net.parameters(), lr=1.0E-3)

for epoch in range(150):
    xi = xtrain
    y = xi * net(xi)  # constraint: f(0)=0
    dydx = grad(y, xi, grad_outputs=torch.ones_like(y), create_graph=True)[0]
    left = dydx + 0.2 * y
    right = torch.exp(-0.2 * xi) * torch.cos(xi)
    loss = torch.mean((left - right) ** 2)
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()
    plot(epoch, loss.item())
