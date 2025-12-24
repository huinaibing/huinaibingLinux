#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>

void test_NUA_graph()
{
    // get graph
    int runlist[8] = {544095, 544098, 544116, 544121, 544122, 544123, 544124, 544091}; // for zzh
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults_data_tr578329.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    THnSparseF *grh = (THnSparseF *)correction->Get("hRunNumberPhiEtaVertex");
    // end graph

    TH1D *hPhi = new TH1D("hPhi", "", 60, 0, 2 * TMath::Pi());
    // loop all the runList
    for (int idxRun = 0; idxRun < 8; idxRun++)
    {
        if (idxRun != 2 && idxRun != 4)
            continue;
        // for each run, create a file

        // end create file

        // loop the thnd NUA and fill the fWeight
        for (int binPhi = 1; binPhi <= 60; binPhi++)
        {
            double sumation = 0;
            for (int binEta = 1; binEta <= 40; binEta++)
            {
                for (int binVz = 1; binVz <= 40; binVz++)
                {
                    double weight = grh->GetBinContent(new int[4]{idxRun + 1, binPhi, binEta, binVz});
                    sumation += weight;
                }
            }
            hPhi->SetBinContent(binPhi, hPhi->GetBinContent(binPhi) + sumation);
        }
        // end loop the thnd NUA and fill the fWeight
    }
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    hPhi->Draw();
    // end loop all the runlist
}