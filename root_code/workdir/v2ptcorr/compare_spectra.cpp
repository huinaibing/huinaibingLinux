#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TLegend.h"

#define DATA_PATH "/home/huinaibing/Documents/datas4o2/AnalysisResults_data_tr578329.root"

void compare_spectra()
{
    TFile *file = TFile::Open(DATA_PATH);
    TDirectory *dir = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *dir_pi = (TDirectory *)dir->Get("pi");
    TH2D* h_cent_meanpt = (TH2D *)dir_pi->Get("c22dmeanpt");

    TH1D *h_pt = new TH1D("h_pt", "", 100, 0, 3);

    for (int i = 1; i <= 1; i++)
    {
        for (int j = 1; j <= h_cent_meanpt->GetNbinsY(); j++)
        {
            double content = h_cent_meanpt->GetBinContent(i, j);
            double pt = h_cent_meanpt->GetYaxis()->GetBinCenter(j);
            h_pt->Fill(pt, content);
        }
    }
    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    h_pt->Draw();
}