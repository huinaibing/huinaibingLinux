#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>

using namespace std;

void draw_NUA_graph()
{
    // get graph
    int runlist[8] = {544095, 544098, 544116, 544121, 544122, 544123, 544124, 544091}; // for zzh
    GFWWeights *fWeightsREF = nullptr;
    TFile *file = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/small_littlecut_tr602740.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    THnSparseD *grh = (THnSparseD *)correction->Get("hRunNumberPhiEtaVertex");
    // end graph

    // loop all the runList
    for (int idxRun = 0; idxRun < 8; idxRun++)
    {
        if (idxRun != 2 && idxRun != 4)
        {
            continue;
        }

        // for each run, create a file
        TFile *output_file = 0;
        std::cout << "Creating file for run " << runlist[idxRun] << std::endl;
        output_file = new TFile(Form("correction_littlecut/pass5zzh_NUA_%d.root", runlist[idxRun]), "RECREATE");
        // end create file

        // init GFWWeights, althought I dont know why they init a pointer, i just copy it
        GFWWeights WeightsREF = GFWWeights("ccdb_object");
        fWeightsREF = &WeightsREF;
        fWeightsREF->init(true, false);
        // end init GFWWeights

        // get the hist
        grh->GetAxis(0)->SetRange(idxRun + 1, idxRun + 1);
        TH3D *NUA = grh->Projection(1, 2, 3);
        // end get the hist

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

        // end loop the thnd NUA and fill the fWeight

        fWeightsREF->Write();
        output_file->Close();
    }
    // end loop all the runlist
}