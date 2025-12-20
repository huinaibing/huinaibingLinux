#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "iostream"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "GFWWeights.h"

void vnsp()
{
  // TFile *myFile1 = TFile::Open("NUAREF.root");
  // if (!myFile1 || myFile1->IsZombie()){
  //   return;
  // }
  // GFWWeights* mAcceptance;
  // myFile1->ls();
  // myFile1->GetObject("ccdb_object", mAcceptance);
  // // mAcceptance->SetName("ccdb_object");
  // TFile *tmp_file = nullptr;
  // tmp_file = new TFile("NUAREF.root","RECREATE","inv. mass Graph");
  // mAcceptance->Write();
  // return;

  // TFile *input_file = TFile::Open("AnalysisResults.root");
  // if (!input_file || input_file->IsZombie()){
  //   return;
  // }

  int Nruns = 8;
  // int runlist[1] = {544013};                                                           // for zzf
  // int runlist[2] = {544028, 544032};                                                   // for zzg
  int runlist[8] = {544095, 544098, 544116, 544121, 544122, 544123, 544124, 544091}; // for zzh
  // int runlist[6] = {544184, 544185, 544389, 544390, 544391, 544392};                   // for zzi
  // int runlist[16] = {544451, 544454, 544474, 544475, 544476, 544477, 544490, 544491,
  //                    544492, 544508, 544510, 544511, 544512, 544514, 544515, 544518};  // for zzk
  // int runlist[12] = {544548, 544549, 544550, 544551, 544564, 544565, 544567, 544568,
  //                    544580, 544582, 544583, 544585};                                  // for zzl
  // int runlist[14] = {544614, 544640, 544674, 544754, 544767, 544652, 544653, 544672,
  //                    544692, 544693, 544694, 544696, 544739, 544742};                  // for zzm
  // int runlist[4] = {544794, 544795, 544797, 544813};                                   // for zzn
  // int runlist[29] = {544868, 544886, 544887, 544896, 544913, 544914, 544917, 544931,
  //                    544947, 544961, 544963, 544964, 544968, 544992, 545009, 545044,
  //                    545047, 545063, 545064, 545066, 545185, 545210, 545223, 545249,
  //                    545291, 545294, 545295, 545296, 545312};                          // for zzo

  // int Nruns = 92;
  // int runlist[92] = {
  //   544013,
  //   544028, 544032,
  //   544124, 544095, 544098, 544116, 544121, 544122, 544123, 544124,
  //   544184, 544185, 544389, 544390, 544391, 544392,
  //   544451, 544454, 544474, 544475, 544476, 544477, 544490, 544491,
  //   544492, 544508, 544510, 544511, 544512, 544514, 544515, 544518,
  //   544548, 544549, 544550, 544551, 544564, 544565, 544567, 544568,
  //   544580, 544582, 544583, 544585,
  //   544614, 544640, 544674, 544754, 544767, 544652, 544653, 544672,
  //   544692, 544693, 544694, 544696, 544739, 544742,
  //   544794, 544795, 544797, 544813,
  //   544868, 544886, 544887, 544896, 544913, 544914, 544917, 544931,
  //   544947, 544961, 544963, 544964, 544968, 544992, 545009, 545044,
  //   545047, 545063, 545064, 545066, 545185, 545210, 545223, 545249,
  //   545291, 545294, 545295, 545296, 545312
  // };
  TString Particle[3] = {"REF",
                         /* "K0s", "Lambda", "Xi", "Omega"*/};
  TString dataset[9] = {"251204"};

  // int Nruns = 1;
  // int runlist[7] = {544124};

  GFWWeights *fWeightsREF = nullptr;
  for (int iDataset = 1; iDataset <= 1; iDataset++)
  {
    TFile *input_file = TFile::Open(Form("AnalysisResults_%s.root", dataset[iDataset - 1].Data()));
    if (!input_file || input_file->IsZombie())
    {
      return;
    }
    for (int iSpecies = 1; iSpecies <= 1; iSpecies++)
    {
      for (int run = 1; run <= Nruns; run++)
      {

        TH3D *NUA = nullptr;
        if (iSpecies == 1)
          input_file->GetObject(Form("flow-gfw-omega-xi/%d/hPhiEtaVtxz;1", runlist[run - 1]), NUA);
        else
          input_file->GetObject(Form("flow-gfw-omega-xi/%d/hPhiEtaVtxz%s;1", runlist[run - 1], Particle[iSpecies - 1].Data()), NUA);

        if (NUA == nullptr)
        {
          delete NUA;
          continue;
        }

        TFile *output_file = nullptr;
        output_file = new TFile(Form("%s/NUA%s_%d.root", Particle[iSpecies - 1].Data(), Particle[iSpecies - 1].Data(), runlist[run - 1]), "RECREATE", "");
        GFWWeights WeightsREF = GFWWeights("ccdb_object");
        fWeightsREF = &WeightsREF;
        fWeightsREF->init(true, false);

        if (runlist[run - 1] == 544813)
        {
          cout << "check" << endl;
          for (int phibin = 1; phibin <= 60; phibin++)
          {
            for (int etabin = 1; etabin <= 64; etabin++)
            {
              for (int vtxzbin = 1; vtxzbin <= 40; vtxzbin++)
              {
                double weight = 100000;
                // double err = NUA->GetBinError(phibin, etabin, vtxzbin);
                double phiuse = NUA->GetXaxis()->GetBinCenter(phibin);
                double etause = NUA->GetYaxis()->GetBinCenter(etabin);
                double vtxzuse = NUA->GetZaxis()->GetBinCenter(vtxzbin);
                // for (int i = 0; i <= weight; i++)
                fWeightsREF->fill(phiuse, etause, vtxzuse, 1, 0, 0, weight);
              }
            }
          }
        }
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
        delete NUA;
        delete output_file;
        // delete fWeightsREF;
      }
    }
    input_file->Close();
    delete input_file;
  }

  // TFile *input_file = TFile::Open("1.root");
  // if (!input_file || input_file->IsZombie()){
  //   return;
  // }

  // TH1D *NUE = nullptr;
  // input_file->GetObject("ccdb_object", NUE);
  // for (int i = 0; i <= 1000; i++){
  //   NUE->SetBinContent(i,1);
  // }

  // NUE->Draw();
  // TFile *output_file = nullptr;
  // output_file = new TFile("NUE1.root","RECREATE","");
  // NUE->Write();

  return;
}