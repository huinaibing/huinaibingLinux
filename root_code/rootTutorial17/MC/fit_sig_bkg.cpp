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
#include "/home/huinaibing/huinaibingLinux/root_code/rootTutorial17/ROOFIT/MyCB.h"

#pragma cling load("/home/huinaibing/huinaibingLinux/root_code/rootTutorial17/BUILD/MyCB_cxx.so")

using namespace RooFit;


void fit_sig_bkg()
{
    TChain* chain = new TChain();
    chain->Add("/home/huinaibing/huinaibingLinux/root_code/rootTutorial17/MC/BUILD/PseudoData_signal_bkg.root/myTuple");
     
}