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

double calculate_ratio(double pid_charged_v4, double pid_charged_v2, double charged_v2, double charged_v4)
{
    double cn4 = charged_v4 - 2 * charged_v2 * charged_v2;
    if (cn4 > 0)
    {
        return 0;
    }

    return (2 * pid_charged_v2 * charged_v2 - pid_charged_v4) / pow(-cn4, 0.75);
}

double calculate_pure(double pid_v4, double pid_v2)
{
    double cn4 = pid_v4 - 2 * pid_v2 * pid_v2;
    if (cn4 > 0)
    {
        return 0;
    }

    return pow(-cn4, 0.25);
}

void compare_pid_v4()
{
    TFile *file = TFile::Open("/home/huinaibing/Downloads/AnalysisResults_12_7_OO.root");

    TDirectory *dir = (TDirectory *)file->Get("pid-flow-pt-corr");
    TProfile *h_charged_v2 = (TProfile *)dir->Get("c22");
    TProfile *h_charged_v4 = (TProfile *)dir->Get("c24");

    TDirectory *dir_pi = (TDirectory *)dir->Get("pi");
    TProfile *h_pi_v2 = (TProfile *)dir_pi->Get("c22PiPi");
    TProfile *h_pi_v4 = (TProfile *)dir_pi->Get("c24PiPi");
    TProfile *h_pi_charged_v2 = (TProfile *)dir_pi->Get("c22");
    TProfile *h_pi_charged_v4 = (TProfile *)dir_pi->Get("c24");

    TDirectory *dir_ka = (TDirectory *)dir->Get("ka");
    TProfile *h_ka_v2 = (TProfile *)dir_ka->Get("c22KaKa");
    TProfile *h_ka_v4 = (TProfile *)dir_ka->Get("c24KaKa");
    TProfile *h_ka_charged_v2 = (TProfile *)dir_ka->Get("c22");
    TProfile *h_ka_charged_v4 = (TProfile *)dir_ka->Get("c24");

    TDirectory *dir_pr = (TDirectory *)dir->Get("pr");
    TProfile *h_pr_v2 = (TProfile *)dir_pr->Get("c22PrPr");
    TProfile *h_pr_v4 = (TProfile *)dir_pr->Get("c24PrPr");
    TProfile *h_pr_charged_v2 = (TProfile *)dir_pr->Get("c22");
    TProfile *h_pr_charged_v4 = (TProfile *)dir_pr->Get("c24");

    TH1D *ratio_pi = new TH1D("ratio_pi", "ratio_pi", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *ratio_ka = new TH1D("ratio_ka", "ratio_ka", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *ratio_pr = new TH1D("ratio_pr", "ratio_pr", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});

    TH1D *pure_pi = new TH1D("pure_pi", "pure_pi", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *pure_ka = new TH1D("pure_ka", "pure_ka", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});
    TH1D *pure_pr = new TH1D("pure_pr", "pure_pr", 11, new double[12]{0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90});

    for (int i = 1; i <= h_charged_v2->GetNbinsX(); i++)
    {
        ratio_pi->SetBinContent(i, calculate_ratio(
                                       h_pi_charged_v4->GetBinContent(i),
                                       h_pi_charged_v2->GetBinContent(i),
                                       h_charged_v2->GetBinContent(i),
                                       h_charged_v4->GetBinContent(i)));
        ratio_ka->SetBinContent(i, calculate_ratio(
                                       h_ka_charged_v4->GetBinContent(i),
                                       h_ka_charged_v2->GetBinContent(i),
                                       h_charged_v2->GetBinContent(i),
                                       h_charged_v4->GetBinContent(i)));
        ratio_pr->SetBinContent(i, calculate_ratio(
                                       h_pr_charged_v4->GetBinContent(i),
                                       h_pr_charged_v2->GetBinContent(i),
                                       h_charged_v2->GetBinContent(i),
                                       h_charged_v4->GetBinContent(i)));

        pure_pi->SetBinContent(i, calculate_pure(
                                      h_pi_v4->GetBinContent(i),
                                      h_pi_v2->GetBinContent(i)));
        pure_ka->SetBinContent(i, calculate_pure(
                                      h_ka_v4->GetBinContent(i),
                                      h_ka_v2->GetBinContent(i)));
        pure_pr->SetBinContent(i, calculate_pure(
                                      h_pr_v4->GetBinContent(i),
                                      h_pr_v2->GetBinContent(i)));
    }

    TFile *file_output = TFile::Open("compare_pid_v4_12_7_OO.root", "RECREATE");

    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    ratio_pi->SetStats(0);
    pure_pi->SetStats(0);
    ratio_pi->SetLineColor(kRed);
    ratio_pi->GetXaxis()->SetTitle("Centrality (%)");
    ratio_pi->GetYaxis()->SetTitle("v_{2}^{#pi} {4}");
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
    ratio_ka->GetYaxis()->SetTitle("v_{2}^{K} {4}");
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
    ratio_pr->GetYaxis()->SetTitle("v_{2}^{p} {4}");
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