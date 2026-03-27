#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"
#include <iostream>
#include <ostream>


#define FILE "/home/huinaibing/Documents/datas4o2/AnalysisResults.root"


double get_cov_part1(int centBin)
{
    TFile *f = TFile::Open(FILE);
    TDirectory *dir = (TDirectory *)f->Get("pid-flow-pt-corr");

    TProfile3D *proOrigin = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>("hCharged");
    TProfile3D *proOriginpt = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>("hChargedMeanpt");

    //proOrigin->GetYaxis()->SetRange(centBin, centBin);


    TProfile2D *protemp2d = proOrigin->Project3DProfile("xy");
    protemp2d->GetXaxis()->SetRange(centBin, centBin);
    auto proMerged = protemp2d->ProfileY();

    TProfile2D *protemp2dpt = proOriginpt->Project3DProfile("xy");
    protemp2dpt->GetXaxis()->SetRange(centBin, centBin);
    auto proMergedpt = protemp2dpt->ProfileY();

    // auto C1 = new TCanvas("c1", "", 800, 600);
    // proMerged->SetStats(0);
    // proMerged->GetXaxis()->SetTitle("mean pt(GeV)");
    // proMerged->GetYaxis()->SetTitle("C22");
    // proMerged->Draw();


    double cov = 0;
    double point = 0;

    for (int i = 1; i <= proMerged->GetNbinsX(); i++)
    {
        double weight = proMerged->GetBinEntries(i);

        if (weight == 0)
        {
            continue;
        }
        // std::cout << i << " " << weight << std::endl;
        double meanpt = proMergedpt->GetBinContent(i);
        double val = proMerged->GetBinContent(i);
        cov += meanpt * val * weight;
        point += weight;


        //std::cout << val << " " << weight << std::endl;
    }
    cov /= point;

    // // std::cout << "cov " << cov << " ptBar " << ptBar << " varbar " << valBar << std::endl;
    // auto C1 = new TCanvas("c1", "", 800, 600);
    // proMerged->Draw();

    return cov;
}


void new_method_v2pt()
{

    for (int i = 1; i <= 11; i++)
        std::cout << get_cov_part1(i) << std::endl;
}
