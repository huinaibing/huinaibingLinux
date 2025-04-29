import torch
import torch.nn as nn


class BindEnergyPredict(nn.Module):
    """
    预测结合能
    """
    def __init__(self, input_size=2, output_size=1, activation=nn.ReLU()):
        super(BindEnergyPredict, self).__init__()

        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        print("device:", self.device)

        self.input_size = input_size
        self.output_size = output_size
        self.activation = activation

        self.fc1 = nn.Linear(in_features=input_size, out_features=16, device=self.device)
        self.fc2 = nn.Linear(in_features=16, out_features=32, device=self.device)
        self.fc3 = nn.Linear(in_features=32, out_features=1, device=self.device)

    def forward(self, x):
        x = self.fc1(x)
        x = self.activation(x)
        x = self.fc2(x)
        x = self.activation(x)
        x = self.fc3(x)
        return x
