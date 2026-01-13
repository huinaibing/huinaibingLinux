#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>
#include "TProfile2D.h"

void test_bootstrap()
{
    TFile *f = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_bootstraptest.root");
    TDirectory *dir = (TDirectory *)f->Get("pid-flow-pt-corr");

    FlowContainer *fc = (FlowContainer *)dir->Get("FlowContainerPi");
    TProfile2D *h_prof_total = fc->GetProfile();
    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    h_prof_total->Draw("COLZ");

    TObjArray *arr = fc->GetSubProfiles();
    TProfile *h_single = (TProfile *)arr->At(0);
    TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
    h_single->Draw();
}