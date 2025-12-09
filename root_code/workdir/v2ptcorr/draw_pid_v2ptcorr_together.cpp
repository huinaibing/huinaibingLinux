#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"

void draw_pid_v2ptcorr_together()
{
    TFile *file_pi = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/v2ptcorrelation_pi.root");
    TFile *file_ka = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/v2ptcorrelation_kaon.root");
    TFile *file_pr = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/v2ptcorrelation_proton.root");

    TH1D *h_pi = (TH1D *)file_pi->Get("v2ptcorre_mean");
    TH1D *h_ka = (TH1D *)file_ka->Get("v2ptcorre_mean");
    TH1D *h_pr = (TH1D *)file_pr->Get("v2ptcorre_mean");

    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    h_pr->SetLineColor(kRed);
    h_pr->SetMarkerColor(kRed);
    h_pr->SetMarkerStyle(20);
    h_pr->SetTitle("v_{2} - p_{T} Correlation for #pi, K, p");
    h_pr->GetXaxis()->SetTitle("centrality (%)");
    h_pr->GetYaxis()->SetTitle("#rho(v_{2}, p_{T})");
    h_pr->SetStats(0);
    h_pr->Draw("PE");
    h_ka->SetLineColor(kBlue);
    h_ka->SetMarkerColor(kBlue);
    h_ka->SetMarkerStyle(21);
    h_ka->Draw("PE same");
    h_pi->SetLineColor(kGreen + 2);
    h_pi->SetMarkerColor(kGreen + 2);
    h_pi->SetMarkerStyle(22);
    h_pi->Draw("PE same");
    TLegend *leg = new TLegend(0.3, 0.7, 0.5, 0.9);
    leg->AddEntry(h_pi, "Pion", "pe");
    leg->AddEntry(h_ka, "Kaon", "pe");
    leg->AddEntry(h_pr, "Proton", "pe");
    leg->Draw("SAME");
    c->SaveAs("v2ptcorrelation_together.png");
}