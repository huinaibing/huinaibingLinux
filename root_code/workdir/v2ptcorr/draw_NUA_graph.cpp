#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>

void draw_NUA_graph()
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    THnSparseF *grh = (THnSparseF *)correction->Get("hRunNumberPhiEtaVertex");

    TH1D *res = (TH1D *)grh->Projection(1);

    double max = 1;
    for (int i = 1; i <= res->GetNbinsX(); i++)
    {
        double tmp = res->GetBinContent(i);
        if (tmp > max)
        {
            max = tmp;
        }
    }
    std::cout << max;
    for (int i = 1; i <= res->GetNbinsX(); i++)
    {
        double tmp = res->GetBinContent(i);

        res->SetBinContent(i, max / tmp * tmp);
    }

    TCanvas *c1 = new TCanvas("c1", "", 1600, 1000);
    res->Draw();
}