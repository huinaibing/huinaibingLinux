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
void draw_corrolation_v2pt()
{
    TFile *file = TFile::Open("/home/huinaibing/Downloads/AnalysisResults12_4.root");
    TDirectory *dir = (TDirectory *)file->Get("pid-flow-pt-corr");

    TProfile *h_cov_v2pt_diffpt = (TProfile *)dir->Get("covV2Pt_diffpt"); // 对应代码块1
    TProfile *h_ptAve = (TProfile *)dir->Get("ptAve");
    TProfile *h_ptSquareAve = (TProfile *)dir->Get("ptSquareAve");
    TProfile *h_c22 = (TProfile *)dir->Get("c22");
    TProfile *h_c24 = (TProfile *)dir->Get("c24");

    TProfile *h_cov_v2pt = (TProfile *)dir->Get("covV2Pt"); // 对应代码块2

    /**
     * @brief 代码块1,直接是用cov(v2,pt) / (sqrt(var(pt)) * sqrt(var(v2)))计算rho(v2,pt)
     *
     */
    {
        // TH1D *tmp = new TH1D("v2ptcorre", "v2ptcorre", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
        // for (int i = 1; i <= h_cov_v2pt_diffpt->GetNbinsX(); ++i)
        // {
        //     double cov = h_cov_v2pt_diffpt->GetBinContent(i);
        //     double sigma_cov = h_cov_v2pt_diffpt->GetBinError(i);

        //     double ptAve = h_ptAve->GetBinContent(i);
        //     double sigma_ptAve = h_ptAve->GetBinError(i);

        //     double ptSquareAve = h_ptSquareAve->GetBinContent(i);
        //     double sigma_ptSqureAve = h_ptSquareAve->GetBinError(i);

        //     double c22 = h_c22->GetBinContent(i);
        //     double sigma_c22 = h_c22->GetBinContent(i);

        //     double c24 = h_c24->GetBinContent(i);
        //     double sigma_c24 = h_c24->GetBinContent(i);

        //     double v2ptcorre = cov / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);

        //     double A = ptSquareAve - ptAve * ptAve;
        //     double B = c24 - c22 * c22;

        //     cout << cov << endl;

        //     if (A <= 0 || B <= 0)
        //     {
        //         tmp->SetBinContent(i, 0);
        //         tmp->SetBinError(i, 0);
        //         continue;
        //     }

        //     double sigma_A = sqrt(sigma_ptSqureAve * sigma_ptSqureAve + 4 * ptAve * ptAve * sigma_ptAve * sigma_ptAve);
        //     double sigma_B = sqrt(sigma_c24 * sigma_c24 + 4 * c22 * c22 * sigma_c22 * sigma_c22);

        //     double sigma_v2ptcorre = v2ptcorre * sqrt(sigma_cov * sigma_cov / cov / cov + sigma_A * sigma_A / 2 / A / 2 / A + sigma_B * sigma_B / 2 / 2 / B / B);

        //     tmp->SetBinContent(i, v2ptcorre);
        //     tmp->SetBinError(i, sigma_v2ptcorre);
        // }

        // TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
        // tmp->Draw();
        // tmp->GetXaxis()->SetTitle("cent %");
        // tmp->GetYaxis()->SetTitle("#rho(v2,pt)");
        // tmp->SetTitle("");
        // c1->SaveAs("v2ptcorre_from_diffpt_3.png");
    }

    /**
     * @brief 代码块2,使用<<v2pt>> - <pt> * <v2>去算，这个误差可能很大
     *
     */
    {
        TH1D* h_c22_trackweighted = (TH1D *)dir->Get("c22TrackWeight");
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
        file_output->Close();
    }

    /**
     * @brief 第三种方式，直接从图里面去提取
     *
     */
    {
        // TH1D *tmp = new TH1D("v2ptcorre_from_graph", "v2ptcorre", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});

        // TProfile2D *v2pt_graph = (TProfile2D *)dir->Get("c22dmeanpt");

        // double threshold = 999; // 相对误差阈值
        // for (int i = 1; i <= v2pt_graph->GetNbinsX(); ++i)
        // {
        //     // 首先，提取每个bin的内容（每个xbin的centrality不一样）
        //     // 注意到要筛选
        //     TH1D *h_onebin = new TH1D("onebin", "onebin", v2pt_graph->GetNbinsY(), 0, 3);
        //     for (int j = 1; j <= v2pt_graph->GetNbinsY(); ++j)
        //     {
        //         double bin_content = v2pt_graph->GetBinContent(i, j);
        //         double bin_error = v2pt_graph->GetBinError(i, j);
        //         if (bin_content == 0 || bin_error / fabs(bin_content) > threshold)
        //             continue;
        //         h_onebin->SetBinContent(j, bin_content);
        //         h_onebin->SetBinError(j, bin_error);
        //     }
        //     // 然后，对提取出来的每个bin，进行线性拟合，截距就是cov(v2,pt)
        //     TF1 *fit_func = new TF1("fit_func", [](double *x, double *p)
        //                             { return x[0] * p[0]; }, 0, 3, 1);

        //     h_onebin->Fit(fit_func, "RW");
        //     double slope = fit_func->GetParameter(0);
        //     double slope_error = fit_func->GetParError(0);
        //     cout << "slope: " << slope << " +- " << slope_error << endl;

        //     // 现在开始计算rho(v2,pt)
        //     double cov = slope;
        //     double sigma_cov = slope_error;

        //     double ptAve = h_ptAve->GetBinContent(i);
        //     double sigma_ptAve = h_ptAve->GetBinError(i);

        //     double ptSquareAve = h_ptSquareAve->GetBinContent(i);
        //     double sigma_ptSqureAve = h_ptSquareAve->GetBinError(i);

        //     double c22 = h_c22->GetBinContent(i);
        //     double sigma_c22 = h_c22->GetBinContent(i);

        //     double c24 = h_c24->GetBinContent(i);
        //     double sigma_c24 = h_c24->GetBinContent(i);

        //     double v2ptcorre = cov / sqrt(ptSquareAve - ptAve * ptAve) / sqrt(c24 - c22 * c22);

        //     double A = ptSquareAve - ptAve * ptAve;
        //     double B = c24 - c22 * c22;

        //     cout << cov << endl;

        //     if (A <= 0 || B <= 0)
        //     {
        //         tmp->SetBinContent(i, 0);
        //         tmp->SetBinError(i, 0);
        //         continue;
        //     }

        //     double sigma_A = sqrt(sigma_ptSqureAve * sigma_ptSqureAve + 4 * ptAve * ptAve * sigma_ptAve * sigma_ptAve);
        //     double sigma_B = sqrt(sigma_c24 * sigma_c24 + 4 * c22 * c22 * sigma_c22 * sigma_c22);

        //     double sigma_v2ptcorre = v2ptcorre * sqrt(sigma_cov * sigma_cov / cov / cov + sigma_A * sigma_A / 2 / A / 2 / A + sigma_B * sigma_B / 2 / 2 / B / B);

        //     tmp->SetBinContent(i, v2ptcorre);
        //     tmp->SetBinError(i, sigma_v2ptcorre);
        // }

        // TCanvas *c3 = new TCanvas("c3", "c3", 1800, 1000);
        // tmp->Draw();
        // tmp->GetXaxis()->SetTitle("cent %");
        // tmp->GetYaxis()->SetTitle("#rho(v2,pt)");
        // tmp->SetTitle("");
        // c3->SaveAs("v2ptcorre_from_graph.png");
    }
}
