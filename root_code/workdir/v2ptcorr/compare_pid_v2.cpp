/**
 * @file compare_pid_v2.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-12-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TLegend.h"

using namespace std;

void compare_pid_v2()
{
    TFile *file = TFile::Open("/home/huinaibing/Downloads/AnalysisResults_12_7_OO.root");

    TDirectory *dir = (TDirectory *)file->Get("pid-flow-pt-corr");
    TProfile *h_charged_v2 = (TProfile *)dir->Get("c22");

    TDirectory *dir_pi = (TDirectory *)dir->Get("pi");
    TProfile *h_pi_v2 = (TProfile *)dir_pi->Get("c22PiPi");
    TProfile *h_pi_charged_v2 = (TProfile *)dir_pi->Get("c22");

    TDirectory *dir_ka = (TDirectory *)dir->Get("ka");
    TProfile *h_ka_v2 = (TProfile *)dir_ka->Get("c22KaKa");
    TProfile *h_ka_charged_v2 = (TProfile *)dir_ka->Get("c22");

    TDirectory *dir_pr = (TDirectory *)dir->Get("pr");
    TProfile *h_pr_v2 = (TProfile *)dir_pr->Get("c22PrPr");
    TProfile *h_pr_charged_v2 = (TProfile *)dir_pr->Get("c22");

    TH1D *ratio_pi = new TH1D("ratio_pi", "ratio_pi", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *ratio_ka = new TH1D("ratio_ka", "ratio_ka", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *ratio_pr = new TH1D("ratio_pr", "ratio_pr", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});

    TH1D *pure_pi = new TH1D("pure_pi", "pure_pi", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *pure_ka = new TH1D("pure_ka", "pure_ka", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *pure_pr = new TH1D("pure_pr", "pure_pr", 10, new double[11]{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90});

    for (int i = 1; i <= h_charged_v2->GetNbinsX(); i++)
    {
        double charged_v2 = h_charged_v2->GetBinContent(i);

        double pi_v2 = h_pi_v2->GetBinContent(i);
        double pi_charged_v2 = h_pi_charged_v2->GetBinContent(i);

        double ka_v2 = h_ka_v2->GetBinContent(i);
        double ka_charged_v2 = h_ka_charged_v2->GetBinContent(i);

        double pr_v2 = h_pr_v2->GetBinContent(i);
        double pr_charged_v2 = h_pr_charged_v2->GetBinContent(i);

        if (charged_v2 < 0)
            continue;

        ratio_pi->SetBinContent(i, pi_charged_v2 / sqrt(charged_v2));
        ratio_ka->SetBinContent(i, ka_charged_v2 / sqrt(charged_v2));
        ratio_pr->SetBinContent(i, pr_charged_v2 / sqrt(charged_v2));

        pure_pi->SetBinContent(i, sqrt(pi_v2));
        pure_ka->SetBinContent(i, sqrt(ka_v2));
        pure_pr->SetBinContent(i, sqrt(pr_v2));
    }

    TFile *file_output = TFile::Open("compare_pid_v2__12_7_OO.root", "RECREATE");

    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    ratio_pi->SetStats(0);
    pure_pi->SetStats(0);
    ratio_pi->SetLineColor(kRed);
    ratio_pi->GetXaxis()->SetTitle("Centrality (%)");
    ratio_pi->GetYaxis()->SetTitle("v_{2}^{#pi} {2}");
    ratio_pi->SetTitle("");
    pure_pi->SetLineColor(kBlue);
    ratio_pi->Draw();
    pure_pi->Draw("SAME");
    TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->AddEntry(ratio_pi, "method 1 Pi", "l");
    leg->AddEntry(pure_pi, "method 2 Pi", "l");
    leg->Draw("SAME");

    TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
    ratio_ka->SetStats(0);
    pure_ka->SetStats(0);
    ratio_ka->SetLineColor(kRed);
    ratio_ka->GetXaxis()->SetTitle("Centrality (%)");
    ratio_ka->GetYaxis()->SetTitle("v_{2}^{K} {2}");
    ratio_ka->SetTitle("");
    pure_ka->SetLineColor(kBlue);
    ratio_ka->Draw();
    pure_pi->Draw("SAME");
    TLegend *leg2 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg2->AddEntry(ratio_ka, "method 1 Ka", "l");
    leg2->AddEntry(pure_ka, "method 2 Ka", "l");
    leg2->Draw("SAME");

    TCanvas *c3 = new TCanvas("c3", "c3", 1800, 1000);
    ratio_pr->SetStats(0);
    pure_pr->SetStats(0);
    ratio_pr->SetLineColor(kRed);
    ratio_pr->GetXaxis()->SetTitle("Centrality (%)");
    ratio_pr->GetYaxis()->SetTitle("v_{2}^{p} {2}");
    ratio_pr->SetTitle("");
    pure_pr->SetLineColor(kBlue);
    ratio_pr->Draw();
    pure_pr->Draw("SAME");
    TLegend *leg3 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg3->AddEntry(ratio_pr, "method 1 Pr", "l");
    leg3->AddEntry(pure_pr, "method 2 Pr", "l");
    leg3->Draw("SAME");

    c1->Write();
    c2->Write();
    c3->Write();
    file_output->Close();
}