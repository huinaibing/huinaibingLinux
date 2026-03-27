#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"

#define PIONFILE "result/res_pion.root"
#define KAONFILE "result/res_kaon.root"
#define PROTONFILE "result/res_proton.root"

void res_merger()
{
    TFile *f_pi = TFile::Open(PIONFILE);
    TFile *f_ka = TFile::Open(KAONFILE);
    TFile *f_pr = TFile::Open(PROTONFILE);

    TH1D *h_pi = f_pi->Get<TH1D>("hrhoPID");
    TH1D *h_ka = f_ka->Get<TH1D>("hrhoPID");
    TH1D *h_pr = f_pr->Get<TH1D>("hrhoPID");

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
    TH2D *frame = new TH2D("frame", "", 60, 0, 60, 100, -1, 1);

    h_pi->SetStats(0);
    h_ka->SetStats(0);
    h_pr->SetStats(0);
    frame->SetStats(0);

    h_pi->SetLineColor(kRed);
    h_pi->SetLineWidth(2);
    h_pi->SetMarkerColor(kRed);
    h_pi->SetMarkerStyle(kFullCircle);

    h_ka->SetLineColor(kBlue);
    h_ka->SetLineWidth(2);
    h_ka->SetMarkerColor(kBlue);
    h_ka->SetMarkerStyle(kFullSquare);

    h_pr->SetLineColor(kGreen + 2);
    h_pr->SetLineWidth(2);
    h_pr->SetMarkerColor(kGreen + 2);
    h_pr->SetMarkerStyle(kFullTriangleUp);

    frame->Draw();
    h_pr->Draw("SAME P");
    h_pi->Draw("SAME P"); // "HIST"：仅画直方图线；若要数据点用 "EP"
    h_ka->Draw("SAME P"); // "SAME"：叠加在已有图上


    TLegend *leg = new TLegend(0.72, 0.72, 0.92, 0.92); // 坐标：(x1,y1,x2,y2)，NDC格式(0-1)
    leg->SetFillColor(kWhite);                          // 图例背景白色
    leg->SetBorderSize(1);                              // 图例边框
    leg->SetTextFont(42);
    leg->AddEntry(h_pi, "#pi^{#pm}", "l"); // "l"=线，"p"=点，"lp"=线+点
    leg->AddEntry(h_ka, "K^{#pm}", "l");
    leg->AddEntry(h_pr, "p/#bar{p}", "l");
    leg->Draw("SAME");
}
