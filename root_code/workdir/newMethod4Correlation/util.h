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
        double charged_c22_full = ch2d_reader.h_c22Full->GetBinContent(centBin);
        double pid_charged_c24 = pid2d_reader.h_c24->GetBinContent(centBin);
        double charged_c24 = ch2d_reader.h_c24->GetBinContent(centBin);


        double v22pow4 = pow(pid_charged_c22_gap, 4) / charged_c22_gap / charged_c22_gap;

        double v24pow4 = pow(2 * pid_charged_c22_full * charged_c22_full - pid_charged_c24, 4) /
                         pow(2 * charged_c22_full * charged_c22_full - charged_c24, 3);


        // double v22pow4 = TMath::Power(charged_c22_gap, 4) / charged_c22_gap / charged_c22_gap;

        // double v24pow4 = TMath::Power(2 * charged_c22_full * charged_c22_full - charged_c24, 4) /
        //                  TMath::Power(2 * charged_c22_full * charged_c22_full - charged_c24, 3);

        double res = v22pow4 - v24pow4;

        // std::cout << res << std::endl;

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

    static std::vector<double> get_double_gaus_params(TH1D *hist)
    {
        std::vector<double> params(6, 0.0); // 固定 6 个参数
        if (!hist || hist->GetEntries() < 10)
            return params;

        TAxis *xaxis = hist->GetXaxis();
        int nBins = xaxis->GetNbins();
        double binW = xaxis->GetBinWidth(1);

        // ======================
        // 1. 找主峰（最大值）
        // ======================
        int maxBin = hist->GetMaximumBin();
        double peakX = xaxis->GetBinCenter(maxBin);
        double peakY = hist->GetBinContent(maxBin);

        // ======================
        // 2. 估算主高斯 sigma (FWHM 方法)
        // ======================
        double halfMax = peakY * 0.5;
        int lBin = maxBin, rBin = maxBin;

        while (lBin > 1 && hist->GetBinContent(lBin) > halfMax)
            lBin--;
        while (rBin < nBins && hist->GetBinContent(rBin) > halfMax)
            rBin++;

        double fwhm = xaxis->GetBinCenter(rBin) - xaxis->GetBinCenter(lBin);
        double sigmaMain = fwhm / 2.3548; // 高斯 FWHM → sigma

        // 安全下限：防止 sigma 太小
        if (sigmaMain < binW * 0.5)
            sigmaMain = binW * 0.8;

        // ======================
        // 3. 设置双高斯初始参数
        // 专门解决：单高斯峰顶拟合不好
        // ======================

        // 高斯 1：主峰高斯（负责主体）
        params[0] = peakY * 1.2; // [0] 振幅1
        params[1] = peakX;       // [1] 均值1
        params[2] = sigmaMain;   // [2] sigma1

        // 高斯 2：修正高斯（同中心，专门修正峰顶）
        params[3] = peakY;     // [3] 振幅2（小幅度修正）
        params[4] = peakX;     // [4] 均值2 = 和主峰完全一样
        params[5] = sigmaMain; // [5] sigma2（更窄，拟合峰顶）

        // --- 如果你需要修正峰“太宽”，把上面一行改成 sigmaMain * 1.5 即可 ---

        // 安全保护
        if (params[0] <= 0)
            params[0] = 10;
        if (params[3] <= 0)
            params[3] = 5;

        return params;
    }

    static std::vector<double> GetSkewedGGInitPars(TH1D *h)
    {
        std::vector<double> pars(5, 1.0); // 初始化 5 个参数
        if (!h)
            return pars;

        // ========== 1. 获取峰位 mu (中心位置) ==========
        int binMax = h->GetMaximumBin();
        double mu = h->GetBinCenter(binMax);
        pars[1] = mu;

        // ========== 2. 获取振幅 A (峰高) ==========
        double A = h->GetMaximum() * 1.05; // 稍微放大一点，拟合更稳
        pars[0] = A;

        // ========== 3. 估算宽度 sigma ==========
        // 取半高全宽 FWHM 的 1/2.355 作为高斯宽度初始值
        double fwhm = h->GetRMS() * 1.5; // 用 RMS 做稳健估计
        double sigma = fwhm / 2.355;
        // 限制 sigma 范围，避免 0 或负数
        if (sigma < 1e-6)
            sigma = 0.01 * (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin());
        pars[2] = sigma;

        // ========== 4. 峰度 beta 初始值 ==========
        // 高斯分布 beta=2，最安全的初始值
        pars[3] = 2.0;

        // ========== 5. 偏度 alpha 初始值 ==========
        // 不对称峰默认从 0 或 1 开始，最稳定
        pars[4] = 0.5;

        return pars;
    }
};

#endif
