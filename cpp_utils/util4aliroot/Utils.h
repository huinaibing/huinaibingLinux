#ifndef UTILS_H
#define UTILS_H

#include "TVector3.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "TParticle.h"

using std::swap;
using std::vector;

const double KAON_MASS_GEV = 0.493677;

namespace agglomerativeAlgorithm
{
    /**
     * @brief 2d point structure
     *
     */
    struct Point2D
    {
        double x; // x坐标
        double y; // y坐标
        Point2D(double x_, double y_) : x(x_), y(y_) {}
    };

    /**
     * @brief Get the Cluster Centroid
     *
     * @param cluster
     * @return Point2D
     */
    Point2D getClusterCentroid(const vector<Point2D> &cluster)
    {
        double sumX = 0.0;
        double sumY = 0.0;
        for (const auto &p : cluster)
        {
            sumX += p.x;
            sumY += p.y;
        }
        return Point2D(sumX / cluster.size(), sumY / cluster.size());
    }

    /**
     * @brief calculate the euclidean distance between two 2D points
     *
     * @param p1
     * @param p2
     * @return double
     */
    double euclideanDistance(const Point2D &p1, const Point2D &p2)
    {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief calulate the distance between two clusters (sets of 2D points)
     *        this function is used for agglomerativeClustering
     *        time complexity: O(N*M), N and M are the sizes of clusterA and clusterB
     *
     * @param clusterA
     * @param clusterB
     * @return double
     */
    double clusterDistance(const vector<Point2D> &clusterA, const vector<Point2D> &clusterB)
    {
        double minDist = INFINITY; // 初始化为无穷大
        // 遍历两簇中所有点对，找最小距离
        for (const auto &pA : clusterA)
        {
            for (const auto &pB : clusterB)
            {
                double dist = euclideanDistance(pA, pB);
                if (dist < minDist)
                {
                    minDist = dist;
                }
            }
        }
        return minDist;
    }

    /**
     * @brief perform agglomerative clustering on 2D points
     *        time complexity: O(N^3), N is the number of points
     *
     * @param points
     * @param threshold unit: cm
     * @return vector<vector<Point2D>>
     */
    vector<vector<Point2D>> agglomerativeClustering(const vector<Point2D> &points, double threshold)
    {
        // 初始化：每个点作为一个独立的簇
        vector<vector<Point2D>> clusters;
        for (const auto &p : points)
        {
            clusters.emplace_back(1, p); // 每个簇包含一个点
        }

        // 若点数≤1，直接返回（无需聚类）
        if (clusters.size() <= 1)
        {
            return clusters;
        }

        // 迭代合并簇，直到所有簇间距离>阈值
        while (true)
        {
            int numClusters = clusters.size();
            double minDist = INFINITY; // 记录当前最小簇间距离
            int idx1 = -1, idx2 = -1;  // 记录距离最近的两个簇的索引

            // 遍历所有簇对，找到距离最近的两个
            for (int i = 0; i < numClusters; ++i)
            {
                for (int j = i + 1; j < numClusters; ++j)
                { // j > i，避免重复计算
                    double dist = clusterDistance(clusters[i], clusters[j]);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        idx1 = i;
                        idx2 = j;
                    }
                }
            }

            // 检查停止条件：最小簇间距离>阈值
            if (minDist > threshold)
            {
                break;
            }

            // 合并距离最近的两个簇（确保idx1 < idx2，方便后续删除）
            if (idx1 > idx2)
            {
                swap(idx1, idx2);
            }

            // 合并簇idx1和idx2
            vector<Point2D> mergedCluster;
            mergedCluster.insert(mergedCluster.end(), clusters[idx1].begin(), clusters[idx1].end());
            mergedCluster.insert(mergedCluster.end(), clusters[idx2].begin(), clusters[idx2].end());

            // 构建新的簇集合（移除原idx1和idx2，加入合并后的簇）
            vector<vector<Point2D>> newClusters;
            for (int k = 0; k < numClusters; ++k)
            {
                if (k != idx1 && k != idx2)
                {
                    newClusters.push_back(clusters[k]);
                }
            }
            newClusters.push_back(mergedCluster);

            // 更新簇集合
            clusters = newClusters;
        }

        return clusters;
    }
}

namespace xqy
{
    /**
     * @brief calculate the angle cosine between two vectors
     *
     * @param x1
     * @param y1
     * @param z1
     * @param x2
     * @param y2
     * @param z2
     * @return float
     */
    float calcVectorAngle(double x1, double y1, double z1,
                          double x2, double y2, double z2)
    {
        // 1. 用TVector3创建两个向量
        TVector3 a(x1, y1, z1);
        TVector3 b(x2, y2, z2);

        // 2. 计算点积
        double dotProduct = a.Dot(b);

        // 3. 计算两个向量的模长
        double magA = a.Mag(); // |a| = sqrt(x1² + y1² + z1²)
        double magB = b.Mag(); // |b| = sqrt(x2² + y2² + z2²)

        // 4. 处理特殊情况：避免除以零（模长为0的向量无方向）
        if (magA < 1e-9 || magB < 1e-9)
        {
            std::cerr << "Error: 向量模长为0，无法计算夹角！" << std::endl;
            return 0;
        }

        // 5. 计算cosθ（注意：因浮点误差可能略超出[-1,1]，需截断）
        double cosTheta = dotProduct / (magA * magB);
        return TMath::Max(-1.0, TMath::Min(1.0, cosTheta)); // 截断到[-1,1]
    }

    /**
     * @brief 修正能量
     * @example Float_t* pars = clusterizer->GetCalibrationPars(segment);
     *
     * @param signal
     * @param pars
     * @return Float_t
     */
    Float_t Calibration(Float_t signal, Float_t *pars)
    {
        Float_t result = (signal - pars[1] * (signal / pars[0] * signal / pars[0])) / pars[0];

        if (result < 0)
        {
            result = 0;
        }

        return result;
    }

    float calculateEuclideDistance2D(float x1, float y1, float x2, float y2)
    {
        float square_res = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        return TMath::Sqrt(square_res);
    }

    double reconstructMotherMass(
        double x1a, double y1a, double z1a,
        double x1b, double y1b, double z1b,
        double E1,
        double x2a, double y2a, double z2a,
        double x2b, double y2b, double z2b,
        double E2)
    {
        // 步骤1：计算两个粒子的轨迹位移矢量（方向即动量方向）
        double dx1 = x1b - x1a; // 粒子1在x方向的位移
        double dy1 = y1b - y1a; // 粒子1在y方向的位移
        double dz1 = z1b - z1a; // 粒子1在z方向的位移

        double dx2 = x2b - x2a; // 粒子2在x方向的位移
        double dy2 = y2b - y2a; // 粒子2在y方向的位移
        double dz2 = z2b - z2a; // 粒子2在z方向的位移

        // 检查位移矢量是否有效（避免零矢量导致角度计算错误）
        if (dx1 == 0 && dy1 == 0 && dz1 == 0)
        {
            return -1;
        }
        if (dx2 == 0 && dy2 == 0 && dz2 == 0)
        {
            return -1;
        }

        // 步骤2：计算两个粒子动量方向的夹角（调用提供的函数）
        double cos_theta = calcVectorAngle(dx1, dy1, dz1, dx2, dy2, dz2);

        // 步骤3：计算每个粒子的动量大小（p*c，单位：GeV）
        // 相对论关系：E² = (p*c)² + (m*c²)² → (p*c)² = E² - (m*c²)²
        double p1c_sq = E1 * E1 - KAON_MASS_GEV * KAON_MASS_GEV;
        double p2c_sq = E2 * E2 - KAON_MASS_GEV * KAON_MASS_GEV;

        // 检查动量平方是否非负（能量需大于等于静止质量）
        if (p1c_sq < 0)
        {
            return -1;
        }
        if (p2c_sq < 0)
        {
            return -1;
        }

        double p1c = std::sqrt(p1c_sq); // 粒子1的动量大小（p1*c）
        double p2c = std::sqrt(p2c_sq); // 粒子2的动量大小（p2*c）

        // 步骤4：利用能动量守恒计算母粒子质量
        // 母粒子四动量平方：(M*c²)² = (E1+E2)² - (p1*c + p2*c · cosθ)²_total
        double E_total = E1 + E2;                                         // 总能量（GeV）
        double p_totalc_sq = p1c_sq + p2c_sq + 2 * p1c * p2c * cos_theta; // (p_total*c)²

        // 母粒子质量平方（(M*c²)²）
        double mother_mass_sq = E_total * E_total - p_totalc_sq;

        // 处理可能的数值误差导致的负质量平方（取绝对值）
        if (mother_mass_sq < 0)
        {
            return -1;
        }

        // 母粒子质量（GeV/c²）
        return std::sqrt(mother_mass_sq);
    }

}

namespace reconstuctMom
{
    /**
     * @brief Get the Mother Particle Rapidity, notice that Ionut distinguish eta and rapidity...
     *
     * @param part1
     * @param part2
     * @return double
     */
    double getMotherParticleRapidity(TParticle *part1, TParticle *part2)
    {
        double E_total = part1->Energy() + part2->Energy();
        double pz_total = part1->Pz() + part2->Pz();

        // Rapidity calculation
        if (E_total != pz_total)
        {
            return 0.5 * std::log((E_total + pz_total) / (E_total - pz_total));
        }
        else
        {
            // Avoid division by zero
            throw std::runtime_error("Error: E_total equals pz_total, rapidity is undefined.");
            return 0.0;
        }
    }

    /**
     * @brief Get the Mother Particle Pt
     *
     * @param part1
     * @param part2
     * @return double
     */
    double getMotherParticlePt(TParticle *part1, TParticle *part2)
    {
        double px_total = part1->Px() + part2->Px();
        double py_total = part1->Py() + part2->Py();

        return std::sqrt(px_total * px_total + py_total * py_total);
    }

    /**
     * @brief Get the Mother Particle Mass
     *
     * @param part1
     * @param part2
     * @return double
     */
    double getMotherParticleMass(TParticle *part1, TParticle *part2)
    {
        double momE = part1->Energy() + part2->Energy();
        double momPx = part1->Px() + part2->Px();
        double momPy = part1->Py() + part2->Py();
        double momPz = part1->Pz() + part2->Pz();

        return sqrt(momE * momE - momPx * momPx - momPy * momPy - momPz * momPz);
    }
}
#endif
