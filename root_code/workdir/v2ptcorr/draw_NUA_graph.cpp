#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>
#include "GFWWeights.h"

void draw_NUA_graph()
{
    // get graph
    int runlist[8] = {544095, 544098, 544116, 544121, 544122, 544123, 544124, 544091}; // for zzh
    GFWWeights *fWeightsREF = nullptr;
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults_data_tr578329.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    THnSparseF *grh = (THnSparseF *)correction->Get("hRunNumberPhiEtaVertex");
    // end graph

    // loop all the runList
    for (int idxRun = 0; idxRun < 8; idxRun++)
    {
        // for each run, create a file
        TFile *output_file = 0;
        output_file = new TFile(Form("corrections/NUA_%d.root", runlist[idxRun]), "RECREATE", "");
        // end create file

        // init GFWWeights, althought I dont know why they init a pointer, i just copy it
        GFWWeights WeightsREF = GFWWeights("ccdb_object");
        fWeightsREF = &WeightsREF;
        fWeightsREF->init(true, false);
        // end init GFWWeights

        // loop the thnd NUA and fill the fWeight
        for (int binPhi = 1; binPhi <= 60; binPhi++)
        {
            for (int binEta = 1; binEta <= 40; binEta++)
            {
                for (int binVz = 1; binVz <= 40; binVz++)
                {
                    double weight = grh->GetBinContent(new int[4]{idxRun + 1, binPhi, binEta, binVz});
                    // double err = NUA->GetBinError(binPhi, binEta, binVz);
                    double phiuse = grh->GetAxis(1)->GetBinCenter(binPhi);
                    double etause = grh->GetAxis(2)->GetBinCenter(binEta);
                    double vtxzuse = grh->GetAxis(3)->GetBinCenter(binVz);
                    fWeightsREF->fill(phiuse, etause, vtxzuse, 1, 0, 0, weight);
                }
            }
        }
        // end loop the thnd NUA and fill the fWeight

        fWeightsREF->Write();
        output_file->Close();
    }
    // end loop all the runlist
}