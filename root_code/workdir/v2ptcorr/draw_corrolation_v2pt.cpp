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

using namespace std;
void draw_corrolation_v2pt()
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults_data_small.root");
    TDirectory *dir = (TDirectory *)file->Get("pid-flow-pt-corr");

    TProfile *h_cov_v2pt_diffpt = (TProfile *)dir->Get("covV2Pt_diffpt"); // 对应代码块1
    TProfile *h_ptAve = (TProfile *)dir->Get("ptAve");
    TProfile *h_ptSquareAve = (TProfile *)dir->Get("ptSquareAve");
    TProfile *h_c22 = (TProfile *)dir->Get("c22");
    TProfile *h_c24 = (TProfile *)dir->Get("c24");

    TProfile *h_cov_v2pt = (TProfile *)dir->Get("covV2Pt"); // 对应代码块2

    /**
     * @brief 代码块2,使用<<v2pt>> - <pt> * <v2>去算，这个误差可能很大
     *
     */
    {
        TH1D *h_c22_trackweighted = (TH1D *)dir->Get("c22TrackWeight");
        TH1D *tmp = new TH1D("v2ptcorre_mean", "v2ptcorre_mean", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
        TFile *data_run2 = TFile::Open("/home/huinaibing/Downloads/run2v2ptcorr.root");
        TDirectory *dir_run2 = (TDirectory *)data_run2->Get("Table 1");
        TGraphErrors *v2pt_run2 = (TGraphErrors *)dir_run2->Get("Graph1D_y1");
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
        TFile *file_output = TFile::Open("v2ptcorrelation.root", "RECREATE");
        TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
        tmp->SetStats(0);
        tmp->SetMarkerStyle(20);
        tmp->SetMarkerColor(kRed);
        tmp->SetLineColor(kRed);
        tmp->SetTitle("");
        tmp->Draw();
        tmp->GetXaxis()->SetTitle("cent %");
        tmp->GetYaxis()->SetTitle("#rho(v2,pt)");
        c2->SetFillColorAlpha(0, 0);
        v2pt_run2->Draw("P SAME");
        TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
        leg->AddEntry(tmp, "Pb-Pb #sqrt{s_{NN}} = 5.36TeV");
        leg->AddEntry(v2pt_run2, "Pb-Pb #sqrt{s_{NN}} = 5.02TeV");
        leg->Draw("SAME");
        c2->SaveAs("v2ptcorre_from_mean.png");
        file_output->cd();
        tmp->Write();
        c2->Write();

        TCanvas *c3 = new TCanvas("c3", "c3", 1800, 1000);
        TH1D *ratio = new TH1D("ratio", "", 12, 0, 60);
        ratio->SetBinContent(1, v2pt_run2->GetPointY(0) / tmp->GetBinContent(1));
        ratio->SetBinContent(2, v2pt_run2->GetPointY(1) / tmp->GetBinContent(2));
        ratio->SetBinContent(3, v2pt_run2->GetPointY(2) / tmp->GetBinContent(3));
        ratio->SetBinContent(4, v2pt_run2->GetPointY(3) / tmp->GetBinContent(4));
        ratio->SetBinContent(5, v2pt_run2->GetPointY(4) / tmp->GetBinContent(5));
        ratio->SetBinContent(6, v2pt_run2->GetPointY(5) / tmp->GetBinContent(5));
        ratio->SetBinContent(7, v2pt_run2->GetPointY(6) / tmp->GetBinContent(6));
        ratio->SetBinContent(8, v2pt_run2->GetPointY(7) / tmp->GetBinContent(6));
        ratio->SetBinContent(9, v2pt_run2->GetPointY(8) / tmp->GetBinContent(7));
        ratio->SetBinContent(10, v2pt_run2->GetPointY(9) / tmp->GetBinContent(7));
        ratio->SetBinContent(11, v2pt_run2->GetPointY(10) / tmp->GetBinContent(8));
        ratio->SetBinContent(12, v2pt_run2->GetPointY(11) / tmp->GetBinContent(8));

        TLine *line = new TLine(0, 1.0, 60, 1.0);

        TH2D *frame1 = new TH2D("2", "", 200, 0, 60, 100, 0.3, 1.8);
        frame1->SetStats(0);
        frame1->GetXaxis()->SetTitle("cent %");
        frame1->GetYaxis()->SetTitle("run2(#rho) / run3");
        frame1->Draw();
        ratio->Draw("same");

        // 3. 设置直线样式（可选，增强可见性）
        line->SetLineColor(kRed); // 红色
        line->SetLineWidth(2);    // 线宽2
        line->SetLineStyle(2);    // 虚线（1为实线，2为虚线等）
        line->Draw("same");
        c3->Write();
        file_output->Close();
    }
}
