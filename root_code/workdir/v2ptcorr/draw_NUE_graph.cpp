#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "THn.h"
#include "THnSparse.h"
#include "TLegend.h"
#include <iostream>

#define MC_PATH "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/lhc23zzhpass5mctr639743.root"

void draw_NUE_graph()
{
    // open file and dir
    TFile *file = TFile::Open(MC_PATH);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    // end open file and dir

    // open graph
    TH2D *hMcCentPt = (TH2D *)correction->Get("hPtCentMcGen");
    TH2D *hDataCentPt = (TH2D *)correction->Get("hPtCentMcRec");

    // divide
    TCanvas *c3 = new TCanvas("c3", "c3", 1800, 1000);
    hDataCentPt->Divide(hMcCentPt);
    hDataCentPt->SetStats(0);
    hDataCentPt->Draw();
    TFile *file_efficiency = new TFile("correctionsNUE/NUE2D_23zzhpass5_occ4000_tr639743.root", "RECREATE");
    hDataCentPt->SetName("ccdb_object");
    hDataCentPt->Write();
    file_efficiency->Close();
    return;

    // TFile *file_efficiency = new TFile("corrections/NUE.root", "RECREATE");

    // TH2D *frame = new TH2D("frame", "", 100, 0, 100, 100, 0, 1);
    // frame->GetXaxis()->SetTitle("p_{T}(GeV)");
    // frame->GetYaxis()->SetTitle("efficiency");
    // frame->Draw();
    // TH1D **tmp = new TH1D *[hMcCentPt->GetYaxis()->GetNbins()];
    // for (int i = 0; i < hMcCentPt->GetYaxis()->GetNbins(); i++)
    // {
    //     tmp[i] = new TH1D((std::to_string(int(hMcCentPt->GetYaxis()->GetBinCenter(i + 1))) + std::string("%")).c_str(),
    //                       "",
    //                       hMcCentPt->GetXaxis()->GetNbins(),
    //                       hMcCentPt->GetXaxis()->GetXbins()->GetArray());
    //     for (int j = 0; j < hMcCentPt->GetYaxis()->GetNbins(); j++)
    //     {
    //         tmp[i]->SetBinContent(j + 1, hDataCentPt->GetBinContent(i + 1, j + 1));
    //     }
    // }
    // TLegend *leg = new TLegend(0.2, 0.2, 0.3, 0.4);
    // for (int i = 0; i < hMcCentPt->GetXaxis()->GetNbins(); i++)
    // {
    //     tmp[i]->SetStats(0);
    //     tmp[i]->SetLineColor(kBlue + 3 * i);

    //     leg->AddEntry(tmp[i], Form("%2.4s%%", std::to_string(hMcCentPt->GetXaxis()->GetBinCenter(i + 1)).c_str()));

    // }
    // leg->Draw("same");
    // c3->Write();
    // file_efficiency->Close();
    // // end divide
}
