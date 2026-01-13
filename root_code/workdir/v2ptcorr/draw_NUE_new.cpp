#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include "TLegend.h"
#include "TH2.h"
#include <iostream>

#define MC_PATH "/home/huinaibing/Documents/datas4o2/AnalysisResults_mc_579410.root"

void draw_NUE_new()
{
    // get graph
    TFile *f = new TFile(MC_PATH);
    TDirectory *dir_main = (TDirectory *)f->Get("flow-pt-efficiency");
    TH1D *hRec = (TH1D *)dir_main->Get("hPtMCRec");
    TH1D *hGen = (TH1D *)dir_main->Get("hPtMCGen");
    // end get graph

    hRec->Divide(hGen);

    // save
    TFile *out = new TFile("corrections/Eff.root", "RECREATE");
    hRec->SetTitle("efficiency");
    hRec->SetName("ccdb_object");
    hRec->GetXaxis()->SetTitle("p_{T} GeV");
    hRec->GetYaxis()->SetTitle("efficiency");
    hRec->Write();
    out->Close();
    // end save
}