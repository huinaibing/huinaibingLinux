#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include "TLegend.h"
#include "TH2.h"
#include <iostream>

#define MC_PATH "/home/huinaibing/Documents/datas4o2/AnalysisResults_mc_tr578758.root"

void draw_NUE_graph()
{
    // open file and dir
    TFile *file = TFile::Open(MC_PATH);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    // end open file and dir

    // open graph
    TH2D *hMcCentPt = (TH2D *)correction->Get("hCentPtMC");
    TH2D *hDataCentPt = (TH2D *)correction->Get("hCentPtData");

    // end open graph

    // divide
    hDataCentPt->Divide(hMcCentPt);
    TFile *file_efficiency = new TFile("corrections/NUE.root", "RECREATE");
    TCanvas *c3 = new TCanvas("c3", "c3", 1800, 1000);
    TH1D **tmp = new TH1D *[hMcCentPt->GetXaxis()->GetNbins()];
    for (int i = 0; i < hMcCentPt->GetXaxis()->GetNbins(); i++)
    {
        tmp[i] = new TH1D((std::to_string(int(hMcCentPt->GetXaxis()->GetBinCenter(i + 1))) + std::string("%")).c_str(),
                          "",
                          hMcCentPt->GetYaxis()->GetNbins(),
                          hMcCentPt->GetYaxis()->GetXbins()->GetArray());
        for (int j = 0; j < hMcCentPt->GetYaxis()->GetNbins(); j++)
        {
            tmp[i]->SetBinContent(j + 1, hDataCentPt->GetBinContent(i + 1, j + 1));
        }
    }
    TLegend *leg = new TLegend(0.2, 0.2, 0.3, 0.4);
    for (int i = 0; i < hMcCentPt->GetXaxis()->GetNbins(); i++)
    {
        tmp[i]->SetStats(0);
        tmp[i]->SetLineColor(kBlue + 3 * i);
        tmp[i]->GetXaxis()->SetTitle("p_{T}(GeV)");
        tmp[i]->GetYaxis()->SetTitle("efficiency");
        leg->AddEntry(tmp[i], Form("%2.4s%%", std::to_string(hMcCentPt->GetXaxis()->GetBinCenter(i + 1)).c_str()));
        tmp[i]->Draw("same");
    }
    leg->Draw("same");
    c3->Write();
    file_efficiency->Close();
    // end divide
}