#include "Plotter.h"

void CalculateEfficiency(Int_t part = 0, Bool_t rebinSwitch = kTRUE)
{
  TString name[] = {"Trigger", "K0Short", "Lambda", "AntiLambda", "XiMinus", "XiPlus", "OmegaMinus", "OmegaPlus", "Pion"};
  TString finalNames[] = {"h", "K_{S}^{0}", "#Lambda", "#bar{#Lambda}", "#Xi^{-}", "#Xi^{+}", "#Omega^{-}", "#Omega^{+}", "#pi^{+}+#pi^{-}"};
  TString dir = "/home/mima/下载/Macros/analysisresult";
  TString analysisfile = "/10.17pprefMC_k0s_500971";
  TString analysisFilePath = dir + analysisfile;
  TFile *fFile = new TFile(Form("%s/AnalysisResults.root", analysisFilePath.Data()));

  TH2F *genHist = (TH2F *)fFile->Get(Form("h-strange-correlation/GeneratedWithPV/h%s", name[part].Data()));
  genHist->Sumw2();
  if (part == 0)
    name[0] = "Track";

  TH3F *recoHist = (TH3F *)fFile->Get(Form("h-strange-correlation/h%sEtaVsPtVsPhi", name[part].Data()));
  recoHist->Sumw2();
  if (part == 0)
    recoHist->GetXaxis()->SetRange(21, 55);
  if (part == 0)
    name[0] = "Trigger";

  TH3F *recoHist_Bckg = nullptr;
  if (part > 0)
  {
    recoHist_Bckg = (TH3F *)fFile->Get(Form("h-strange-correlation/h%sEtaVsPtVsPhiBg", name[part].Data()));
    recoHist_Bckg->Sumw2();
  }

  TH2F *recoEtaPtProj;

  if (!rebinSwitch)
  {
    // 原始逻辑
    recoEtaPtProj = (TH2F *)recoHist->Project3D("yx");
    if (part > 0)
    {
      TH2F *recoEtaPtProj_Bckg = (TH2F *)recoHist_Bckg->Project3D("yx");
      recoEtaPtProj->Add(recoEtaPtProj_Bckg, -1);
    }
    if (part == 0)
    {
      Double_t ptAxis[] = {2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.4, 4.8, 5.2, 5.6, 6.0,
                           6.5, 7.0, 7.5, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 17.0,
                           19.0, 21.0, 23.0, 25.0, 30.0, 35.0, 40.0, 50.0}; // 35 bins
      TH2F *genHistClone = new TH2F("genHistClone", "", 35, ptAxis, 80, -0.8, 0.8);
      for (int i = 0; i < 35; i++)
      {
        for (int j = 0; j < 80; j++)
        {
          genHistClone->SetBinContent(i + 1, j + 1, genHist->GetBinContent(21 + i, j + 1));
        }
      }
      recoEtaPtProj->Divide(genHistClone);
    }
    else
    {
      recoEtaPtProj->Divide(genHist);
    }
  }
  else
  {
    // Rebin 模式（你需要的）
    // 定义 x 轴：非均匀合并，35 bins
    float xbins1[26] = {
        0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.6, 3.0, 3.6,
        4.4, 5.2, 6.0, 8.0, 10.0, 12.0, 15.0, 19.0, 25.0, 35.0, 50.0};

    float xbins2[36] = {
        2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.4, 4.8, 5.2, 5.6, 6.0, 6.5, 7.0,
        7.5, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 17.0, 19.0, 21.0, 23.0, 25.0, 30.0, 35.0, 40.0, 50.0};

    // y 轴均匀合并成 20 bins
    float ybins[21];
    for (int i = 0; i <= 20; i++)
      ybins[i] = -0.8 + i * (1.6 / 20.0);

    // 先进行 3D -> 2D 投影
    TH2F *rawReco2D = (TH2F *)recoHist->Project3D("yx");
    if (part > 0)
    {
      TH2F *rawBkg = (TH2F *)recoHist_Bckg->Project3D("yx");
      rawReco2D->Add(rawBkg, -1);
    }

    // 创建新的 2D histogram
    const float *xbins = nullptr;
    int nbinsX = 0;

    if (part == 0)
    {
      xbins = xbins2;
      nbinsX = 35;
    }
    else
    {
      xbins = xbins1;
      nbinsX = 25;
    }

    recoEtaPtProj = new TH2F("recoEtaPtProj", "", nbinsX, xbins, 20, ybins);
    recoEtaPtProj->Sumw2();

    TH2F *genRebinned = new TH2F("genHistRebin", "", nbinsX, xbins, 20, ybins);
    genRebinned->Sumw2();
    // 手动合并内容
    for (int ix = 1; ix <= recoEtaPtProj->GetNbinsX(); ix++)
    {
      double xlow = recoEtaPtProj->GetXaxis()->GetBinLowEdge(ix);
      double xup = recoEtaPtProj->GetXaxis()->GetBinUpEdge(ix);
      for (int iy = 1; iy <= recoEtaPtProj->GetNbinsY(); iy++)
      {
        double ylow = recoEtaPtProj->GetYaxis()->GetBinLowEdge(iy);
        double yup = recoEtaPtProj->GetYaxis()->GetBinUpEdge(iy);
        double sum = 0;

        for (int ox = 1; ox <= rawReco2D->GetNbinsX(); ox++)
        {
          double xc = rawReco2D->GetXaxis()->GetBinUpEdge(ox);
          if (xc <= xlow || xc > xup)
            continue;
          for (int oy = 1; oy <= rawReco2D->GetNbinsY(); oy++)
          {
            double yc = rawReco2D->GetYaxis()->GetBinUpEdge(oy);
            if (yc <= ylow || yc > yup)
              continue;
            sum += rawReco2D->GetBinContent(ox, oy);
          }
        }

        recoEtaPtProj->SetBinContent(ix, iy, sum);
      }
    }

    // 对生成图也做同样合并

    for (int ix = 1; ix <= genRebinned->GetNbinsX(); ix++)
    {
      double xlow = genRebinned->GetXaxis()->GetBinLowEdge(ix);
      double xup = genRebinned->GetXaxis()->GetBinUpEdge(ix);
      for (int iy = 1; iy <= genRebinned->GetNbinsY(); iy++)
      {
        double ylow = genRebinned->GetYaxis()->GetBinLowEdge(iy);
        double yup = genRebinned->GetYaxis()->GetBinUpEdge(iy);
        double sum = 0;

        for (int ox = 1; ox <= genHist->GetNbinsX(); ox++)
        {
          double xc = genHist->GetXaxis()->GetBinUpEdge(ox);
          if (xc <= xlow || xc > xup)
            continue;
          for (int oy = 1; oy <= genHist->GetNbinsY(); oy++)
          {
            double yc = genHist->GetYaxis()->GetBinUpEdge(oy);
            if (yc <= ylow || yc > yup)
              continue;
            sum += genHist->GetBinContent(ox, oy);
          }
        }
        genRebinned->SetBinContent(ix, iy, sum);
      }
    }

    recoEtaPtProj->Divide(genRebinned);
  }

  // 绘图部分保持不变
  TCanvas *can = Plotter::CreateCanvas("c");
  gPad->SetTheta(45);
  gPad->SetPhi(40);
  gPad->GetFrame()->SetLineColor(0);
  Plotter::Set2DHistAxes(recoEtaPtProj, "#font[52]{p}_{T} (GeV/#font[52]{c})", "#eta", "#varepsilon", "");
  if (part == 0)
    recoEtaPtProj->GetXaxis()->SetRangeUser(2, 50);
  recoEtaPtProj->GetZaxis()->SetTitleOffset(1);
  recoEtaPtProj->GetXaxis()->SetTitleOffset(1.5);
  recoEtaPtProj->GetYaxis()->SetTitleOffset(1.5);
  recoEtaPtProj->DrawCopy("surf2 fb");
  recoEtaPtProj->DrawCopy("surf same fb");
  recoEtaPtProj->SetName(Form("hEfficiency%s", name[part].Data()));

  recoHist->GetYaxis()->SetRangeUser(-0.8, 0.8);
  if (part == 0)
    recoHist->GetXaxis()->SetRangeUser(2, 50);
  genHist->GetYaxis()->SetRangeUser(-0.8, 0.8);
  if (part == 0)
    genHist->GetXaxis()->SetRangeUser(2, 50);

  TH1F *pt_eff = (TH1F *)recoHist->Project3D("x");
  pt_eff->SetName(Form("pt_%s", name[part].Data()));
  TH1F *genPtProj = (TH1F *)genHist->ProjectionX();
  genPtProj->SetName("genPtProj");
  pt_eff->Divide(genPtProj);
  TCanvas *canPtEff = Plotter::CreateCanvas("canPtEff");
  Plotter::SetHistAxes(pt_eff, "#font[52]{p}_{T} (GeV/#font[52]{c})", "#varepsilon");
  Plotter::SetHist(pt_eff, "", 20, kBlack, 1.2);
  pt_eff->DrawCopy();

  TPaveText *pave = new TPaveText();
  Plotter::SetPaveText(pave, 42, 0.05, 0, 0, 33, 0, 0.55, 0.97, 0.7, 0.95);
  pave->AddText("ALICE, Work in Progress");
  pave->AddText("pp, 13.6 TeV");
  if (part == 0)
    pave->AddText(Form("%s, 2 < #font[52]{p}_{T} < 50 GeV/#font[52]{c}", finalNames[part].Data()));
  else
    pave->AddText(Form("%s, |#eta| < 0.8", finalNames[part].Data()));
  pave->Draw("same");

  TH1F *eta_eff = (TH1F *)recoHist->Project3D("y");
  eta_eff->SetName(Form("eta_%s", name[part].Data()));
  TH1F *genEtaProj = (TH1F *)genHist->ProjectionY();
  genEtaProj->SetName("genEtaProj");
  eta_eff->Divide(genEtaProj);

  TCanvas *canEtaEff = Plotter::CreateCanvas("canEtaEff");
  Plotter::SetHistAxes(eta_eff, "#eta", "#varepsilon");
  Plotter::SetHist(eta_eff, "", 20, kBlack, 1.2);
  eta_eff->DrawCopy("");
  pave->Draw("same");

  TFile *fileNew = new TFile(Form("%s/Eff_%s.root", analysisFilePath.Data(), name[part].Data()), "RECREATE");
  eta_eff->Write();
  pt_eff->Write();
  recoEtaPtProj->Write();
  fileNew->Close();
}
