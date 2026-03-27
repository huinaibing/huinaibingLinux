#ifndef MYCHARGEDREADER
#define MYCHARGEDREADER

#include "TProfile3D.h"
#include "baseFile.h"
#include "util.h"

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
};


#endif
