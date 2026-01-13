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

#define DATA_PATH "/home/huinaibing/Documents/datas4o2/AnalysisResults_data_tr578329.root"
#define RUN2_PATH "/home/huinaibing/Downloads/run2flow/run2flow.root"

void draw_v24()
{
    TFile *file = TFile::Open(DATA_PATH);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TProfile *h_charged_c22 = (TProfile *)dir_main->Get("c22");
    TProfile *h_charged_c24 = (TProfile *)dir_main->Get("c24");

    TFile *file_run2 = TFile::Open(RUN2_PATH);

    TH2D *frame = new TH2D("1", "", 200, 0, 70, 100, 0, 0.05);
    TH1D *hv24 = new TH1D("run 3 v_{2}{4}", "", h_charged_c24->GetNbinsX(), h_charged_c24->GetXaxis()->GetXbins()->GetArray());
    TH1D *hv24_run2 = (TH1D*)((TDirectory*)file_run2->Get("Table 1"))->Get("Hist1D_y2");

    for (int i = 1; i <= h_charged_c24->GetNbinsX(); i++)
    {
        double c22 = h_charged_c22->GetBinContent(i);
        double c24 = h_charged_c24->GetBinContent(i);
        if (2 * c22 * c22 - c24 > 0)
        {
            hv24->SetBinContent(i, TMath::Power(2 * c22 * c22 - c24, 1 / 4.0));
            std::cout << "bin " << i << " : v24 = " << hv24->GetBinContent(i) << std::endl;
        }
        else
        {
            hv24->SetBinContent(i, 0);
        }
        hv24->SetBinError(i, 0.00001);
    }
    TCanvas *c1 = new TCanvas("c1", "", 1600, 1000);
    frame->SetStats(0);
    hv24->Draw("SAME");
    hv24_run2->Draw("SAME");
    hv24_run2->SetTitle("Run 2 v_{2}{4}");
    hv24_run2->SetLineColor(kRed);
    hv24->SetLineColor(kBlue);
    TLegend *legend = new TLegend(0.6, 0.7, 0.88, 0.88);
    legend->AddEntry(hv24, "Run 3 v_{2}{4}", "l");
    legend->AddEntry(hv24_run2, "Run 2 v_{2}{4}", "l");
    legend->Draw("SAME");
}