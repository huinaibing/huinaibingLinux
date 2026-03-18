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

class PIDReader
{
protected:
    TProfile3D *PIDProfileUp;
    TProfile3D *PIDprofileDown;


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
        TDirectory *dir = (TDirectory *)f->Get(dir_name);

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

        TProfile *pro1DUp = pro2dUp->ProfileY();
        TProfile *pro1DDown = pro2dDown->ProfileY();

        TProfile *resProfile = new TProfile(std::to_string(Utils::get_random_int(0, 1000000)).c_str(),
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

    TProfile *getSpecifiedCentBin(int centBin, int bootstrapIdx)
    {
        TProfile3D *tmpUp3D =
            (TProfile3D *)this->PIDProfileUp->Clone(std::to_string(Utils::get_random_int(0, 1000000)).c_str());
        TProfile3D *tmpDown3D =
            (TProfile3D *)this->PIDprofileDown->Clone(std::to_string(Utils::get_random_int(0, 1000000)).c_str());

        tmpUp3D->GetZaxis()->SetRange(bootstrapIdx + 1, bootstrapIdx + 1);
        tmpDown3D->GetZaxis()->SetRange(bootstrapIdx + 1, bootstrapIdx + 1);

        TProfile2D *pro2dUp = tmpUp3D->Project3DProfile("xy");
        TProfile2D *pro2dDown = tmpDown3D->Project3DProfile("xy");

        pro2dUp->GetXaxis()->SetRange(centBin, centBin);
        pro2dDown->GetXaxis()->SetRange(centBin, centBin);

        TProfile *pro1DUp = pro2dUp->ProfileY();
        TProfile *pro1DDown = pro2dDown->ProfileY();

        TProfile *resProfile = new TProfile(std::to_string(Utils::get_random_int(0, 1000000)).c_str(),
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
};

#endif
