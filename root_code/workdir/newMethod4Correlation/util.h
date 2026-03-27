#ifndef UTIL
#define UTIL
#include "TProfile.h"
#include "flowContainerReader.h"
#include <random>

class Utils
{
public:
    static double getMeanXY(TProfile *proMerged, TProfile *proMergedpt)
    {
        double cov = 0;
        double point = 0;

        // std::cout << "===========================" << std::endl;

        for (int i = 1; i <= proMerged->GetNbinsX(); i++)
        {
            double weight = proMerged->GetBinEntries(i);

            if (weight == 0)
            {
                continue;
            }

            double meanpt = 0;
            if (proMergedpt)
            {
                meanpt = proMergedpt->GetBinContent(i);
            }
            else
            {
                meanpt = proMerged->GetBinCenter(i);
            }

            double val = proMerged->GetBinContent(i);
            cov += meanpt * val * weight;
            point += weight;


            // std::cout << "///////////////////////////////" << std::endl;
            // std::cout << "meanpt " << meanpt << std::endl;
            // std::cout << "binc enter " << proMerged->GetBinCenter(i) << std::endl;
            // std::cout << "///////////////////////////////" << std::endl;
        }
        cov /= point;
        return cov;
    }

    static double calculate_var_pid(int centBin,
                                    FlowContainerTProfile2DReader ch2d_reader,
                                    FlowContainerTProfile2DReader pid2d_reader)
    {
        if (ch2d_reader.isPID != false || pid2d_reader.isPID != true)
        {
            std::cout << "reader type error !!!!!!!!!!!!!!!!!!!" << std::endl;
            return 0;
        }

        double pid_charged_c22_gap = pid2d_reader.h_c22->GetBinContent(centBin);
        double charged_c22_gap = ch2d_reader.h_c22->GetBinContent(centBin);

        double pid_charged_c22_full = pid2d_reader.h_c22Full->GetBinContent(centBin);
        double charged_c22_full = ch2d_reader.h_c22Full->GetBinCenter(centBin);
        double pid_charged_c24 = pid2d_reader.h_c24->GetBinContent(centBin);
        double charged_c24 = ch2d_reader.h_c24->GetBinContent(centBin);


        double v22pow4 = pow(pid_charged_c22_gap, 4) / charged_c22_gap / charged_c22_gap;

        double v24pow4 = pow(2 * pid_charged_c22_full * charged_c22_full - pid_charged_c24, 4) /
                         pow(2 * charged_c22_full * charged_c22_full - charged_c24, 3);

        double res = v22pow4 - v24pow4;

        if (res <= 0)
            return 0;

        return sqrt(res);
    }

    static int get_random_int(int min, int max)
    {
        // 1. 真随机种子（硬件级随机，只初始化一次）
        static std::random_device rd;
        // 2. 随机数引擎（高性能，最常用）
        static std::mt19937 gen(rd());
        // 3. 定义整数均匀分布
        std::uniform_int_distribution<> dist(min, max);

        return dist(gen);
    }


    static double get_cov(TProfile *proMerged, double meanptInCent)
    {
        double cov = 0;
        double point = 0;

        for (int i = 1; i <= proMerged->GetNbinsX(); i++)
        {
            double weight = proMerged->GetBinEntries(i);

            if (weight == 0)
            {
                continue;
            }

            double meanpt = proMerged->GetBinCenter(i);

            double val = proMerged->GetBinContent(i);
            cov += (meanpt - meanptInCent) * val * weight;
            point += weight;
        }

        cov /= point;
        return cov;
    }

    static double get_cov(TProfile *proMerged, double meanptInCent, TProfile *proMergedpt)
    {
        double cov = 0;
        double point = 0;

        for (int i = 1; i <= proMerged->GetNbinsX(); i++)
        {
            double weight = proMerged->GetBinEntries(i);
            double meanpt = proMergedpt->GetBinContent(i);

            double val = proMerged->GetBinContent(i);
            cov += (meanpt - meanptInCent) * val * weight;
            point += weight;
        }

        cov /= point;
        return cov;
    }

    static inline int64_t get_timestamp_nanoseconds()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    }
};

#endif
