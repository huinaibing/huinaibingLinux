#include "Plotter.h"

void CalculateEfficiencyAttempt(Int_t part = 2){
/*
  TString name[]={"Trigger","K0Short","Lambda","AntiLambda","XiMinus","XiPlus","OmegaMinus","OmegaPlus","Pion"};
  TString finalNames[]={"h","K_{S}^{0}","#Lambda","#bar{#Lambda}","#Xi^{-}","#Xi^{+}","#Omega^{-}","#Omega^{+})","#pi^{+}+#pi^{-}"};

  TFile * fFile_unprimary = new TFile(Form("~/Downloads/AnalysisResults.root"));
  TFile * fFile_primary = new TFile(Form("~/Downloads/AnalysisResults_primary.root"));

  TH2F * genHist_unprimary  = (TH2F *) fFile_unprimary->Get(Form("h-strange-correlation/GeneratedWithPV/h%s",name[part].Data()));//
  genHist_unprimary->Sumw2();

  TH2F * genHist_primary  = (TH2F *) fFile_primary->Get(Form("h-strange-correlation/GeneratedWithPV/h%s",name[part].Data()));//
  genHist_primary->Sumw2();

  TH1F * hPvz_unprimary  = (TH1F *) fFile_unprimary->Get(Form("h-strange-correlation/EventQA/hPvz"));//
  TH1F * hPvz_primary  = (TH1F *) fFile_primary->Get(Form("h-strange-correlation/EventQA/hPvz"));//

  TH3F * recoHist_unprimary  = (TH3F *) fFile_unprimary->Get(Form("h-strange-correlation/h%sEtaVsPtVsPhi",name[part].Data()));//_requireTrigger_3
  recoHist_unprimary->Sumw2();

  TH3F * recoHist_primary  = (TH3F *) fFile_primary->Get(Form("h-strange-correlation/h%sEtaVsPtVsPhi",name[part].Data()));//_requireTrigger_3
  recoHist_primary->Sumw2();

  TH3F * recoHist_Bckg_unprimary;
  TH3F * recoHist_Bckg_primary;
  if(part>0)  {
    recoHist_Bckg_unprimary= (TH3F *) fFile_unprimary->Get(Form("h-strange-correlation/h%sEtaVsPtVsPhiBg",name[part].Data()));//_requireTrigger_3
    recoHist_Bckg_unprimary->Sumw2();
    recoHist_Bckg_primary= (TH3F *) fFile_primary->Get(Form("h-strange-correlation/h%sEtaVsPtVsPhiBg",name[part].Data()));//_requireTrigger_3
    recoHist_Bckg_primary->Sumw2();
  }

  TH2F * recoEtaPtProj_unprimary = (TH2F*) recoHist_unprimary->Project3D("yx");
  recoEtaPtProj_unprimary->SetName(Form("hEfficiency%s_unprimary",name[part].Data()));
  TH2F * recoEtaPtProj_Bckg_unprimary = (TH2F*) recoHist_Bckg_unprimary->Project3D("yx");
  recoEtaPtProj_Bckg_unprimary->SetName(Form("hEfficiency_bckg%s_unprimary",name[part].Data()));
  //recoEtaPtProj_unprimary->Add(recoEtaPtProj_Bckg_unprimary,-1);

  TH2F * recoEtaPtProj_primary = (TH2F*) recoHist_primary->Project3D("yx");
  recoEtaPtProj_primary->SetName(Form("hEfficiency%s_primary",name[part].Data()));
  TH2F * recoEtaPtProj_Bckg_primary = (TH2F*) recoHist_Bckg_primary->Project3D("yx");
  recoEtaPtProj_Bckg_primary->SetName(Form("hEfficiency_bckg%s_primary",name[part].Data()));
  //recoEtaPtProj_primary->Add(recoEtaPtProj_Bckg_primary,-1);
  recoEtaPtProj_unprimary->Scale(1/hPvz_unprimary->GetEntries());
  recoEtaPtProj_Bckg_unprimary->Scale(1/hPvz_unprimary->GetEntries());
  recoEtaPtProj_primary->Scale(1/hPvz_primary->GetEntries());
  recoEtaPtProj_Bckg_primary->Scale(1/hPvz_primary->GetEntries());

  recoEtaPtProj_unprimary->Add(recoEtaPtProj_primary,-1);
  recoEtaPtProj_Bckg_unprimary->Add(recoEtaPtProj_Bckg_primary,-1);
  recoEtaPtProj_unprimary->Add(recoEtaPtProj_Bckg_unprimary,-1);
  recoEtaPtProj_unprimary->SetName(Form("hEfficiency%s_Secondary",name[part].Data()));

  genHist_unprimary->Scale(1/hPvz_unprimary->GetEntries());
  genHist_primary->Scale(1/hPvz_primary->GetEntries());

  recoEtaPtProj_unprimary->Divide(genHist_unprimary);
  // recoEtaPtProj_woBckg->Divide(genHist);
  TCanvas * can = Plotter::CreateCanvas("c");
  gPad->SetTheta(45);
  gPad->SetPhi(40);
  gPad->GetFrame()->SetLineColor(0);
  Plotter::Set2DHistAxes(recoEtaPtProj_unprimary,"#font[52]{p}_{T} (GeV/#font[52]{c})","#eta","#varepsilon","");

  recoEtaPtProj_unprimary->GetZaxis()->SetTitleOffset(1);
  recoEtaPtProj_unprimary->GetXaxis()->SetTitleOffset(1.5);
  recoEtaPtProj_unprimary->GetYaxis()->SetTitleOffset(1.5);
  recoEtaPtProj_unprimary->DrawCopy("surf2 fb");
  recoEtaPtProj_unprimary->DrawCopy("surf same fb");

  recoHist_unprimary->GetYaxis()->SetRangeUser(-0.8,0.8);
  recoHist_primary->GetYaxis()->SetRangeUser(-0.8,0.8);
  genHist_unprimary->GetYaxis()->SetRangeUser(-0.8,0.8);
  //
  TH1F * pt_eff = (TH1F*) recoHist_unprimary->Project3D("x");
  pt_eff->Scale(1/hPvz_unprimary->GetEntries());
  //pt_eff->Draw();
  TH1F * pt_eff_primary = (TH1F*) recoHist_primary->Project3D("x");
  pt_eff_primary->Scale(1/hPvz_primary->GetEntries());
  //pt_eff_primary->Draw();

  pt_eff->SetName(Form("pt_%s",name[part].Data()));
  //pt_eff->Add(pt_eff_primary,-1);

  TH1F * genPtProj = (TH1F*) genHist_unprimary->ProjectionX();
  genPtProj->SetName("genPtProj");
  pt_eff_primary->Divide(genPtProj);

  TCanvas * canPtEff = Plotter::CreateCanvas("canPtEff");
  Plotter::SetHistAxes(pt_eff,"#font[52]{p}_{T} (GeV/#font[52]{c})","#varepsilon");
  Plotter::SetHist(pt_eff,"",20,kBlack,1.2);

  pt_eff_primary->DrawCopy();

  TPaveText * pave = new TPaveText();
  Plotter::SetPaveText(pave,42,0.05, 0, 0,33,0,0.55,0.97, 0.7,0.95);
  pave->AddText("ALICE, Work in Progress");
  pave->AddText("pp, 13.6 TeV");
  //
  if(part==0) pave->AddText(Form("%s, 2 < #font[52]{p}_{T} < 50 GeV/#font[52]{c}",finalNames[part].Data()));
  else pave->AddText(Form("%s, |#eta| < 0.8",finalNames[part].Data()));
  pave->Draw("same");

  TFile *fileNew = new TFile(Form("/Users/cuikai/Desktop/work/Eff_%s_Secondary.root",name[part].Data()), "RECREATE");
  recoEtaPtProj_unprimary->Write();
  fileNew->Close();
*/

TString name[]={"Trigger","K0Short","Lambda","AntiLambda","XiMinus","XiPlus","OmegaMinus","OmegaPlus","Pion"};
  TString finalNames[]={"h","K_{S}^{0}","#Lambda","#bar{#Lambda}","#Xi^{-}","#Xi^{+}","#Omega^{-}","#Omega^{+})","#pi^{+}+#pi^{-}"};

  TFile * fFile = new TFile(Form("~/Downloads/AnalysisResults.root"));

  TH2F * genHist  = (TH2F *) fFile->Get(Form("h-strange-correlation/GeneratedWithPV/h%sFromXiZero",name[part].Data()));//
  genHist->Sumw2();

  if(part==0)name[0]="Track";
  TH3F * recoHist  = (TH3F *) fFile->Get(Form("h-strange-correlation/h%sFromXiZeroEtaVsPtVsPhi",name[part].Data()));//_requireTrigger_3
  recoHist->Sumw2();
  if(part==0){
    recoHist->GetXaxis()->SetRange(21,55);
  }
  if(part==0)name[0]="Trigger";

  Double_t ptAxis[]={2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.4, 4.8, 5.2, 5.6, 6.0, 6.5, 7.0, 7.5, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 17.0, 19.0, 21.0, 23.0, 25.0, 30.0, 35.0, 40.0, 50.0};
  TH2F * genHistClone = new TH2F("genHistClone","",35,ptAxis,80, -0.8, 0.8);
  for (size_t i = 0; i < 35; i++) {
    for (size_t j = 0; j < 80; j++) {
      genHistClone->SetBinContent(i+1,j+1,genHist->GetBinContent(21+i,j+1));
    }
  }

  TH2F * recoEtaPtProj = (TH2F*) recoHist->Project3D("yx");
  recoEtaPtProj->SetName(Form("hEfficiency%s",name[part].Data()));


  if (part==0) recoEtaPtProj->Divide(genHistClone);
  else recoEtaPtProj->Divide(genHist);
  TCanvas * can = Plotter::CreateCanvas("c");
  gPad->SetTheta(45);
  gPad->SetPhi(40);
  gPad->GetFrame()->SetLineColor(0);
  Plotter::Set2DHistAxes(recoEtaPtProj,"#font[52]{p}_{T} (GeV/#font[52]{c})","#eta","#varepsilon","");
  // Plotter::Set2DHistAxes(recoEtaPtProj_woBckg,"p_{T}","#eta","eff","");
  if(part==0) recoEtaPtProj->GetXaxis()->SetRangeUser(2,50);

  recoEtaPtProj->GetZaxis()->SetTitleOffset(1);
  recoEtaPtProj->GetXaxis()->SetTitleOffset(1.5);
  recoEtaPtProj->GetYaxis()->SetTitleOffset(1.5);
  recoEtaPtProj->DrawCopy("surf2 fb");
  recoEtaPtProj->DrawCopy("surf same fb");

  // //1d Projections
  //recoHist->GetYaxis()->SetRangeUser(-0.8,0.8);
  if(part==0)recoHist->GetXaxis()->SetRangeUser(2,50);
  // recoHist_Bckg->GetYaxis()->SetRangeUser(-0.8,0.8);
  //genHist->GetYaxis()->SetRangeUser(-0.8,0.8);
  if(part==0)genHist->GetXaxis()->SetRangeUser(2,50);

  TH1F * pt_eff = (TH1F*) recoHist->Project3D("x");
  pt_eff->SetName(Form("pt_%s",name[part].Data()));
  
  TH1F * genPtProj = (TH1F*) genHist->ProjectionX();
  genPtProj->SetName("genPtProj");
  
  pt_eff->Divide(genPtProj);
  for(int i =0; i<pt_eff->GetXaxis()->GetNbins();i++){
    cout<<pt_eff->GetBinError(i+1)<<endl;
  }
  TCanvas * canPtEff = Plotter::CreateCanvas("canPtEff");
  //
  // Plotter::SetHist(pt_eff_wPv,"",24,kBlack,1.2);
  Plotter::SetHistAxes(pt_eff,"#font[52]{p}_{T} (GeV/#font[52]{c})","#varepsilon");
  Plotter::SetHist(pt_eff,"",20,kBlack,1.2);
  // Plotter::SetHist(pt_eff_woBckg,"",20,kBlue+1,1.2);
  // Plotter::SetHist(pt_eff_woBckg_wPV,"",24,kBlue+1,1.2);
  pt_eff->DrawCopy();
  // pt_eff_wPv->DrawCopy("same");
  // pt_eff_woBckg->DrawCopy("same");
  // pt_eff_woBckg_wPV->DrawCopy("same");
  //
  TPaveText * pave = new TPaveText();
  Plotter::SetPaveText(pave,42,0.05, 0, 0,33,0,0.55,0.97, 0.7,0.95);
  pave->AddText("ALICE, Work in Progress");
  pave->AddText("pp, 13.6 TeV");
  //
  if(part==0) pave->AddText(Form("%s, 2 < #font[52]{p}_{T} < 50 GeV/#font[52]{c}",finalNames[part].Data()));
  else pave->AddText(Form("%s, |#eta| < 0.8",finalNames[part].Data()));
  pave->Draw("same");
  //
  // TLegend *leg= Plotter::CreateLegend(0.55, 0.92, 0.45, 0.65,0.05);
  // leg->AddEntry(pt_eff,"Gen: all","p");
  // leg->AddEntry(pt_eff_wPv,"Gen: only with PV","p");
  // leg->AddEntry(pt_eff_woBckg,"Gen: all, Rec: bckg. subtracted","p");
  // leg->AddEntry(pt_eff_woBckg_wPV,"Gen: only with PV, Rec: bckg. subtracted","p");
  // leg->Draw();
  //
  // TCanvas * canPtEffRatio = Plotter::CreateCanvas("canPtEffRatio");
  // TH1F * pt_eff_Ratio = (TH1F *) pt_eff->Clone();
  // pt_eff_Ratio->SetName("pt_eff_Ratio");
  // pt_eff_Ratio->Divide(pt_eff_wPv);
  // Plotter::SetHistAxes(pt_eff_Ratio,"#font[12]{p}_{T} (GeV/#font[12]{c})","#varepsilon_{all}/#varepsilon_{with PV}");
  // pt_eff_Ratio->DrawCopy();
  // pave->Draw("same");
  //
  TH1F * eta_eff = (TH1F*) recoHist->Project3D("y");
  eta_eff->SetName(Form("eta_%s",name[part].Data()));
  //
  // TH1F * eta_eff_wPv = (TH1F*) recoHist->Project3D("y");
  // eta_eff_wPv->SetName(Form("eta_%s_wPV",name[part].Data()));
  //
  // TH1F * eta_eff_woBckg = (TH1F*) recoHist->Project3D("y");
  // eta_eff_woBckg->Add((TH1F*) recoHist_Bckg->Project3D("y"),-1);
  // eta_eff_woBckg->SetName(Form("eta_%s_woBckg",name[part].Data()));
  //
  // TH1F * eta_eff_woBckg_wPV = (TH1F*) recoHist->Project3D("y");
  // eta_eff_woBckg_wPV->Add((TH1F*) recoHist_Bckg->Project3D("y"),-1);
  // eta_eff_woBckg_wPV->SetName(Form("eta_%s_woBckg_wPV",name[part].Data()));
  //
  TH1F * genEtaProj = (TH1F*) genHist->ProjectionY();
  genEtaProj->SetName("genEtaProj");
  // TH1F * genEtaProj_wPv = (TH1F*) genHist_wPV->ProjectionY();
  // genEtaProj_wPv->SetName("genPtProj_wPv");
  // eta_eff_wPv->RebinX(2);
  // eta_eff->RebinX(2);
  // eta_eff_woBckg->RebinX(2);
  // eta_eff_woBckg_wPV->RebinX(2);
  //
  eta_eff->Divide(genEtaProj);
  // eta_eff_wPv->Divide(genEtaProj_wPv);
  // eta_eff_woBckg->Divide(genEtaProj);
  // eta_eff_woBckg_wPV->Divide(genEtaProj_wPv);
  //
  TCanvas * canEtaEff = Plotter::CreateCanvas("canEtaEff");
  Plotter::SetHistAxes(eta_eff,"#eta","#varepsilon");
  Plotter::SetHist(eta_eff,"",20,kBlack,1.2);
  // Plotter::SetHist(eta_eff_wPv,"",24,kBlack,1.2);
  // Plotter::SetHist(eta_eff_woBckg,"",20,kBlue+1,1.2);
  // Plotter::SetHist(eta_eff_woBckg_wPV,"",24,kBlue+1,1.2);
  // // eta_eff->GetYaxis()->SetRangeUser(0,0.2999);
  eta_eff->DrawCopy("E1");
  // eta_eff_wPv->DrawCopy("same");
  // eta_eff_woBckg->DrawCopy("same");
  // eta_eff_woBckg_wPV->DrawCopy("same");
  pave->Draw("same");
  // leg->Draw();
  //
  // recoHist_SpectrY->GetZaxis()->SetRange(2,2);
  // TH1F * pt_eff_ycut_woBckg_wPV = (TH1F *) recoHist_SpectrY->Project3D("x");
  // pt_eff_ycut_woBckg_wPV->SetName(Form("pt_%s_ycut_woBckg_wPV",name[part].Data()));
  // recoHist_SpectrY->GetZaxis()->SetRange(1,1);
  // TH1F * pt_Bckg = (TH1F *) recoHist_SpectrY->Project3D("x");
  // pt_Bckg->SetName("pt_Bckg");
  // recoHist_SpectrY->GetZaxis()->SetRange(3,3);
  // pt_Bckg->Add((TH1F *) recoHist_SpectrY->Project3D("x"));
  //
  // pt_eff_ycut_woBckg_wPV->Add(pt_Bckg,-1);
  // pt_eff_ycut_woBckg_wPV->Divide(genPtProj_wPv_yCut);
  //
  // TCanvas * canEtaEffRatio = Plotter::CreateCanvas("canEtaEffRatio");
  // TH1F * eta_eff_Ratio = (TH1F *) eta_eff->Clone();
  // eta_eff_Ratio->SetName("eta_eff_Ratio");
  // eta_eff_Ratio->Divide(eta_eff_wPv);
  // Plotter::SetHistAxes(eta_eff_Ratio,"#eta","#varepsilon_{all}/#varepsilon_{with PV}");
  // eta_eff_Ratio->DrawCopy();
  // pave->Draw("same")

  TFile *fileNew = new TFile(Form("/Users/cuikai/Desktop/work/Eff_%s_Secondary_XiZero.root",name[part].Data()), "RECREATE");
  // TFile *fileNew = new TFile(Form("../data/Efficiency/Eff_LHC24f3b_%s_MB.root",name[part].Data()), "RECREATE");
  eta_eff->Write();
  // eta_eff_wPv->Write();
  // eta_eff_woBckg->Write();
  // eta_eff_woBckg_wPV->Write();
  pt_eff->Write();
  // pt_eff_wPv->Write();
  // pt_eff_woBckg->Write();
  // pt_eff_woBckg_wPV->Write();
  // recoEtaPtProj_woBckg->Write();
  // eff_wPV->Write();
  // eff_woBckg_wPV->Write();
  recoEtaPtProj->Write();
  //genHistClone->Write();
  // pt_eff_ycut_woBckg_wPV->Write();
  fileNew->Close();

}
