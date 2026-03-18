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
#include <iostream>

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


    }
};

#endif
