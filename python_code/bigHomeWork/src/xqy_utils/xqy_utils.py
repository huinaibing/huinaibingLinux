# 工具类
# copyright (c) Qiuyu Xia 
import numpy as np


class XQYUtils:
    @staticmethod
    def get_R_v_v1(psi):
        """
        获取绕z轴旋转的旋转矩阵
        :param psi: 旋转角度, 用pi表示（那个单位制叫啥我忘记了）
        :return np.matrix: shape=(3, 3)
        """
        return np.matrix(
            [
                [np.cos(psi), np.sin(psi), 0],
                [-np.sin(psi), np.cos(psi), 0],
                [0, 0, 1]
            ]
        )
    
    @staticmethod
    def get_R_v1_v2(theta):
        """
        获取绕y轴旋转的旋转矩阵
        :param theta: 旋转角度
        :return np.matrix: shape=(3, 3)
        """
        return np.matrix(
            [
                [np.cos(theta), 0, -np.sin(theta)],
                [0, 1, 0],
                [np.sin(theta), 0, np.cos(theta)]
            ]
        )
    
    @staticmethod
    def get_v2_b(phi):
        """
        获取绕x轴旋转的旋转矩阵
        :param phi: 旋转角度
        :return np.matrix: shape=(3, 3)
        """
        return np.matrix(
            [
                [1, 0, 0],
                [0, np.cos(phi), np.sin(phi)],
                [0, -np.sin(phi), np.cos(phi)]
            ]
        )
    