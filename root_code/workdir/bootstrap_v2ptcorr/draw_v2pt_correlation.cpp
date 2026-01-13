#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TLine.h"
#include "FlowContainer.h"

#define FILE "/home/huinaibing/Downloads/AnalysisResults_bootstrap_test.root"

// #define DEBUG

TH1D *calculate_v2pt_corr(TProfile2D *h_prof_total, const char *name)
{
    // get single profiles
    TProfile *h_c22 = h_prof_total->ProfileX(Form("c22%s", name), 1, 1);
    TProfile *h_c24 = h_prof_total->ProfileX(Form("c24%s", name), 3, 3);
    TProfile *h_covv2pt = h_prof_total->ProfileX(Form("cov_v2pt%s", name), 11, 11);
    TProfile *h_ptsquareave = h_prof_total->ProfileX(Form("ptSquareAve%s", name), 13, 13);
    TProfile *h_ptave = h_prof_total->ProfileX(Form("ptAve%s", name), 14, 14);
    TProfile *h_meanpt = h_prof_total->ProfileX(Form("meanpt%s", name), 15, 15);
    TProfile *h_c22_trackweighted = h_prof_total->ProfileX(Form("c22_trackweighted%s", name), 6, 6);
    // end get single profiles

    TH1D *output = new TH1D(name, name, h_c22->GetNbinsX(), h_c22->GetXaxis()->GetXbins()->GetArray());

    for (int i = 1; i <= h_c22->GetNbinsX(); i++)
    {
        // get value from each bin
        double cov_v2pt = h_covv2pt->GetBinContent(i);
        double c22 = h_c22->GetBinContent(i);
        double ptAve = h_ptave->GetBinContent(i);
        double c24 = h_c24->GetBinContent(i);
        double ptSquareAve = h_ptsquareave->GetBinContent(i);
        double c22_trackweighted = h_c22_trackweighted->GetBinContent(i);
        double meanpt = h_meanpt->GetBinContent(i);
        // end get value from each bin

        // check negative sqrt
        if (ptSquareAve - 2 * ptAve * meanpt + meanpt * meanpt <= 0)
        {
            output->SetBinContent(i, 0);
            continue;
        }
        if (c24 - c22 * c22 <= 0)
        {
            output->SetBinContent(i, 0);
            continue;
        }
        // end check negative sqrt

        // calculate v2ptcorre
        double v2ptcorre = (cov_v2pt - ptAve * c22_trackweighted) / sqrt(ptSquareAve - 2 * ptAve * meanpt + meanpt * meanpt) / sqrt(c24 - c22 * c22);
        output->SetBinContent(i, v2ptcorre);
        // end calculate v2ptcorre
    }

    // clean up
    delete h_c22;
    delete h_c24;
    delete h_covv2pt;
    delete h_ptsquareave;
    delete h_ptave;
    delete h_meanpt;
    delete h_c22_trackweighted;
    // end clean up

    return output;
}

void draw_v2pt_correlation()
{
    // init and get prof
    TFile *f = TFile::Open(FILE);
    TDirectory *dir = (TDirectory *)f->Get("pid-flow-pt-corr");
    FlowContainer *fc_ch = (FlowContainer *)dir->Get("FlowContainerCharged");
    TProfile2D *h_prof_ch = fc_ch->GetProfile();
    TObjArray *arr = fc_ch->GetSubProfiles();
    int nsub = arr->GetEntriesFast();
    std::cout << "Number of sub-profiles: " << nsub << std::endl;
    // end init and get prof

    // get value
    TH1D *res = calculate_v2pt_corr(h_prof_ch, "v2pt_correlation");
    // get error
    std::vector<double> error4eachbin;
    for (int i = 1; i <= res->GetNbinsX(); i++)
    {
        error4eachbin.push_back(0.0);
    }

    for (int i = 0; i < nsub; i++)
    {
        TProfile2D *h_sub_prof = (TProfile2D *)arr->At(i);
        TH1D *h_sub = calculate_v2pt_corr(h_sub_prof, std::to_string(i).c_str());
        for (int j = 1; j <= h_sub->GetNbinsX(); j++)
        {
            double diff_square = TMath::Power(h_sub->GetBinContent(j) - res->GetBinContent(j), 2);
            error4eachbin[j - 1] += diff_square;
        }
    }

    for (int i = 0; i < error4eachbin.size(); i++)
    {
        double stddev = TMath::Sqrt(error4eachbin[i] / (nsub - 1));
        res->SetBinError(i + 1, stddev);
    }
    // end get value and error

    // draw
    TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
    res->Draw();
    // end draw
}