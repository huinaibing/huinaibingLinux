import pdg
import numpy as np
from scipy.integrate import quad


class HrgEosIntegration:
    """
    通过直接调用PDG的python API来获取粒子数据
    """

    def __init__(self, particle_name):
        self.pdg_api = pdg.connect()
        self.particle = self.pdg_api.get_particle_by_name(particle_name)

    def dist_f(self, momentum, temper):
        """
        强子的分布函数
        :param momentum: 动量，单位GeV
        :param temper: 温度，单位GeV
        :return:
        """
        # pdg 给的自旋的写法是字符串的"1/2"，很奇怪
        spin_dof = eval(self.particle.quantum_J) * 2 + 1
        mass = self.particle.mass
        eta = -1 if self.particle.is_boson else -1
        return spin_dof / (
                np.exp(np.sqrt(momentum ** 2 + mass ** 2) / temper) + eta
        )

    def density_n(self, temper):
        """
        在给定温度情况下计算密度
        :param temper: 单位GeV
        :return:
        """
        kmax = 50 * temper  # 积分的上线
        return quad(
            lambda momentum: momentum ** 2 * self.dist_f(momentum, temper) / (2 * np.pi ** 2),
            0, kmax
        )[0]

    def pressure_p(self, temper):
        spin_dof = eval(self.particle.quantum_J) * 2 + 1
        mass = self.particle.mass
        eta = -1 if self.particle.is_boson else -1

        def integral(momentum):
            return momentum ** 2 * np.log(
                1 + eta * np.exp(
                    -(np.sqrt(momentum ** 2 + mass ** 2)) / temper)
            )

        coefficient = eta * temper * spin_dof / (2 * np.pi ** 2)

        kmax = 50 * temper
        return coefficient * quad(integral, 0, kmax)[0]

    def energy_density_e(self, temper):
        mass = self.particle.mass

        def integral(momentum):
            return momentum ** 2 * np.sqrt(momentum ** 2 + mass ** 2) \
                * self.dist_f(momentum, temper) / (2 * np.pi ** 2)

        kmax = 50 * temper
        return quad(integral, 0, kmax)[0]

    # def c_s_square(self, temper, dT=0.01):
    #     return (
    #             self.pressure_p(temper + dT) - self.pressure_p(temper)
    #     ) / (
    #             self.energy_density_e(temper + dT) - self.energy_density_e(temper)
    #     )
