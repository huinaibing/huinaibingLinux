#include "TFile.h"
#include <iostream>
#include "TH3D.h"
#include "TH2D.h"
#include "TCanvas.h"

#define DATA_PID "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_pid_new.root"

void nsigma_check()
{
    TFile *file = TFile::Open(DATA_PID);
    TDirectory *dir = file->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("DetectorPidPerformace");
    if (!dir)
    {
        std::cout << "dir no found" << std::endl;
        return;
    }

    TH3D *TPCvsTOFPi = dir->Get<TDirectory>("TPCvsITS")->Get<TH3D>("Ka");
    if (!TPCvsTOFPi)
    {
        std::cout << "no found" << std::endl;
        return;
    }
    TPCvsTOFPi->GetZaxis()->SetRangeUser(0.2, 0.6);

    // TPCvsTOFPi->GetXaxis()->SetRangeUser(-5, 5);
    // TPCvsTOFPi->GetYaxis()->SetRangeUser(-5, 5);

    TPCvsTOFPi->GetXaxis()->SetTitle("TPC");
    TPCvsTOFPi->GetYaxis()->SetTitle("ITS");

    TH2D *piProject = (TH2D *)TPCvsTOFPi->Project3D("yx");

    TCanvas *c1 = new TCanvas("c1", "c1", 1600, 1000);
    piProject->Draw();
}

void pid_check()
{
    nsigma_check();
}