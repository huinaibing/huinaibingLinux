#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#define POI "ka"

#define POI_C22 "c22KaKa"
#define POI_C24 "c24KaKa"

#define TITLE "v_{2} - p_{T} Correlation for K"

using namespace std;
void draw_pid_v2ptcorr()
{
    TFile *file = TFile::Open("/home/huinaibing/Downloads/AnalysisResults12_3.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *dir = (TDirectory *)dir_main->Get(POI);
    TProfile *h_cov_v2pt_diffpt = (TProfile *)dir->Get("covV2Pt_diffpt"); // 对应代码块1
    TProfile *h_ptAve = (TProfile *)dir_main->Get("ptAve");
    TProfile *h_ptSquareAve = (TProfile *)dir_main->Get("ptSquareAve");
    TProfile *h_c22 = (TProfile *)dir->Get(POI_C22);
    TProfile *h_c24 = (TProfile *)dir->Get(POI_C24);

    TProfile *h_cov_v2pt = (TProfile *)dir->Get("covV2Pt"); // 对应代码块2

    /**
     * @brief 代码块2,使用<<v2pt>> - <pt> * <v2>去算，
     *
     */
    {
        TH1D* h_c22_trackweighted = (TH1D *)dir->Get("c22TrackWeight");
        TH1D *tmp = new TH1D("v2ptcorre_mean", "v2ptcorre_mean", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
        
        for (int i = 1; i <= h_cov_v2pt->GetNbinsX(); ++i)
        {
            // calculate value
            double cov_v2pt = h_cov_v2pt->GetBinContent(i);
            double c22 = h_c22->GetBinContent(i);
            double ptAve = h_ptAve->GetBinContent(i);
            double c24 = h_c24->GetBinContent(i);
            double ptSquareAve = h_ptSquareAve->GetBinContent(i);

            double sigma_cov_v2pt = h_cov_v2pt->GetBinError(i);
            double sigma_c22 = h_c22->GetBinError(i);
            double sigma_ptAve = h_ptAve->GetBinError(i);
            double sigma_c24 = h_c24->GetBinError(i);
            double sigma_ptSquareAve = h_ptSquareAve->GetBinError(i);

            double c22_trackweighted = h_c22_trackweighted->GetBinContent(i);

            double v2ptcorre = (cov_v2pt - ptAve * c22_trackweighted) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);

            tmp->SetBinContent(i, v2ptcorre);
            // end calculate value

            // calculate error
            double partial_cov_v2pt = 1.0 / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            double partial_ptAve = (-c22) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22) + (cov_v2pt - ptAve * c22) * ptAve / (ptSquareAve - ptAve * ptAve) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            double partial_c22 = (-ptAve) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22) + (cov_v2pt - ptAve * c22) * c22 / (c24 - c22 * c22) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            double partial_ptSquareAve = -(cov_v2pt - ptAve * c22) / 2 / (ptSquareAve - ptAve * ptAve) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);
            double partial_c24 = -(cov_v2pt - ptAve * c22) / 2 / (c24 - c22 * c22) / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);

            double A = ptSquareAve - ptAve * ptAve;
            double B = c24 - c22 * c22;

            if (A <= 0 || B <= 0)
            {
                tmp->SetBinContent(i, 0);
                tmp->SetBinError(i, 0);
                continue;
            }

            double sigma_v2ptcorre = sqrt(
                partial_cov_v2pt * partial_cov_v2pt * sigma_cov_v2pt * sigma_cov_v2pt +
                partial_ptAve * partial_ptAve * sigma_ptAve * sigma_ptAve +
                partial_c22 * partial_c22 * sigma_c22 * sigma_c22 +
                partial_ptSquareAve * partial_ptSquareAve * sigma_ptSquareAve * sigma_ptSquareAve +
                partial_c24 * partial_c24 * sigma_c24 * sigma_c24);
            tmp->SetBinError(i, 0.00001);

            // end calculate error
        }
        TFile *file_output = TFile::Open("v2ptcorrelation_kaon.root", "RECREATE");
        TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
        tmp->SetStats(0);
        tmp->SetMarkerStyle(20);
        tmp->SetMarkerColor(kRed);
        tmp->SetLineColor(kRed);
        tmp->SetTitle(TITLE);
        tmp->Draw();
        tmp->GetXaxis()->SetTitle("cent %");
        tmp->GetYaxis()->SetTitle("#rho(v2,pt)");
        c2->SetFillColorAlpha(0, 0);
        
        
        c2->SaveAs("v2ptcorre_from_mean_pid.png");
        file_output->cd();
        tmp->Write();
        file_output->Close();
    }
}
