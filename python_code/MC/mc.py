import random
import numpy as np


class Hand:
    def __init__(self, win_array):
        self.num_array = [i for i in range(0, 6)]
        self.win_array = win_array
        self.win_num = 0

    def show_finger(self):
        return random.choice(self.num_array)

    def win(self):
        self.win_num += 1


class Game:
    def __init__(self, player_num):
        self.player_num = player_num
        self.hands = [Hand([i, i + 3, i + 6]) for i in range(1, player_num + 1)]
        self.round = 0

    def play(self):
        res = 0
        for hand in self.hands:
            res += hand.show_finger()
        self.round += 1
        return res

    def judge(self, res):
        for hand in self.hands:
            if res in hand.win_array:
                hand.win()


def correlation(x, y):
    return np.mean((x - x.mean()) * (y - y.mean())) - np.mean(x - x.mean()) * np.mean(
        y - y.mean()
    )


from scipy.integrate import quad
from scipy.stats import norm


def entropy(f):
    return quad(lambda x: -f(x) * np.log(f(x)), -10, 10)[0]


def entropy_2(f, x):
    return -np.log(f(x)).mean()


def gaussian(x, mu=0, sigma=1):
    return np.exp(-((x - mu) ** 2) / (2 * sigma**2)) / np.sqrt(2 * np.pi * sigma**2)


if __name__ == "__main__":
    print(entropy(gaussian))
    print(entropy(lambda x: gaussian(x, mu=0, sigma=2)))
    xs = np.random.randn(10000000)
    print(entropy_2(gaussian, xs))
    print(entropy_2(lambda x: gaussian(x, mu=0, sigma=2), xs))
