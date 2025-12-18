#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"

using namespace std;

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

void draw_pid_v2ptcorr_main(const char *poi, const char *title, const char *output_name = "v2ptcorrelation_kaon.root")
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *dir = (TDirectory *)dir_main->Get(poi);

    // get things from charged particle (not include poi)
    TProfile *h_ptAve = (TProfile *)dir_main->Get("ptAve");
    TProfile *h_ptSquareAve = (TProfile *)dir_main->Get("ptSquareAve");
    TProfile *h_charged_c22 = (TProfile *)dir_main->Get("c22");
    TProfile *h_charged_c24 = (TProfile *)dir_main->Get("c24");
    // end get things from charged particle (not include poi)

    // get things from POI
    TProfile *h_cov_v2pt = (TProfile *)dir->Get("covV2Pt"); // 对应代码块2
    TProfile *h_pid_charged_c22 = (TProfile *)dir->Get("c22");
    TProfile *h_pid_charged_c24 = (TProfile *)dir->Get("c24");
    // end get things from poi

    /**
     * @brief 代码块2,使用<<v2pt>> - <pt> * <v2>去算，
     *
     */
    {
        TH1D *h_c22_trackweighted = (TH1D *)dir->Get("c22TrackWeight");
        TH1D *tmp = new TH1D("v2ptcorre_mean", "v2ptcorre_mean", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});

        for (int i = 1; i <= h_cov_v2pt->GetNbinsX(); ++i)
        {
            // calculate value
            double cov_v2pt = h_cov_v2pt->GetBinContent(i);
            double ptAve = h_ptAve->GetBinContent(i);
            double ptSquareAve = h_ptSquareAve->GetBinContent(i);
            double c22_trackweighted = h_c22_trackweighted->GetBinContent(i);

            double pid_charged_c22 = h_pid_charged_c22->GetBinContent(i);
            double pid_charged_c24 = h_pid_charged_c24->GetBinContent(i);

            double charged_c22 = h_charged_c22->GetBinContent(i);
            double charged_c24 = h_charged_c24->GetBinContent(i);

            double varc22 = calculate_var(pid_charged_c22, pid_charged_c24, charged_c22, charged_c24);
            if (varc22 <= 0)
                continue;

            double v2ptcorre = (cov_v2pt - ptAve * c22_trackweighted) / sqrt(ptSquareAve - ptAve * ptAve) / varc22;

            tmp->SetBinContent(i, v2ptcorre);
            // end calculate value

            // // calculate error
            // double partial_cov_v2pt = 1.0 / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            // double partial_ptAve = (-c22) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22) + (cov_v2pt - ptAve * c22) * ptAve / (ptSquareAve - ptAve * ptAve) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            // double partial_c22 = (-ptAve) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22) + (cov_v2pt - ptAve * c22) * c22 / (c24 - c22 * c22) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            // double partial_ptSquareAve = -(cov_v2pt - ptAve * c22) / 2 / (ptSquareAve - ptAve * ptAve) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            // double partial_c24 = -(cov_v2pt - ptAve * c22) / 2 / (c24 - c22 * c22) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);

            // double A = ptSquareAve - ptAve * ptAve;
            // double B = c24 - c22 * c22;

            // if (A <= 0 || B <= 0)
            // {
            //     tmp->SetBinContent(i, 0);
            //     tmp->SetBinError(i, 0);
            //     continue;
            // }

            // double sigma_v2ptcorre = sqrt(
            //     partial_cov_v2pt * partial_cov_v2pt * sigma_cov_v2pt * sigma_cov_v2pt +
            //     partial_ptAve * partial_ptAve * sigma_ptAve * sigma_ptAve +
            //     partial_c22 * partial_c22 * sigma_c22 * sigma_c22 +
            //     partial_ptSquareAve * partial_ptSquareAve * sigma_ptSquareAve * sigma_ptSquareAve +
            //     partial_c24 * partial_c24 * sigma_c24 * sigma_c24);
            tmp->SetBinError(i, 0.00001);

            // end calculate error
        }
        TFile *file_output = TFile::Open(output_name, "RECREATE");
        TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
        tmp->SetStats(0);
        tmp->SetMarkerStyle(20);
        tmp->SetMarkerColor(kRed);
        tmp->SetLineColor(kRed);
        tmp->SetTitle(title);
        tmp->Draw();
        tmp->GetXaxis()->SetTitle("cent %");
        tmp->GetYaxis()->SetTitle("#rho(v2,pt)");
        c2->SetFillColorAlpha(0, 0);

        // c2->SaveAs("v2ptcorre_from_mean_pid.png");
        file_output->cd();
        tmp->Write();
        file_output->Close();
    }
}

void draw_pid_v2ptcorr()
{
    draw_pid_v2ptcorr_main("ka", "v_{2} - p_{T} Correlation for K", "v2ptcorrelation_kaon.root");
    draw_pid_v2ptcorr_main("pi", "v_{2} - p_{T} Correlation for #pi", "v2ptcorrelation_pi.root");
    draw_pid_v2ptcorr_main("pr", "v_{2} - p_{T} Correlation for Proton", "v2ptcorrelation_proton.root");
}
