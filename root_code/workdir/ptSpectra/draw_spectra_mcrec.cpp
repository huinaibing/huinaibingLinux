#include "TFile.h"
#include "TH2D.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLegend.h"

void draw_spectra_mcrec()
{
    TFile *file = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");
    TH2D *hPtCent = file->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("correction")->Get<TH2D>("hPtCentMcRec");
    TH1D *hCentEventCountData = file->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH1D>("hCentEventCountMcRec");

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