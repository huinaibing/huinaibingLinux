#ifndef UTILS4V2PTCORR_H
#define UTILS4V2PTCORR_H
#include "FlowContainer.h"
#include <functional>
#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include <vector>

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

class FlowContainerManager
{
public:
    FlowContainer *fc;
    FlowContainer *fc_pid;
    FlowContainerManager(FlowContainer *fc, FlowContainer *fc_pid) : fc(fc), fc_pid(fc_pid) {}

    TH1D *getHistFrom2FC(std::function<TH1D *(TProfile2D *, TProfile2D *)> getValFunc)
    {
        TH1D *res = getValFunc(this->fc->GetProfile(), this->fc_pid->GetProfile());

        TObjArray *sub_prof_arr = this->fc->GetSubProfiles();
        TObjArray *sub_prof_arr_pid = this->fc_pid->GetSubProfiles();
        int nsub = sub_prof_arr->GetEntriesFast();

        std::vector<double> error4eachbin;
        for (int i = 1; i <= res->GetNbinsX(); i++)
        {
            error4eachbin.push_back(0.0);
        }

        for (int i = 0; i < nsub; i++)
        {
            TProfile2D *sub_prof2d = (TProfile2D *)(sub_prof_arr->At(i));
            TProfile2D *sub_prof2d_pid = (TProfile2D *)(sub_prof_arr_pid->At(i));
            TH1D *sub_res = getValFunc(sub_prof2d, sub_prof2d_pid);
            for (int j = 1; j <= sub_res->GetNbinsX(); j++)
            {
                double diff = sub_res->GetBinContent(j) - res->GetBinContent(j);
                error4eachbin[j - 1] += diff * diff;
            }
        }

        for (int i = 0; i < error4eachbin.size(); i++)
        {
            double stddev = TMath::Sqrt(error4eachbin[i] / (nsub - 1));
            res->SetBinError(i + 1, stddev);
        }
        return res;
    }
};

#endif