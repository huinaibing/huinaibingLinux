import torch
import torch.nn as nn
import matplotlib.pyplot as plt


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()

        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        print("device:", self.device)

        self.conv1 = nn.Conv2d(in_channels=1, out_channels=32, kernel_size=3, device=self.device)
        self.relu = nn.ReLU()
        self.conv2 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3, device=self.device)
        self.linear = nn.Linear(in_features=36864, out_features=5, device=self.device)

    def forward(self, x):
        return self.linear(nn.Flatten()(self.relu(self.conv2(self.relu(self.conv1(x)))))).to("cpu")


class MLP(nn.Module):
    def __init__(self):
        super(MLP, self).__init__()
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.fc1 = nn.Linear(in_features=1, out_features=32, device=self.device)
        self.fc2 = nn.Linear(in_features=32, out_features=64, device=self.device)
        self.fc3 = nn.Linear(in_features=64, out_features=1, device=self.device)

        self.tanh = nn.Tanh()

    def forward(self, x):
        x = self.fc1(x)
        x = self.tanh(x)
        x = self.fc2(x)
        x = self.tanh(x)
        x = self.fc3(x)
        x = self.tanh(x)
        return x


if __name__ == '__main__':
    mlp = MLP()
    x = torch.linspace(0, 2, 100, requires_grad=True, device="cuda").reshape(-1, 1)

    f = mlp(x)

    loss_function = nn.MSELoss()

    optimizer = torch.optim.Adam(mlp.parameters(), lr=0.01)

    for i in range(100):
        dfdx = torch.autograd.grad(f.to("cuda"), x, create_graph=True, grad_outputs=torch.ones_like(f))[0]

        loss = loss_function(dfdx + 0.2 * f, torch.exp(-0.2 * x) * torch.cos(x))

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    x2 = torch.linspace(0, 2, 100)
    plt.plot(x2, (mlp(x2.reshape(-1, 1))).reshape(-1))
    plt.show()
