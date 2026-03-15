#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"

// #define USE_2D_EFF
// #define USE_CORR

std::vector<TH1D *> draw_spectra_compare(const char *graph_name)
{
    TFile *File = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/right_all_global.root");

    TH2D *hPtCent = File->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH2D>(graph_name);
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

    // TCanvas *c = new TCanvas("c", "c", 1600, 1000);
    TLegend *legend = new TLegend(0.7, 0.7, 0.8, 0.9);
    Int_t colors[] = {kBlack, kRed, kBlue, kGreen + 2, kMagenta + 2, kCyan + 2, kOrange + 2, kSpring + 2, kTeal + 2, kAzure + 2};

    TH1D *hBase = hPtCent->ProjectionX("hPtCentBase", 1, 1);

    std::vector<TH1D *> res;
    for (int centBin = 2; centBin <= hPtCent->GetYaxis()->GetNbins(); centBin++)
    {
        TH1D *hPt = hPtCent->ProjectionX(Form("hPtCent%d", centBin), centBin, centBin);
        hPt->SetTitle("pt spectra ratio to 0-5%");
        hPt->GetXaxis()->SetTitle("p_{T} GeV/c");
        hPt->GetYaxis()->SetTitle("ratio");
        hPt->SetStats(0);
        // hPt->SetLineColor(colors[centBin - 1]);
        hPt->Divide(hBase);
        hPt->GetYaxis()->SetRangeUser(0, 1);
        hPt->GetXaxis()->SetRangeUser(0.2, 3);
        // hPt->Draw("SAME");
        res.push_back((TH1D *)hPt->Clone(Form("%shPtCent%d", graph_name, centBin)));
        // gPad->SetGridx();
        // gPad->SetGridy();
        // legend->AddEntry(hPt, Form("Cent %d-%d%%", (int)hPtCent->GetYaxis()->GetBinLowEdge(centBin), (int)hPtCent->GetYaxis()->GetBinUpEdge(centBin)), "l");
    }
    // legend->Draw("SAME");
    return res;
}

void draw_spectra_comapre()
{
    std::vector<TH1D *> pp = draw_spectra_compare("hPtCentDataPosEtaPosCh");
    std::vector<TH1D *> pn = draw_spectra_compare("hPtCentDataPosEtaNegCh");
    std::vector<TH1D *> np = draw_spectra_compare("hPtCentDataNegEtaPosCh");
    std::vector<TH1D *> nn = draw_spectra_compare("hPtCentDataNegEtaNegCh");
    TCanvas *c1 = new TCanvas("c1", "c1", 1600, 1000);
    for (const auto i : pp)
    {
        i->SetLineColor(kRed);
        i->Draw("SAME");
    }
    for (const auto i : pn)
    {
        i->SetLineColor(kBlack);
        i->Draw("SAME");
    }
    for (const auto i : np)
    {
        i->SetLineColor(kBlue);
        i->Draw("SAME");
    }
    for (const auto i : nn)
    {
        i->SetLineColor(kGreen + 2);
        i->Draw("SAME");
    }
}