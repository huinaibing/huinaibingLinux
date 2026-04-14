#ifndef MYCHARGEDREADER
#define MYCHARGEDREADER

#include "TProfile3D.h"
#include "baseFile.h"
#include "util.h"


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


class chargedReader : public BaseFileReader
{
protected:
    TProfile3D *proOrigin;
    TProfile3D *proOriginpt;

public:
    chargedReader(const char *file_path, const char *dir_name) : BaseFileReader(file_path, dir_name)
    {
        if (!this->dir)
        {
            std::cout << "dir in basefile reader not init" << std::endl;
            return;
        }
        proOrigin = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>("hCharged");
        proOriginpt = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>("hChargedMeanpt");
    }

    TProfile *getSpecifiedCentBin(int centBin, bool isC22)
    {
        TProfile2D *protemp2d;
        if (isC22)
        {
            TProfile3D *protemp3d = (TProfile3D *)this->proOrigin->Clone("origin_clone_tmp");
            protemp2d = (TProfile2D *)protemp3d->Project3DProfile("xy")->Clone(
                std::to_string(Utils::get_timestamp_nanoseconds()).c_str());
        }
        else
        {
            TProfile3D *protemp3dpt = (TProfile3D *)this->proOriginpt->Clone("origin_clone_pt_tmp");
            protemp2d = (TProfile2D *)protemp3dpt->Project3DProfile("xy")->Clone(
                std::to_string(Utils::get_timestamp_nanoseconds()).c_str());
        }

        protemp2d->GetXaxis()->SetRange(centBin, centBin);
        return protemp2d->ProfileY();
    }

    TProfile *getSpecifiedCentBin(int centBin, int bootstrapIdx, bool isC22)
    {
        TProfile2D *protemp2d;
        if (isC22)
        {
            TProfile3D *protemp3d = (TProfile3D *)this->proOrigin->Clone("origin_clone_tmp");
            protemp3d->GetZaxis()->SetRange(bootstrapIdx + 1, bootstrapIdx + 1);
            protemp2d = protemp3d->Project3DProfile("xy");
        }
        else
        {
            TProfile3D *protemp3dpt = (TProfile3D *)this->proOriginpt->Clone("origin_clone_pt_tmp");
            protemp3dpt->GetZaxis()->SetRange(bootstrapIdx + 1, bootstrapIdx + 1);
            protemp2d = protemp3dpt->Project3DProfile("xy");
        }
        protemp2d->GetXaxis()->SetRange(centBin, centBin);
        return protemp2d->ProfileY();
    }

    TProfile *getSpecifiedCentBin_jackknife(int centBin, int jackIdx, bool isC22)
    {
        TProfile3D *tmp3D = nullptr;
        if (isC22)
        {
            tmp3D = (TProfile3D *)this->proOrigin->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());
        }
        else
        {
            tmp3D = (TProfile3D *)this->proOriginpt->Clone(std::to_string(Utils::get_timestamp_nanoseconds()).c_str());
        }

        // 清除指定的 jackknife 样本层 (Z轴)
        clearZBinLayer(tmp3D, jackIdx + 1);

        // 投影到 2D (xy) 并取指定中心度 bin
        TProfile2D *protemp2d = tmp3D->Project3DProfile("xy");
        protemp2d->GetXaxis()->SetRange(centBin, centBin);

        // 投影到 1D 作为结果
        TProfile *resProfile = protemp2d->ProfileY();

        // 清理临时对象
        delete tmp3D;

        return resProfile;
    }
};


#endif
