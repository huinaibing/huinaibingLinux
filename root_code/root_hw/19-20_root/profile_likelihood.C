#include "TH1D.h"
#include <map>
#include <string>
#include <iostream>
#include "TFile.h"
#include <vector>
#include "TLatex.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooMsgService.h"
#include "RooRandom.h"
#include "RooCategory.h"
#include "RooFormulaVar.h"
#include "RooCBShape.h"
#include "RooHist.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooKeysPdf.h"
#include "RooAbsCategory.h"
#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "TObject.h"
#include "TCut.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TColor.h"
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include "TPostScript.h"
#include "RooCBShape.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooExtendPdf.h"
#include "RooExponential.h"
#include "RooPlot.h"
#include "RooFit.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooHist.h"
#include "RooNLLVar.h"
#include "RooMinimizer.h"
#include "RooFitResult.h"
#include "RooGlobalFunc.h"
#include "RooAddModel.h"

using namespace RooStats;

bool _use_Frequentist = false;

void profile_likelihood()
{
    double Mass;
    TH1D *hist_data = new TH1D("hist_data", "", 100, 5000, 6000);
    TH1D *hist_sig = new TH1D("hist_sig", "", 100, 5000, 6000);
    TH1D *hist_sig_clone = new TH1D("hist_sig_clone", "", 100, 5000, 6000);
    TH1D *hist_bkg = new TH1D("hist_bkg", "", 100, 5000, 6000);
    TH1D *hist_bkg_up = new TH1D("hist_bkg_up", "", 100, 5000, 6000);
    TH1D *hist_bkg_down = new TH1D("hist_bkg_down", "", 100, 5000, 6000);
    hist_data->Sumw2();
    hist_sig->Sumw2();
    hist_sig_clone->Sumw2();
    hist_bkg->Sumw2();
    hist_bkg_up->Sumw2();
    hist_bkg_down->Sumw2();

    // Prepare data histograms
    TChain *ch_data = new TChain("Tuple");
    TChain *ch_sig = new TChain("Tuple");
    TChain *ch_bkg = new TChain("Tuple");

    ch_data->Add("pseudoData_Signal_BKG.root");
    ch_sig->Add("pseudoData_Signal.root");
    ch_bkg->Add("pseudoData_BKG.root");

    ch_data->SetBranchAddress("Mass", &Mass);
    ch_sig->SetBranchAddress("Mass", &Mass);
    ch_bkg->SetBranchAddress("Mass", &Mass);

    // Fill data histogram
    for (int i = 0; i < ch_data->GetEntries(); i++)
    {
        ch_data->GetEntry(i);
        hist_data->Fill(Mass);
    }

    // Fill signal histograms
    for (int i = 0; i < ch_sig->GetEntries(); i++)
    {
        ch_sig->GetEntry(i);
        hist_sig->Fill(Mass);
        hist_sig_clone->Fill(Mass);
    }

    // Fill background histograms with 5% systematic uncertainty
    for (int i = 0; i < ch_bkg->GetEntries(); i++)
    {
        ch_bkg->GetEntry(i);
        hist_bkg->Fill(Mass);
        hist_bkg_up->Fill(Mass, 1.05);   // +5% uncertainty
        hist_bkg_down->Fill(Mass, 0.95); // -5% uncertainty
    }

    RooWorkspace *wspace = new RooWorkspace("wspace_forCLs");

    HistFactory::Measurement *meas = new HistFactory::Measurement("my_measurement", "my_measurement");
    meas->SetExportOnly(false);

    meas->SetPOI("nSignal");

    HistFactory::Channel *channel = new HistFactory::Channel("wspace_forCLs");

    meas->SetLumi(1.0);

    meas->AddConstantParam("Lumi");

    channel->SetData(hist_data);

    HistFactory::Sample *signal = new HistFactory::Sample("signal");
    signal->SetNormalizeByTheory(false);

    double n_sig = hist_sig->Integral();
    hist_sig->Scale(1. / n_sig);
    signal->SetHisto(hist_sig);

    signal->AddNormFactor("nSignal", 6, 0.0, 500.0); 

    double syst_tot = 0.03;
    signal->AddOverallSys("signal_norm_uncertainty", 1. - syst_tot, 1. + syst_tot);

    channel->AddSample(*signal);

    HistFactory::Sample *background = new HistFactory::Sample("background");
    background->SetNormalizeByTheory(false);

    double nbkg = hist_bkg->Integral();
    double ndata = hist_data->Integral();
    hist_bkg->Scale(ndata / nbkg);
    hist_bkg_up->Scale(ndata / nbkg);
    hist_bkg_down->Scale(ndata / nbkg);
    background->SetHisto(hist_bkg);

    HistFactory::HistoSys *background_shape = new HistFactory::HistoSys();
    background_shape->SetHistoHigh(hist_bkg_up);
    background_shape->SetHistoLow(hist_bkg_down);
    background->AddHistoSys(*background_shape);

    channel->AddSample(*background);

    meas->AddChannel(*channel);
    HistFactory::HistoToWorkspaceFactoryFast *hist2workspace = new HistFactory::HistoToWorkspaceFactoryFast(*meas);
    wspace = hist2workspace->MakeSingleChannelModel(*meas, *channel);

    wspace->Print();
    wspace->writeToFile("singlePoint_test.root");

    TFile *inputfile = new TFile("singlePoint_test.root");

    RooWorkspace *wspace2 = (RooWorkspace *)inputfile->Get("wspace_forCLs");

    ModelConfig *sbModel, *bModel;
    RooDataSet *input_data;

    sbModel = (ModelConfig *)wspace2->obj("ModelConfig");
    sbModel->SetSnapshot(RooArgSet(*wspace2->var("nSignal")));
    input_data = (RooDataSet *)wspace2->data("obsData");

    sbModel->SetObservables(RooArgSet(*wspace2->var("obs_x_wspace_forCLs")));

    bModel = (ModelConfig *)sbModel->Clone("BKG_ModelConfig");
    RooRealVar *poi = (RooRealVar *)sbModel->GetParametersOfInterest()->first();
    bModel->SetParametersOfInterest(RooArgSet(*wspace2->var("nSignal")));
    bModel->SetObservables(RooArgSet(*wspace2->var("obs_x_wspace_forCLs")));
    poi->setVal(0);
    bModel->SetSnapshot(*poi);

    ProfileLikelihoodCalculator *pl = new ProfileLikelihoodCalculator(*input_data, *sbModel);
    HypoTestResult *htr = pl->GetHypoTest();
    Double_t pValue = htr->NullPValue();
    Double_t significance = htr->Significance();

    pl->SetConfidenceLevel(0.95);
    Double_t confLevel = 0.95;
    LikelihoodInterval *interval = pl->GetInterval();

    RooRealVar *firstPOI = (RooRealVar *)sbModel->GetParametersOfInterest()->first();

    LikelihoodIntervalPlot *plot = new LikelihoodIntervalPlot(interval);
    plot->SetNPoints(50);
    TCanvas *cas = new TCanvas("cas", "", 700, 500);
    cas->SetLogy(false);
    plot->Draw("tf1");
    cas->Print("ProfileLikelihood_Test.pdf");

    RooArgSet nullparams("nullparams");
    nullparams.addClone(*firstPOI);
    nullparams.setRealValue(firstPOI->GetName(), 0);
    pl->SetNullParameters(nullparams);

    std::cout << "----------------------------------------Result------------------------------" << std::endl;
    pl->GetHypoTest()->Print();
}
