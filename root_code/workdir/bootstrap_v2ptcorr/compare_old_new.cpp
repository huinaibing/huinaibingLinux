#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include <iostream>

void compare_old_new()
{
    TFile *file_old = new TFile("old_method.root");
    TFile *file_new = new TFile("new_method.root");
    TH1D *oldf = file_old->Get<TH1D>("v2pt_correlation");
    TH1D *newf = file_new->Get<TH1D>("v2pt_correlation");

    TCanvas *c1 = new TCanvas("c1", "old method compare to new method", 800, 600);
    TLegend *leg = new TLegend(0.6, 0.6, 0.8, 0.8);
    oldf->SetLineColor(kRed);
    newf->SetLineColor(kBlue);
    leg->AddEntry(oldf, "old method");
    leg->AddEntry(newf, "new method");

    oldf->SetStats(0);
    oldf->GetXaxis()->SetTitle("Centrality (%)");
    oldf->GetYaxis()->SetTitle("#rho(v_{2}^{2}, [p_{T}])");
    oldf->Draw();
    newf->Draw("SAME");
    leg->Draw("SAME");
}
