#ifndef PIDREADER
#define PIDREADER

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"
#include "util.h"
#include <iostream>
#include <random> // 核心头文件


// #define MERGE_BIN

#ifdef MERGE_BIN
double bins[] = {0,
                 0.601,
                 0.602,
                 0.603,
                 0.604,
                 0.605,
                 0.606,
                 0.607,
                 0.608,
                 0.609,
                 0.61,
                 0.611,
                 0.612,
                 0.613,
                 0.614,
                 0.615,
                 0.616,
                 0.617,
                 0.618,
                 0.619,
                 0.62,
                 0.621,
                 0.622,
                 0.623,
                 0.624,
                 0.625,
                 0.626,
                 0.627,
                 0.628,
                 0.629,
                 0.63,
                 0.631,
                 0.632,
                 0.633,
                 0.634,
                 0.635,
                 0.636,
                 0.637,
                 0.638,
                 0.639,
                 0.64,
                 0.641,
                 0.642,
                 0.643,
                 0.644,
                 0.645,
                 0.646,
                 0.647,
                 0.648,
                 0.649,
                 0.65,
                 0.651,
                 0.652,
                 0.653,
                 0.654,
                 0.655,
                 0.656,
                 0.657,
                 0.658,
                 0.659,
                 0.66,
                 0.661,
                 0.662,
                 0.663,
                 0.664,
                 0.665,
                 0.666,
                 0.667,
                 0.668,
                 0.669,
                 0.67,
                 0.671,
                 0.672,
                 0.673,
                 0.674,
                 0.675,
                 0.676,
                 0.677,
                 0.678,
                 0.679,
                 0.68,
                 0.681,
                 0.682,
                 0.683,
                 0.684,
                 0.685,
                 0.686,
                 0.687,
                 0.6880000000000001,
                 0.6890000000000001,
                 0.6900000000000001,
                 0.6910000000000001,
                 0.6920000000000001,
                 0.6930000000000001,
                 0.6940000000000001,
                 0.6950000000000001,
                 0.6960000000000001,
                 0.6970000000000001,
                 0.6980000000000001,
                 0.6990000000000001,
                 0.7000000000000001,
                 0.7010000000000001,
                 0.7020000000000001,
                 0.7030000000000001,
                 0.704,
                 0.705,
                 0.706,
                 0.707,
                 0.708,
                 0.709,
                 0.71,
                 0.711,
                 0.712,
                 0.713,
                 0.714,
                 0.715,
                 0.716,
                 0.717,
                 0.718,
                 0.719,
                 0.72,
                 0.721,
                 0.722,
                 0.723,
                 0.724,
                 0.725,
                 0.726,
                 0.727,
                 0.728,
                 0.729,
                 0.73,
                 0.731,
                 0.732,
                 0.733,
                 0.734,
                 0.735,
                 0.736,
                 0.737,
                 0.738,
                 0.739,
                 0.74,
                 0.741,
                 0.742,
                 0.743,
                 0.744,
                 0.745,
                 0.746,
                 0.747,
                 0.748,
                 0.749,
                 0.75,
                 0.751,
                 0.752,
                 0.753,
                 0.754,
                 0.755,
                 0.756,
                 0.757,
                 0.758,
                 0.759,
                 0.76,
                 0.761,
                 0.762,
                 0.763,
                 0.764,
                 0.765,
                 0.766,
                 0.767,
                 0.768,
                 0.769,
                 0.77,
                 0.771,
                 0.772,
                 0.773,
                 0.774,
                 0.775,
                 0.776,
                 0.777,
                 0.778,
                 0.779,
                 0.78,
                 0.781,
                 0.782,
                 0.783,
                 0.784,
                 0.785,
                 0.786,
                 0.787,
                 0.788,
                 0.789,
                 0.79,
                 0.791,
                 0.792,
                 0.793,
                 0.794,
                 0.795,
                 0.796,
                 0.797,
                 0.798,
                 0.799,
                 0.8,
                 0.801,
                 0.802,
                 0.803,
                 0.804,
                 0.805,
                 0.806,
                 0.807,
                 0.808,
                 0.809,
                 3};

int nBins = sizeof(bins) / sizeof(bins[0]) - 1;
#endif


void clearSpecificBin(TProfile3D *prof, int xbin, int ybin, int zbin)
{
    // 1. 获取全局 bin 编号
    int globalBin = prof->GetBin(xbin, ybin, zbin);

    // 2. 重置所有统计量
    prof->SetBinContent(globalBin, 0); // 重置均值
    prof->SetBinError(globalBin, 0);   // 重置误差
    prof->SetBinEntries(globalBin, 0); // 重置事件数
}

void clearZBinLayer(TProfile3D *prof, int zbin)
{
    // 获取 x 和 y 轴的 bin 数
    int nbinsx = prof->GetXaxis()->GetNbins();
    int nbinsy = prof->GetYaxis()->GetNbins();

    // 遍历该 zbin 下的所有 x 和 y bin
    for (int xbin = 1; xbin <= nbinsx; ++xbin)
    {
        for (int ybin = 1; ybin <= nbinsy; ++ybin)
        {
            // 复用上面的 clearSpecificBin 函数
            clearSpecificBin(prof, xbin, ybin, zbin);
        }
    }
}


class PIDReader
{
protected:
    TProfile3D *PIDProfileUp;
    TProfile3D *PIDprofileDown;
    TProfile3D *PIDProfileExtra;
    TDirectory *dir;


public:
    /**
     * @brief Construct a new PIDReader object
     *
     * @param file_path
     * @param dir_name
     * @param particleType 1 = pi 2 = ka 3 = pr
     */
    PIDReader(const char *file_path, const char *dir_name, int particleType)
    {
        TFile *f = TFile::Open(file_path);
        dir = (TDirectory *)f->Get(dir_name);

        std::string upName;
        std::string downName;

        switch (particleType)
        {
        case 1:
            upName = "hPion";
            downName = "hChargedPionFull";
            break;
        case 2:
            upName = "hKaon";
            downName = "hChargedKaonFull";
            break;
        case 3:
            upName = "hProton";
            downName = "hChargedProtonFull";
            break;
        default:
            break;
        }

        this->PIDProfileUp = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>(upName.c_str());
        this->PIDprofileDown = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>(downName.c_str());
    }

    TProfile *getSpecifiedCentBin(int centBin)
    {
        TProfile2D *pro2dUp = this->PIDProfileUp->Project3DProfile("xy");
        TProfile2D *pro2dDown = this->PIDprofileDown->Project3DProfile("xy");

        pro2dUp->GetXaxis()->SetRange(centBin, centBin);
        pro2dDown->GetXaxis()->SetRange(centBin, centBin);

#ifndef MERGE_BIN
        TProfile *pro1DUp = pro2dUp->ProfileY();
        TProfile *pro1DDown = pro2dDown->ProfileY();
#else
        TProfile *pro1DUptmp = pro2dUp->ProfileY();
        TProfile *pro1DDowntmp = pro2dDown->ProfileY();

        TProfile *pro1DUp = (TProfile *)pro1DUptmp->Rebin(nBins, "", bins);
        TProfile *pro1DDown = (TProfile *)pro1DDowntmp->Rebin(nBins, "", bins);

#endif

        TProfile *resProfile = new TProfile(std::to_string(Utils::get_timestamp_nanoseconds()).c_str(),
                                            "",
                                            pro1DUp->GetNbinsX(),
                                            pro1DUp->GetXaxis()->GetXbins()->GetArray());

        for (int idxPt = 1; idxPt <= pro1DUp->GetNbinsX(); idxPt++)
        {
            double upVal = pro1DUp->GetBinContent(idxPt);
            double weight = pro1DUp->GetBinEntries(idxPt);
            double downVal = pro1DDown->GetBinContent(idxPt);

            if (downVal == 0)
                continue;


            double ptBinres = upVal * upVal / downVal;

            // std::cout << ptBinres << std::endl;

            resProfile->SetBinContent(idxPt, ptBinres * weight);
            resProfile->SetBinEntries(idxPt, weight);
        }

        return resProfile;
    }


    TProfile *getSpecifiedCentBin(int centBin, const char *graph_name)
    {
        TProfile2D *protemp2d =
            dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>(graph_name)->Project3DProfile("xy");
        protemp2d->GetXaxis()->SetRange(centBin, centBin);

#ifndef MERGE_BIN
        return protemp2d->ProfileY();
#else
        return (TProfile *)protemp2d->ProfileY()->Rebin(nBins, "", bins);
#endif
    }

    TProfile *getSpecifiedCentBin(int centBin, int bootstrapIdx)
    {
        TProfile3D *tmpUp3D =
            (TProfile3D *)this->PIDProfileUp->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());
        TProfile3D *tmpDown3D =
            (TProfile3D *)this->PIDprofileDown->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());

        tmpUp3D->GetZaxis()->SetRange(bootstrapIdx + 1, bootstrapIdx + 1);
        tmpDown3D->GetZaxis()->SetRange(bootstrapIdx + 1, bootstrapIdx + 1);

        TProfile2D *pro2dUp = tmpUp3D->Project3DProfile("xy");
        TProfile2D *pro2dDown = tmpDown3D->Project3DProfile("xy");

        pro2dUp->GetXaxis()->SetRange(centBin, centBin);
        pro2dDown->GetXaxis()->SetRange(centBin, centBin);

        TProfile *pro1DUp = pro2dUp->ProfileY();
        TProfile *pro1DDown = pro2dDown->ProfileY();

        TProfile *resProfile = new TProfile(std::to_string(Utils::get_timestamp_nanoseconds()).c_str(),
                                            "",
                                            pro1DUp->GetNbinsX(),
                                            pro1DUp->GetXaxis()->GetXbins()->GetArray());

        for (int idxPt = 1; idxPt <= pro1DUp->GetNbinsX(); idxPt++)
        {
            double upVal = pro1DUp->GetBinContent(idxPt);
            double weight = pro1DUp->GetBinEntries(idxPt);
            double downVal = pro1DDown->GetBinContent(idxPt);

            if (downVal == 0)
                continue;


            double ptBinres = upVal * upVal / downVal;

            // std::cout << ptBinres << std::endl;

            resProfile->SetBinContent(idxPt, ptBinres * weight);
            resProfile->SetBinEntries(idxPt, weight);
        }

        delete tmpUp3D;
        delete tmpDown3D;

        return resProfile;
    }

    TProfile *getSpecifiedCentBin_jackknife(int centBin, int jackIdx)
    {
        TProfile3D *tmpUp3D =
            (TProfile3D *)this->PIDProfileUp->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());
        TProfile3D *tmpDown3D =
            (TProfile3D *)this->PIDprofileDown->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());

        clearZBinLayer(tmpUp3D, jackIdx + 1);
        clearZBinLayer(tmpDown3D, jackIdx + 1);

        TProfile2D *pro2dUp = tmpUp3D->Project3DProfile("xy");
        TProfile2D *pro2dDown = tmpDown3D->Project3DProfile("xy");

        pro2dUp->GetXaxis()->SetRange(centBin, centBin);
        pro2dDown->GetXaxis()->SetRange(centBin, centBin);

        TProfile *pro1DUp = pro2dUp->ProfileY();
        TProfile *pro1DDown = pro2dDown->ProfileY();

        TProfile *resProfile = new TProfile(std::to_string(Utils::get_timestamp_nanoseconds()).c_str(),
                                            "",
                                            pro1DUp->GetNbinsX(),
                                            pro1DUp->GetXaxis()->GetXbins()->GetArray());

        for (int idxPt = 1; idxPt <= pro1DUp->GetNbinsX(); idxPt++)
        {
            double upVal = pro1DUp->GetBinContent(idxPt);
            double weight = pro1DUp->GetBinEntries(idxPt);
            double downVal = pro1DDown->GetBinContent(idxPt);

            if (downVal == 0)
                continue;


            double ptBinres = upVal * upVal / downVal;

            // std::cout << ptBinres << std::endl;

            resProfile->SetBinContent(idxPt, ptBinres * weight);
            resProfile->SetBinEntries(idxPt, weight);
        }

        delete tmpUp3D;
        delete tmpDown3D;

        return resProfile;
    }

    TProfile *getSpecifiedCentBin_jackknife(int centBin, const char *graph_name, int jackIdx)
    {
        // 1. 获取原始的 3D profile（与原函数一致）
        TProfile3D *original3D = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>(graph_name);

        // 2. 克隆 3D profile（避免修改原始对象，使用时间戳保证唯一命名）
        TProfile3D *tmp3D = (TProfile3D *)original3D->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());

        // 3. 清除指定的 Jackknife Z 轴 bin 层（参考第三个函数的逻辑）
        clearZBinLayer(tmp3D, jackIdx + 1);

        // 4. 投影到 xy 平面（与原函数一致）
        TProfile2D *pro2d = tmp3D->Project3DProfile("xy");

        // 5. 锁定中心度 bin（与原函数一致）
        pro2d->GetXaxis()->SetRange(centBin, centBin);

        // 6. 提取 Y 轴的 1D profile（与原函数一致）
        TProfile *resProfile = pro2d->ProfileY();

        // 7. 清理临时克隆的 3D 对象（投影后的 2D/1D 对象不受影响，ROOT 会管理其所有权）
        delete tmp3D;

        return resProfile;
    }
};

#endif
