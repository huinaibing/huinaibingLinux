#include "TCanvas.h"
#include "TFile.h"
#include "TH2D.h"
#include "TLegend.h"

#define USE_2D_EFF
#define USE_CORR

void draw_spectra()
{
    TFile *File = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");

    TH2D *hPtCent =
        File->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH2D>("hPtCentData4ITSOnly");
    TH1D *hCentEventCountData =
        File->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH1D>("hCentEventCountData");

#ifdef USE_CORR
#ifdef USE_2D_EFF
    TFile *Eff2D = TFile::Open(
        "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/correctionsNUE/NUE2D4ITS.root");
    TH2D *hEff2D = Eff2D->Get<TH2D>("ccdb_object");
    hPtCent->Divide(hEff2D);
#else
    TFile *Eff1D =
        TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/corrections/Eff.root");
    TH1D *hEff1D = Eff1D->Get<TH1D>("ccdb_object");
    for (int centBin = 1; centBin <= hPtCent->GetYaxis()->GetNbins(); centBin++)
    {
        for (int ptBin = 1; ptBin <= hPtCent->GetXaxis()->GetNbins(); ptBin++)
        {
            hPtCent->SetBinContent(ptBin,
                                   centBin,
                                   hPtCent->GetBinContent(ptBin, centBin) / hEff1D->GetBinContent(ptBin));
        }
    }
#endif
#endif
    for (int centBin = 1; centBin <= hPtCent->GetYaxis()->GetNbins(); centBin++)
    {
        for (int ptBin = 1; ptBin <= hPtCent->GetXaxis()->GetNbins(); ptBin++)
        {
            hPtCent->SetBinContent(ptBin,
                                   centBin,
                                   hPtCent->GetBinContent(ptBin, centBin) /
                                       hCentEventCountData->GetBinContent(centBin) /
                                       hPtCent->GetXaxis()->GetBinWidth(ptBin) / 1.6);
        }
    }

    TCanvas *c = new TCanvas("c", "c", 1600, 1000);
    c->SetLogy();
    TLegend *legend = new TLegend(0.7, 0.7, 0.8, 0.9);
    Int_t colors[] =
        {kBlack, kRed, kBlue, kGreen + 2, kMagenta + 2, kCyan + 2, kOrange + 2, kSpring + 2, kTeal + 2, kAzure + 2};
    for (int centBin = 1; centBin <= hPtCent->GetYaxis()->GetNbins() - 1; centBin++)
    {
        // rebin 3 && 4
        if (centBin == 4)
            continue;

        if (centBin != 3)
        {
            TH1D *hPt = hPtCent->ProjectionX(Form("hPtCent%d", centBin), centBin, centBin);
            hPt->SetTitle("pt spectra");
            hPt->GetXaxis()->SetTitle("p_{T} GeV/c");
            hPt->GetYaxis()->SetTitle("#frac{1}{N_{evt}} #frac{d^{2}N}{dp_{T}d#eta} (GeV/c)^{-1}");
            hPt->SetStats(0);
            hPt->SetLineColor(kRed);
            hPt->Draw("E SAME");
            legend->AddEntry(hPt,
                             Form("Cent %d-%d%%",
                                  (int)hPtCent->GetYaxis()->GetBinLowEdge(centBin),
                                  (int)hPtCent->GetYaxis()->GetBinUpEdge(centBin)),
                             "l");
        }
        else
        {
            TH1D *hPt3 = hPtCent->ProjectionX(Form("hPtCent%d", 3), 3, 3);
            TH1D *hPt4 = hPtCent->ProjectionX(Form("hPtCent%d", 4), 4, 4);

            // 还原
            for (int ptBin = 1; ptBin <= hPtCent->GetXaxis()->GetNbins(); ptBin++)
            {
                hPt3->SetBinContent(ptBin,
                                    hPt3->GetBinContent(ptBin) * 1.6 * hPtCent->GetXaxis()->GetBinWidth(ptBin) *
                                        hCentEventCountData->GetBinContent(centBin));
                hPt4->SetBinContent(ptBin,
                                    hPt4->GetBinContent(ptBin) * 1.6 * hPtCent->GetXaxis()->GetBinWidth(ptBin) *
                                        hCentEventCountData->GetBinContent(centBin));
            }

            // 相加
            hPt3->Add(hPt4);

            for (int ptBin = 1; ptBin <= hPtCent->GetXaxis()->GetNbins(); ptBin++)
            {
                hPt3->SetBinContent(
                    ptBin,
                    hPt3->GetBinContent(ptBin) /
                        (hCentEventCountData->GetBinContent(3) + hCentEventCountData->GetBinContent(4)) /
                        hPtCent->GetXaxis()->GetBinWidth(ptBin) / 1.6);
            }
            hPt3->SetStats(0);
            hPt3->SetLineColor(kRed);
            hPt3->Draw("E SAME");
        }
    }

    // ==========================================================================================
    TFile *inputDataFile = TFile::Open("/home/huinaibing/Documents/datas4o2/AnalysisResults.root");

    TH2D *hPtVsCentData =
        inputDataFile->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("ptSpectra")->Get<TH2D>("hPtCentData");
    TH1D *hCentEvtCount = inputDataFile->Get<TDirectory>("pid-flow-pt-corr")
                              ->Get<TDirectory>("ptSpectra")
                              ->Get<TH1D>("hCentEventCountData");

#ifdef USE_CORR
#ifdef USE_2D_EFF
    TFile *effCorr2DFile = TFile::Open(
        "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/correctionsNUE/NUE2D4globalTrack.root");
    TH2D *hEffCorr2D = effCorr2DFile->Get<TH2D>("ccdb_object");
    hPtVsCentData->Divide(hEffCorr2D);
#else
    TFile *effCorr1DFile =
        TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/corrections/Eff.root");
    TH1D *hEffCorr1D = effCorr1DFile->Get<TH1D>("ccdb_object");
    for (int centBinIdx = 1; centBinIdx <= hPtVsCentData->GetYaxis()->GetNbins(); centBinIdx++)
    {
        for (int ptBinIdx = 1; ptBinIdx <= hPtVsCentData->GetXaxis()->GetNbins(); ptBinIdx++)
        {
            hPtVsCentData->SetBinContent(ptBinIdx,
                                         centBinIdx,
                                         hPtVsCentData->GetBinContent(ptBinIdx, centBinIdx) /
                                             hEffCorr1D->GetBinContent(ptBinIdx));
        }
    }
#endif
#endif
    for (int centBinIdx = 1; centBinIdx <= hPtVsCentData->GetYaxis()->GetNbins(); centBinIdx++)
    {
        for (int ptBinIdx = 1; ptBinIdx <= hPtVsCentData->GetXaxis()->GetNbins(); ptBinIdx++)
        {
            hPtVsCentData->SetBinContent(ptBinIdx,
                                         centBinIdx,
                                         hPtVsCentData->GetBinContent(ptBinIdx, centBinIdx) /
                                             hCentEvtCount->GetBinContent(centBinIdx) /
                                             hPtVsCentData->GetXaxis()->GetBinWidth(ptBinIdx) / 1.6);
        }
    }


    TLegend *centLegend = new TLegend(0.7, 0.7, 0.8, 0.9);
    Int_t colorPalette[] =
        {kBlack, kRed, kBlue, kGreen + 2, kMagenta + 2, kCyan + 2, kOrange + 2, kSpring + 2, kTeal + 2, kAzure + 2};
    for (int centBinIdx = 1; centBinIdx <= hPtVsCentData->GetYaxis()->GetNbins() - 1; centBinIdx++)
    {
        // rebin 3 && 4
        if (centBinIdx == 4)
            continue;

        if (centBinIdx != 3)
        {
            TH1D *hProjPtCent = hPtVsCentData->ProjectionX(Form("hProjPtCent%d", centBinIdx), centBinIdx, centBinIdx);
            hProjPtCent->SetTitle("pt spectra");
            hProjPtCent->GetXaxis()->SetTitle("p_{T} GeV/c");
            hProjPtCent->GetYaxis()->SetTitle("#frac{1}{N_{evt}} #frac{d^{2}N}{dp_{T}d#eta} (GeV/c)^{-1}");
            hProjPtCent->SetStats(0);
            hProjPtCent->SetLineColor(kBlue);
            hProjPtCent->Draw("E SAME");
            centLegend->AddEntry(hProjPtCent,
                                 Form("Cent %d-%d%%",
                                      (int)hPtVsCentData->GetYaxis()->GetBinLowEdge(centBinIdx),
                                      (int)hPtVsCentData->GetYaxis()->GetBinUpEdge(centBinIdx)),
                                 "l");
        }
        else
        {
            TH1D *hProjPtCent3 = hPtVsCentData->ProjectionX(Form("hProjPtCent%d", 3), 3, 3);
            TH1D *hProjPtCent4 = hPtVsCentData->ProjectionX(Form("hProjPtCent%d", 4), 4, 4);

            // 还原
            for (int ptBinIdx = 1; ptBinIdx <= hPtVsCentData->GetXaxis()->GetNbins(); ptBinIdx++)
            {
                hProjPtCent3->SetBinContent(ptBinIdx,
                                            hProjPtCent3->GetBinContent(ptBinIdx) * 1.6 *
                                                hPtVsCentData->GetXaxis()->GetBinWidth(ptBinIdx) *
                                                hCentEvtCount->GetBinContent(centBinIdx));
                hProjPtCent4->SetBinContent(ptBinIdx,
                                            hProjPtCent4->GetBinContent(ptBinIdx) * 1.6 *
                                                hPtVsCentData->GetXaxis()->GetBinWidth(ptBinIdx) *
                                                hCentEvtCount->GetBinContent(centBinIdx));
            }

            // 相加
            hProjPtCent3->Add(hProjPtCent4);

            for (int ptBinIdx = 1; ptBinIdx <= hPtVsCentData->GetXaxis()->GetNbins(); ptBinIdx++)
            {
                hProjPtCent3->SetBinContent(ptBinIdx,
                                            hProjPtCent3->GetBinContent(ptBinIdx) /
                                                (hCentEvtCount->GetBinContent(3) + hCentEvtCount->GetBinContent(4)) /
                                                hPtVsCentData->GetXaxis()->GetBinWidth(ptBinIdx) / 1.6);
            }
            hProjPtCent3->SetStats(0);
            hProjPtCent3->SetLineColor(kBlue);
            hProjPtCent3->Draw("E SAME");
        }
    }


    // legend->Draw("SAME");

    // run2
    TFile *run2 = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/ptspectra_run2.root");

    for (int i = 1; i <= 9; i++)
    {
        TH1 *hPt_run2 = run2->Get<TDirectory>("Table 1")->Get<TH1>(Form("Hist1D_y%d", i));
        // TH1 *hPt_run2_err = run2->Get<TDirectory>("Table 1")->Get<TH1>(Form("Hist1D_y%d_e1", i));
        TH1D *temp =
            new TH1D(Form("temp%d", i), "", hPt_run2->GetNbinsX(), hPt_run2->GetXaxis()->GetXbins()->GetArray());

        for (int j = 1; j <= hPt_run2->GetNbinsX(); j++)
        {
            temp->SetBinContent(j, hPt_run2->GetBinContent(j));
        }
        temp->Draw("SAME");
        temp->SetLineColor(kGreen);
    }
}
