#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "FlowContainer.h"

using namespace std;

namespace util
{
    double calculate_ratio(double pid_charged_v4, double pid_charged_v2, double charged_v2, double charged_v4)
    {
        double cn4 = charged_v4 - 2 * charged_v2 * charged_v2;
        if (cn4 > 0)
        {
            return 0;
        }

        return (2 * pid_charged_v2 * charged_v2 - pid_charged_v4) / pow(-cn4, 0.75);
    }

    double calculate_var(double pid_charged_c22, double pid_charged_c24, double charged_c22, double charged_c24)
    {
        double v22pow4 = pow(pid_charged_c22, 4) / charged_c22 / charged_c22;
        double v24pow4 = pow(2 * pid_charged_c22 * charged_c22 - pid_charged_c24, 4) / pow(2 * charged_c22 * charged_c22 - charged_c24, 3);

        double res = v22pow4 - v24pow4;
        if (res < 0)
        {
            return 0;
        }
        return sqrt(res);
    }
} // namespace util

namespace util4fc
{
    TH1D *calculate_v2pt_corr(TProfile2D *h_prof_ch, TProfile2D *h_prof_pid, const char *name)
    {
        // get single profiles for charged
        // - ptave
        // - ptsquareave
        // - charged c22
        // - charged c24
        // - mean pt
        TProfile *h_ptave = h_prof_ch->ProfileX(Form("ptAve%s", name), 14, 14);
        TProfile *h_ptsquareave = h_prof_ch->ProfileX(Form("ptSquareAve%s", name), 13, 13);
        TProfile *h_charged_c22 = h_prof_ch->ProfileX(Form("c22%s", name), 1, 1);
        TProfile *h_charged_c24 = h_prof_ch->ProfileX(Form("c24%s", name), 3, 3);
        TProfile *h_meanpt = h_prof_ch->ProfileX(Form("meanpt%s", name), 15, 15);
        // end get single profiles for charged

        // get single profiles for pid
        // - cov v2pt
        // - pid charged c22
        // - pid charged c24
        // - c22 trackweighted
        TProfile *h_cov_v2pt = h_prof_pid->ProfileX(Form("covV2Pt%s", name), 11, 11);
        TProfile *h_pid_charged_c22 = h_prof_pid->ProfileX(Form("c22_ch_pid%s", name), 1, 1);
        TProfile *h_pid_charged_c24 = h_prof_pid->ProfileX(Form("c24_ch_pic%s", name), 3, 3);
        TProfile *h_c22_trackweighted = h_prof_pid->ProfileX(Form("c22_trackweighted%s", name), 6, 6);
        // end get single profiles for pid

        TH1D *output = new TH1D(name, name, h_charged_c22->GetNbinsX(), h_charged_c22->GetXaxis()->GetXbins()->GetArray());

        for (int i = 1; i <= h_charged_c22->GetNbinsX(); i++)
        {
            double cov_v2pt = h_cov_v2pt->GetBinContent(i);
            double ptAve = h_ptave->GetBinContent(i);
            double ptsquareAve = h_ptsquareave->GetBinContent(i);
            double charged_c22 = h_charged_c22->GetBinContent(i);
            double charged_c24 = h_charged_c24->GetBinContent(i);
            double meanpt = h_meanpt->GetBinContent(i);
            double pid_charged_c22 = h_pid_charged_c22->GetBinContent(i);
            double pid_charged_c24 = h_pid_charged_c24->GetBinContent(i);
            double c22_trackweighted = h_c22_trackweighted->GetBinContent(i);

            // check negative sqrt
            double varc22 = util::calculate_var(pid_charged_c22, pid_charged_c24, charged_c22, charged_c24);
            if (varc22 <= 0)
            {
                output->SetBinContent(i, 0);
                continue;
            }
            double varpt = ptsquareAve - 2 * ptAve * meanpt + meanpt * meanpt;
            if (varpt <= 0)
            {
                output->SetBinContent(i, 0);
                continue;
            }
            // end check negative sqrt

            // calculate v2ptcorre
            double v2ptcorre = (cov_v2pt - ptAve * c22_trackweighted) / sqrt(varpt) / varc22;
            output->SetBinContent(i, v2ptcorre);
            // end calculate v2ptcorre
        }
        // clean up
        delete h_ptave;
        delete h_ptsquareave;
        delete h_charged_c22;
        delete h_charged_c24;
        delete h_meanpt;
        delete h_cov_v2pt;
        delete h_pid_charged_c22;
        delete h_pid_charged_c24;
        delete h_c22_trackweighted;
        // end clean up

        return output;
    }
}

#define FILE "/home/huinaibing/Downloads/AnalysisResults_bootstrap_test.root"

void draw_pid_v2pt_correlation_main(const char *fc_name)
{
    // open file and get flow containers
    TFile *file = TFile::Open(FILE);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    FlowContainer *fc_pid = (FlowContainer *)dir_main->Get(fc_name);
    FlowContainer *fc_ch = (FlowContainer *)dir_main->Get("FlowContainerCharged");
    TObjArray *arr_ch = fc_ch->GetSubProfiles();
    TObjArray *arr_pid = fc_pid->GetSubProfiles();
    int nsub = arr_ch->GetEntriesFast();
    // end open file and get flow containers

    // get value and error
    TH1D *res = util4fc::calculate_v2pt_corr(fc_ch->GetProfile(), fc_pid->GetProfile(), Form("v2pt_correlation_%s", fc_name));
    std::vector<double> error4eachbin;
    for (int i = 0; i < res->GetNbinsX(); i++)
    {
        error4eachbin.push_back(0.0);
    }

    for (int i = 0; i < nsub; i++)
    {
        TProfile2D *h_sub_prof_ch = (TProfile2D *)arr_ch->At(i);
        TProfile2D *h_sub_prof_pid = (TProfile2D *)arr_pid->At(i);
        TH1D *h_sub = util4fc::calculate_v2pt_corr(h_sub_prof_ch, h_sub_prof_pid, std::to_string(i).c_str());
        for (int j = 1; j <= h_sub->GetNbinsX(); j++)
        {
            double diff_square = TMath::Power(h_sub->GetBinContent(j) - res->GetBinContent(j), 2);
            error4eachbin[j - 1] += diff_square;
        }
    }

    for (int i = 0; i < error4eachbin.size(); i ++)
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


void draw_pid_v2pt_correlation()
{
    draw_pid_v2pt_correlation_main("FlowContainerPi");
}