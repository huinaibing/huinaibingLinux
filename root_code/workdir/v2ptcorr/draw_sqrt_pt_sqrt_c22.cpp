#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"

using namespace std;

void draw_sqrt_pt_sqrt_c22()
{
    TFile *file = TFile::Open("/home/huinaibing/Downloads/AnalysisResults12_4.root");
    TFile *file_output = TFile::Open("sqrt_pt_c22.root", "RECREATE");

    TDirectory *dir = (TDirectory *)file->Get("pid-flow-pt-corr");

    TProfile *h_cov_v2pt_diffpt = (TProfile *)dir->Get("covV2Pt_diffpt"); // 对应代码块1
    TProfile *h_ptAve = (TProfile *)dir->Get("ptAve");
    TProfile *h_ptSquareAve = (TProfile *)dir->Get("ptSquareAve");
    TProfile *h_c22 = (TProfile *)dir->Get("c22");
    TProfile *h_c24 = (TProfile *)dir->Get("c24");

    TH1D *sqrt_varpt = new TH1D("sqrtvarpt", "sqrtvarpt", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *sqrt_varc22 = new TH1D("sqrtvarc22", "sqrtvarc22", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});

    for (int i = 1; i <= h_cov_v2pt_diffpt->GetNbinsX(); ++i)
    {
        double ptSquareAve = h_ptSquareAve->GetBinContent(i);
        double ptAve = h_ptAve->GetBinContent(i);
        double c22 = h_c22->GetBinContent(i);
        double c24 = h_c24->GetBinContent(i);

        if (ptSquareAve - ptAve * ptAve > 0)
        {
            sqrt_varpt->SetBinContent(i, sqrt(ptSquareAve - ptAve * ptAve));
        }

        if (c24 - c22 * c22 > 0)
        {
            sqrt_varc22->SetBinContent(i, sqrt(c24 - c22 * c22));
        }
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    sqrt_varpt->Draw();
    c1->Write();
    TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
    sqrt_varc22->Draw();
    c2->Write();
    file_output->Close();
}