import numpy as np


class BaseLayer:
    def __init__(self, input_size, output_size):
        self.W = np.random.rand(input_size, output_size)
        self.bias = np.random.rand(output_size, 1)
