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

void handle_pid()
{
    // open file and dir
    TFile *file = TFile::Open(DATA_PATH);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *dir_detector = (TDirectory *)dir_main->Get("DetectorPidPerformace");
    TDirectory *dir_TPC_TOF = (TDirectory *)dir_detector->Get("TPCvsTOF");

    TH3D *grp = (TH3D *)dir_TPC_TOF->Get("Pi");
    TCanvas *c1 = new TCanvas("c1", "", 1600, 1000);
    grp->GetXaxis()->SetRangeUser(-5, 5);
    grp->GetYaxis()->SetRangeUser(-5, 5);
    grp->GetZaxis()->SetRangeUser(0.2, 3);
    grp->Draw("same");
}