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

void draw_flow()
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults_data_small.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TProfile *h_charged_c22 = (TProfile *)dir_main->Get("c22");
    TH2D *frame = new TH2D("1", "", 200, 0, 70, 100, 0, 0.15);

    TH1D *h_v2_cent = new TH1D("v2cent", "", h_charged_c22->GetNbinsX(), h_charged_c22->GetXaxis()->GetXbins()->GetArray());

    for (int i = 1; i <= h_charged_c22->GetNbinsX(); i++)
    {
        h_v2_cent->SetBinContent(i, sqrt(h_charged_c22->GetBinContent(i)));
        h_v2_cent->SetBinError(i, 0.00001);
    }

    TFile *file_run2 = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/run2v2cent.root");
    TDirectory *dir_run2 = (TDirectory *)file_run2->Get("Table 1");
    TH1D *h_run2 = (TH1D *)dir_run2->Get("Hist1D_y1");

    for (int i = 1; i <= h_run2->GetNbinsX(); i++)
    {
        h_run2->SetBinError(i, 0.00001);
    }

    TCanvas *c1 = new TCanvas("c1", "", 1600, 1000);
    h_v2_cent->SetLineColor(kRed);
    h_run2->SetLineColor(kBlue);
    TLegend *leg = new TLegend(0.3, 0.3, 0.5, 0.5);
    leg->AddEntry(h_v2_cent, "run3");
    leg->AddEntry(h_run2, "run2");
    frame->SetStats(0);
    frame->GetXaxis()->SetTitle("cent %");
    frame->GetYaxis()->SetTitle("v_{2}{2}");

    frame->Draw();
    h_v2_cent->Draw("p SAME");
    h_run2->Draw("p same");
    leg->Draw();

    TCanvas *c2 = new TCanvas("c2", "", 1600, 1000);
    TH1D *ratio = new TH1D("ratio", "", h_v2_cent->GetNbinsX(), h_v2_cent->GetXaxis()->GetXbins()->GetArray());
    ratio->SetBinContent(1, h_run2->GetBinContent(1) / h_v2_cent->GetBinContent(1));
    ratio->SetBinContent(2, h_run2->GetBinContent(2) / h_v2_cent->GetBinContent(2));
    ratio->SetBinContent(3, h_run2->GetBinContent(3) / h_v2_cent->GetBinContent(3));
    ratio->SetBinContent(4, h_run2->GetBinContent(3) / h_v2_cent->GetBinContent(4));
    ratio->SetBinContent(5, h_run2->GetBinContent(4) / h_v2_cent->GetBinContent(5));
    ratio->SetBinContent(6, h_run2->GetBinContent(5) / h_v2_cent->GetBinContent(6));
    ratio->SetBinContent(7, h_run2->GetBinContent(6) / h_v2_cent->GetBinContent(7));
    ratio->SetBinContent(8, h_run2->GetBinContent(7) / h_v2_cent->GetBinContent(8));
    ratio->SetBinContent(9, h_run2->GetBinContent(8) / h_v2_cent->GetBinContent(9));
    TH2D *frame1 = new TH2D("2", "", 200, 0, 70, 100, 0.5, 1.5);
    frame1->SetStats(0);
    frame1->GetXaxis()->SetTitle("cent %");
    frame1->GetYaxis()->SetTitle("run2(v_{2}{2}) / run3");
    TLine *line = new TLine(0, 1.0, 70, 1.0);

    // 3. 设置直线样式（可选，增强可见性）
    line->SetLineColor(kRed); // 红色
    line->SetLineWidth(2);    // 线宽2
    line->SetLineStyle(2);    // 虚线（1为实线，2为虚线等）
    frame1->Draw();
    ratio->Draw("same");
    line->Draw("same");
}