#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>

void test_thn()
{
    int runlist[8] = {544095, 544098, 544116, 544121, 544122, 544123, 544124, 544091}; // for zzh
    TFile *file = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/small_littlecut_tr602740.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    THnSparseF *grh = (THnSparseF *)correction->Get("hRunNumberPhiEtaVertex");

    // TH1D *hPhi = new TH1D("hPhi", "hPhi", 60, 0, TMath::Pi() * 2);

    // int idxRun = 2; //544116

    // for (int binEta = 1; binEta <= 64; binEta++)
    // {
    //     for (int binVz = 1; binVz <= 40; binVz++)
    //     {
    //         for (int binPhi = 1; binPhi <= 60; binPhi++)
    //         {
    //             double weight = grh->GetBinContent(new int[4]{idxRun + 1, binPhi, binEta, binVz});
    //             hPhi->SetBinContent(binPhi, hPhi->GetBinContent(binPhi) + weight);
    //         }
    //     }
    // }

    // TCanvas *c1 = new TCanvas("c1", "", 800, 600);
    // hPhi->Draw();
    grh->GetAxis(0)->SetRange(3, 3);

    TH3D *hPhi = grh->Projection(1, 2, 3);
    TCanvas *c1 = new TCanvas("c1", "", 800, 600);
    hPhi->Draw();
}