#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"

void draw_pidcheck_graph_main(const char *detector)
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *pid_perform = (TDirectory *)dir_main->Get("DetectorPidPerformace");
    TDirectory *two_detector_cmp = (TDirectory *)pid_perform->Get(detector);

    TH3D *h_pi = (TH3D *)two_detector_cmp->Get("Pi");
    TH3D *h_pr = (TH3D *)two_detector_cmp->Get("Pr");
    TH3D *h_ka = (TH3D *)two_detector_cmp->Get("Ka");

    for (int i = 1; i <= h_pi->GetZaxis()->GetNbins(); i++)
    {
       
    }
}

void draw_pidcheck_graph()
{
    draw_pidcheck_graph_main("TPCvsTOF");
}