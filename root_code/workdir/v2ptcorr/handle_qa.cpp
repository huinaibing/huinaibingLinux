#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include "TLegend.h"
#include "TH2.h"
#include <iostream>

#define DATA_PATH "/home/huinaibing/Documents/datas4o2/AnalysisResults_data_tr578329.root"

void handle_qa()
{
    // open file and dir
    TFile *file = TFile::Open(DATA_PATH);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TH1D *hEventCount = (TH1D *)dir_main->Get("hEventCount");

    for (int i = 3; i <= 14; i++)
    {
        hEventCount->SetBinContent(i, hEventCount->GetBinContent(i) / 3);
    }
    TCanvas* c1 = new TCanvas("c1", "", 1500, 800);
    hEventCount->SetStats(0);
    hEventCount->Draw();
}