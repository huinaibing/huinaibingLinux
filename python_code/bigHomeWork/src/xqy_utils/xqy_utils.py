# 工具类
# copyright (c) Qiuyu Xia 
import numpy as np


class XQYUtils:
    @staticmethod
    def get_R_v_v1(psi):
        """
        获取绕z轴旋转的旋转矩阵
        :param psi: 旋转角度, 用pi表示（rad)
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

    @staticmethod
    def get_convert_omegaxyz_to_psithetaphi_dot(phi, theta):
        """
        计算从刚体的body角速度到姿态角导数的矩阵
        我class里面的angle顺序摆反了！！！！！！！！！！！！！！！
        QAQ
        :return np.matrix: shape=(3, 3)
        """
        return np.matrix(
            [
                [1, np.sin(phi) * np.tan(theta), np.cos(phi) * np.tan(theta)],
                [0, np.cos(phi), -np.sin(phi)],
                [0, np.sin(phi) / np.cos(theta), np.cos(phi) / np.cos(theta)]
            ]
        )

    @staticmethod
    def calculate_distance(x1, y1, z1, x2, y2, z2):
        """
        计算直线距离的
        """
        return np.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2 + (z1 - z2) ** 2)
