#ifndef FLOWCONTAINERREADER
#define FLOWCONTAINERREADER

#include "FlowContainer.h"
#include "TProfile2D.h"


int get_random_int(int min, int max)
{
    // // 1. 真随机种子（硬件级随机，只初始化一次）
    // static std::random_device rd;
    // // 2. 随机数引擎（高性能，最常用）
    // static std::mt19937 gen(rd());
    // // 3. 定义整数均匀分布
    // std::uniform_int_distribution<> dist(min, max);

    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}


enum FCGraphName
{
    c22 = 1,
    c32,
    c24,
    c34,
    c22Full,
    c22TrackWeight,
    c32TrackWeight,
    c24TrackWeight,
    c34TrackWeight,
    c22FullTrackWeight,
    covV2Pt,
    covV3Pt,
    ptSquareAve,
    ptAve,
    hMeanPt,
    c22pure,
    c32pure
};


class FlowContainerTProfile2DReader
{
public:
    TProfile *h_c22;
    TProfile *h_c32;
    TProfile *h_c24;
    TProfile *h_c34;
    TProfile *h_c22Full;
    TProfile *h_c22TrackWeight;
    TProfile *h_c32TrackWeight;
    TProfile *h_c24TrackWeight;
    TProfile *h_c34TrackWeight;
    TProfile *h_c22FullTrackWeight;
    TProfile *h_covV2Pt;
    TProfile *h_covV3Pt;
    TProfile *h_ptSquareAve;
    TProfile *h_ptAve;
    TProfile *h_hMeanPt;
    TProfile *h_c22pure;
    TProfile *h_c32pure;

    bool isPID;

    /**
     * @brief 专门处理FC的2D直方图
     *
     * @param h_prof_total
     * @param name
     * @param isPID
     */
    FlowContainerTProfile2DReader(TProfile2D *h_prof_total, const char *name, bool isPID)
    {
        this->isPID = isPID;
        h_c22 = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                       FCGraphName::c22,
                                       FCGraphName::c22);
        h_c32 = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                       FCGraphName::c32,
                                       FCGraphName::c32);
        h_c24 = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                       FCGraphName::c24,
                                       FCGraphName::c24);
        h_c34 = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                       FCGraphName::c34,
                                       FCGraphName::c34);
        h_c22Full = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                           FCGraphName::c22Full,
                                           FCGraphName::c22Full);
        h_c22TrackWeight = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                                  FCGraphName::c22TrackWeight,
                                                  FCGraphName::c22TrackWeight);
        h_c32TrackWeight = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                                  FCGraphName::c32TrackWeight,
                                                  FCGraphName::c32TrackWeight);
        h_c24TrackWeight = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                                  FCGraphName::c24TrackWeight,
                                                  FCGraphName::c24TrackWeight);
        h_c34TrackWeight = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                                  FCGraphName::c34TrackWeight,
                                                  FCGraphName::c34TrackWeight);
        h_c22FullTrackWeight = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                                      FCGraphName::c22FullTrackWeight,
                                                      FCGraphName::c22FullTrackWeight);
        h_covV2Pt = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                           FCGraphName::covV2Pt,
                                           FCGraphName::covV2Pt);
        h_covV3Pt = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                           FCGraphName::covV3Pt,
                                           FCGraphName::covV3Pt);
        h_ptSquareAve = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                               FCGraphName::ptSquareAve,
                                               FCGraphName::ptSquareAve);
        h_ptAve = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                         FCGraphName::ptAve,
                                         FCGraphName::ptAve);
        h_hMeanPt = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                           FCGraphName::hMeanPt,
                                           FCGraphName::hMeanPt);
        if (isPID)
        {
            h_c22pure = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                               FCGraphName::c22pure,
                                               FCGraphName::c22pure);
            h_c32pure = h_prof_total->ProfileX(std::to_string(get_random_int(0, 1000000)).c_str(),
                                               FCGraphName::c32pure,
                                               FCGraphName::c32pure);
        }
    }

    double get_var_meanpt(int centBin)
    {
        double ptSquareAve = h_ptSquareAve->GetBinContent(centBin);
        double ptAve = h_ptAve->GetBinContent(centBin);
        double meanpt = h_hMeanPt->GetBinContent(centBin);
        double res = ptSquareAve - 2 * ptAve * meanpt + meanpt * meanpt;
        if (res <= 0)
            return 0;

        return sqrt(res);
    }

    double get_var_c22(int centBin)
    {
        double c22 = h_c22->GetBinContent(centBin);
        double c22full = h_c22Full->GetBinContent(centBin);
        double c24 = h_c24->GetBinContent(centBin);
        double res = c24 - 2 * c22full * c22full + c22 * c22;
        if (res <= 0)
            return 0;

        return sqrt(res);
    }
};


class FlowContainerReader
{
protected:
    bool isPID;
    TProfile2D *h_prof;
    TObjArray *sub_prof;
    const char *name;

public:
    FlowContainerReader(const char *file_path, const char *dir_name, const char *fc_name, const char *name, bool isPID)
    {
        this->isPID = isPID;
        this->name = name;
        TFile *f = TFile::Open(file_path);
        TDirectory *dir = (TDirectory *)f->Get(dir_name);
        FlowContainer *fc = (FlowContainer *)dir->Get(fc_name);
        h_prof = fc->GetProfile();
        sub_prof = fc->GetSubProfiles();
    }

    // =====================================
    double get_var_meanpt(int centBin)
    {
        FlowContainerTProfile2DReader fcrMain(this->h_prof, this->name, this->isPID);
        return fcrMain.get_var_meanpt(centBin);
    }
    double get_var_meanpt(int centBin, int bootstrapIdx)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)this->sub_prof->At(bootstrapIdx);
        FlowContainerTProfile2DReader fcrsub(h_sub_prof, this->name, this->isPID);
        return fcrsub.get_var_meanpt(centBin);
    }
    double get_val_meanpt_jackknife(int centBin, int jackIdx)
    {
        TObjArray *sub_prof_temp =
            (TObjArray *)this->sub_prof->Clone(std::to_string(get_random_int(0, 1000000)).c_str());
        sub_prof_temp->RemoveAt(jackIdx);

        TProfile2D *mergedJack = 0;
        if (jackIdx == 0)
            mergedJack = (TProfile2D *)sub_prof_temp->At(1);
        else
            mergedJack = (TProfile2D *)sub_prof_temp->At(0);

        mergedJack->Merge(sub_prof_temp);

        FlowContainerTProfile2DReader fcrsub(mergedJack, this->name, this->isPID);
        return fcrsub.get_var_meanpt(centBin);
    }
    // =====================================
    double get_var_c22(int centBin)
    {
        FlowContainerTProfile2DReader fcrMain(this->h_prof, this->name, this->isPID);
        return fcrMain.get_var_c22(centBin);
    }
    double get_var_c22(int centBin, int bootstrapIdx)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)this->sub_prof->At(bootstrapIdx);
        FlowContainerTProfile2DReader fcrsub(h_sub_prof, this->name, this->isPID);
        return fcrsub.get_var_c22(centBin);
    }
    double get_val_c22_jackknife(int centBin, int jackIdx)
    {
        TObjArray *sub_prof_temp =
            (TObjArray *)this->sub_prof->Clone(std::to_string(get_random_int(0, 1000000)).c_str());
        sub_prof_temp->RemoveAt(jackIdx);

        TProfile2D *mergedJack = 0;
        if (jackIdx == 0)
            mergedJack = (TProfile2D *)sub_prof_temp->At(1);
        else
            mergedJack = (TProfile2D *)sub_prof_temp->At(0);
        mergedJack->Merge(sub_prof_temp);

        FlowContainerTProfile2DReader fcrsub(mergedJack, this->name, this->isPID);
        return fcrsub.get_var_c22(centBin);
    }
    // =====================================
    double get_ptave(int centBin)
    {
        FlowContainerTProfile2DReader fcrMain(this->h_prof, this->name, this->isPID);
        return fcrMain.h_ptAve->GetBinContent(centBin);
    }
    double get_ptave(int centBin, int bootstrapIdx)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)this->sub_prof->At(bootstrapIdx);
        FlowContainerTProfile2DReader fcrsub(h_sub_prof, this->name, this->isPID);
        return fcrsub.h_ptAve->GetBinContent(centBin);
    }
    double get_ptave_jackknife(int centBin, int jackIdx)
    {
        TObjArray *sub_prof_temp =
            (TObjArray *)this->sub_prof->Clone(std::to_string(get_random_int(0, 1000000)).c_str());
        sub_prof_temp->RemoveAt(jackIdx);

        TProfile2D *mergedJack = 0;
        if (jackIdx == 0)
            mergedJack = (TProfile2D *)sub_prof_temp->At(1);
        else
            mergedJack = (TProfile2D *)sub_prof_temp->At(0);
        mergedJack->Merge(sub_prof_temp);

        FlowContainerTProfile2DReader fcrsub(mergedJack, this->name, this->isPID);
        return fcrsub.h_ptAve->GetBinContent(centBin);
    }
    // =====================================
    double get_c22_trackweight(int centBin)
    {
        FlowContainerTProfile2DReader fcrMain(this->h_prof, this->name, this->isPID);
        return fcrMain.h_c22TrackWeight->GetBinContent(centBin);
    }
    double get_c22_trackweight(int centBin, int bootstrapIdx)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)this->sub_prof->At(bootstrapIdx);
        FlowContainerTProfile2DReader fcrsub(h_sub_prof, this->name, this->isPID);
        return fcrsub.h_c22TrackWeight->GetBinContent(centBin);
    }
    double get_c22_trackweight_jackknife(int centBin, int jackIdx)
    {
        TObjArray *sub_prof_temp =
            (TObjArray *)this->sub_prof->Clone(std::to_string(get_random_int(0, 1000000)).c_str());
        sub_prof_temp->RemoveAt(jackIdx);

        TProfile2D *mergedJack = 0;
        if (jackIdx == 0)
            mergedJack = (TProfile2D *)sub_prof_temp->At(1);
        else
            mergedJack = (TProfile2D *)sub_prof_temp->At(0);
        mergedJack->Merge(sub_prof_temp);

        FlowContainerTProfile2DReader fcrsub(mergedJack, this->name, this->isPID);
        return fcrsub.h_c22TrackWeight->GetBinContent(centBin);
    }
    // ==============================================
    FlowContainerTProfile2DReader get_profile2d_reader()
    {
        return FlowContainerTProfile2DReader(this->h_prof, this->name, this->isPID);
    }
    FlowContainerTProfile2DReader get_profile2d_reader(int bootstrapIdx)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)this->sub_prof->At(bootstrapIdx);
        return FlowContainerTProfile2DReader(h_sub_prof, this->name, this->isPID);
    }
    FlowContainerTProfile2DReader get_profile2d_reader_jackknife(int jackIdx)
    {
        TObjArray *sub_prof_temp =
            (TObjArray *)this->sub_prof->Clone(std::to_string(get_random_int(0, 1000000)).c_str());
        sub_prof_temp->RemoveAt(jackIdx);

        TProfile2D *mergedJack = 0;
        if (jackIdx == 0)
            mergedJack = (TProfile2D *)sub_prof_temp->At(1);
        else
            mergedJack = (TProfile2D *)sub_prof_temp->At(0);
        mergedJack->Merge(sub_prof_temp);

        return FlowContainerTProfile2DReader(mergedJack, this->name, this->isPID);
    }
    // ==============================================
    double get_meanpt(int centBin)
    {
        FlowContainerTProfile2DReader fcrMain(this->h_prof, this->name, this->isPID);
        return fcrMain.h_hMeanPt->GetBinContent(centBin);
    }
    double get_meanpt(int centBin, int bootstrapIdx)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)this->sub_prof->At(bootstrapIdx);
        FlowContainerTProfile2DReader fcrsub(h_sub_prof, this->name, this->isPID);
        return fcrsub.h_hMeanPt->GetBinContent(centBin);
    }
    double get_meanpt_jackknife(int centBin, int jackIdx)
    {
        TObjArray *sub_prof_temp =
            (TObjArray *)this->sub_prof->Clone(std::to_string(get_random_int(0, 1000000)).c_str());
        sub_prof_temp->RemoveAt(jackIdx);

        TProfile2D *mergedJack = 0;
        if (jackIdx == 0)
            mergedJack = (TProfile2D *)sub_prof_temp->At(1);
        else
            mergedJack = (TProfile2D *)sub_prof_temp->At(0);
        mergedJack->Merge(sub_prof_temp);

        FlowContainerTProfile2DReader fcrsub(mergedJack, this->name, this->isPID);
        return fcrsub.h_hMeanPt->GetBinContent(centBin);
    }
};


#endif
