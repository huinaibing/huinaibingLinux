import torch
import torch.nn as nn


class FNet(nn.Module):
    def __init__(self):
        super(FNet, self).__init__()
        self.device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
        self.net = nn.Sequential(
            nn.Linear(1, 32, device=self.device),
            nn.SiLU(),
            nn.Linear(32, 256, device=self.device),
            nn.SiLU(),
            nn.Linear(256, 32, device=self.device),
            nn.SiLU(),
            nn.Linear(32, 1, device=self.device)
        )

    def forward(self, x):
        return self.net(x)
