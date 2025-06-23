import numpy as np
from scipy.linalg import kron
from IPython.display import Markdown as Md


class Qubit:
    def __init__(self, qubit):
        # s_up = np.matrix("1; 0")
        # s_down = np.matrix("0; 1")
        self.qubit = qubit

    def __mul__(self, other):
        return Qubit(kron(self.qubit, other.qubit))

    def __str__(self):
        # string = "$|"
        # for i in self.qubit.flat:
        #     string += str(i)
        # string += r"\rangle$"
        # return Md(string)
        ...

    @staticmethod
    def cvt_str2mat(string):
        if string == "up":
            return np.matrix("1; 0")
        elif string == "down":
            return np.matrix("0; 1")

    @staticmethod
    def cvt_2basis(bits="001"):
        res = Qubit(np.array([[1]]))
