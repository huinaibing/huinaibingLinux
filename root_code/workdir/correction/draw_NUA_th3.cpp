#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>
#include "GFWWeights.h"

void draw_NUA_th3()
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    GFWWeights *fWeightsREF = nullptr;
    TDirectory *correction = (TDirectory *)dir_main->Get("run");
    TH3D *NUA = (TH3D *)correction->Get("hRunNumberPhiEtaVertex");

    TFile *output_file = 0;
    output_file = new TFile(Form("correction_test_th3/pass5zzh_NUA_%d.root", 544124), "RECREATE", "");

    GFWWeights WeightsREF = GFWWeights("ccdb_object");
    fWeightsREF = &WeightsREF;
    fWeightsREF->init(true, false);

    // loop the thnd NUA and fill the fWeight
    for (int phibin = 1; phibin <= 60; phibin++)
    {
        for (int etabin = 1; etabin <= 64; etabin++)
        {
            for (int vtxzbin = 1; vtxzbin <= 40; vtxzbin++)
            {
                double weight = NUA->GetBinContent(phibin, etabin, vtxzbin);
                // double err = NUA->GetBinError(phibin, etabin, vtxzbin);
                double phiuse = NUA->GetXaxis()->GetBinCenter(phibin);
                double etause = NUA->GetYaxis()->GetBinCenter(etabin);
                double vtxzuse = NUA->GetZaxis()->GetBinCenter(vtxzbin);
                // for (int i = 0; i <= weight; i++)
                fWeightsREF->fill(phiuse, etause, vtxzuse, 1, 0, 0, weight);
            }
        }
    }

    fWeightsREF->Write();
    output_file->Close();
}