#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"

// #define USE_2D_EFF
// #define USE_CORR

void draw_spectra()
{
    TFile *File = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");

    TH2D *hPtCent = File->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH2D>("hPtCentData");
    TH1D *hCentEventCountData = File->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH1D>("hCentEventCountData");

#ifdef USE_CORR
#ifdef USE_2D_EFF
    TFile *Eff2D = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/corrections/NUE2D.root");
    TH2D *hEff2D = Eff2D->Get<TH2D>("ccdb_object");
    hPtCent->Divide(hEff2D);
#else
    TFile *Eff1D = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/corrections/Eff.root");
    TH1D *hEff1D = Eff1D->Get<TH1D>("ccdb_object");
    for (int centBin = 1; centBin <= hPtCent->GetYaxis()->GetNbins(); centBin++)
    {
        for (int ptBin = 1; ptBin <= hPtCent->GetXaxis()->GetNbins(); ptBin++)
        {
            hPtCent->SetBinContent(ptBin, centBin,
                                   hPtCent->GetBinContent(ptBin, centBin) / hEff1D->GetBinContent(ptBin));
        }
    }
#endif
#endif
    for (int centBin = 1; centBin <= hPtCent->GetYaxis()->GetNbins(); centBin++)
    {
        for (int ptBin = 1; ptBin <= hPtCent->GetXaxis()->GetNbins(); ptBin++)
        {
            hPtCent->SetBinContent(ptBin, centBin,
                                   hPtCent->GetBinContent(ptBin, centBin) / hCentEventCountData->GetBinContent(centBin) / hPtCent->GetXaxis()->GetBinWidth(ptBin));
        }
    }

    TCanvas *c = new TCanvas("c", "c", 1600, 1000);
    c->SetLogy();
    TLegend *legend = new TLegend(0.7, 0.7, 0.8, 0.9);
    Int_t colors[] = {kBlack, kRed, kBlue, kGreen + 2, kMagenta + 2, kCyan + 2, kOrange + 2, kSpring + 2, kTeal + 2, kAzure + 2};
    for (int centBin = 1; centBin <= hPtCent->GetYaxis()->GetNbins(); centBin++)
    {
        TH1D *hPt = hPtCent->ProjectionX(Form("hPtCent%d", centBin), centBin, centBin);
        hPt->SetTitle("pt spectra");
        hPt->GetXaxis()->SetTitle("p_{T} GeV/c");
        hPt->GetYaxis()->SetTitle("#frac{1}{N_{evt}} #frac{dN}{dp_{T}} (GeV/c)^{-1}");
        hPt->SetStats(0);
        hPt->SetLineColor(colors[centBin - 1]);
        hPt->Draw("E SAME");
        legend->AddEntry(hPt, Form("Cent %d-%d%%", (int)hPtCent->GetYaxis()->GetBinLowEdge(centBin), (int)hPtCent->GetYaxis()->GetBinUpEdge(centBin)), "l");
    }
    legend->Draw("SAME");
}