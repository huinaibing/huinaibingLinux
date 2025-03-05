import numpy as np


class ParticleData:
    def __init__(self, mass, spin, charge):
        self.mass = mass
        self.spin = spin
        self.charge = charge

    def calculate_spin_degree(self):
        return self.spin * 2 + 1

    def judge_fermion(self):
        return True if self.calculate_spin_degree() % 2 == 0 else False

    def spectral(self, p, temper=0.150):
        if self.judge_fermion():
            return self.calculate_spin_degree() / (np.exp(np.sqrt(p ** 2 + self.mass ** 2) / temper) - 1)
        else:
            return self.calculate_spin_degree() / (np.exp(np.sqrt(p ** 2 + self.mass ** 2) / temper) + 1)

