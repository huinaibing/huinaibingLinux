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

#include "/home/huinaibing/huinaibingLinux/root_code/rootTutorial17/BUILD/MyCB.h"
#include "RooGlobalFunc.h"
#include <fstream>

#pragma cling load("/home/huinaibing/huinaibingLinux/root_code/rootTutorial17/BUILD/MyCB_cxx.so")

void geneSignal(RooRealVar* m, RooRealVar* pt, std::ofstream& out, int nsig);
void geneBackground(RooRealVar* m, RooRealVar* pt, std::ofstream& out, int nbkg);
void convertToRoot(std::string txtfile, std::string rootfile, std::string treename);

const int n_signal = 10000;
const int n_bkg = 100000;
const int n_tot = 1000000;

//
// main函数
//
void generateData(int runType=0) {
    // Create the variables
    // 下面是赝数据具有的变量
    RooRealVar* m = new RooRealVar("m", "M^{B^{+}}", 5100, 5550, "MeV");
    RooRealVar* pt = new RooRealVar("pt", "p_{T}", 0, 20000, "MeV");

    // 输出的文本文件
    std::ofstream out("pseudoData.txt");

    std::string output_name = "";
    if (runType == 0)
    {
        geneSignal(m, pt, out, n_signal);
        geneBackground(m, pt, out, n_bkg);
        output_name = "PseudoData_signal_bkg.root";
    }
    else if (runType == 1)
    {
        geneSignal(m, pt, out, n_tot);
        output_name = "PseudoData_signal.root";
    }
    else if (runType == 2)
    {
        geneBackground(m, pt, out, n_tot);
        output_name = "PseudoData_bkg.root";
    }
    
    convertToRoot("pseudoData.txt", output_name, "myTuple");
    out.close();
    std::cout << "Data generation completed. Output file: " << output_name << std::endl;
}
// END main函数
// -----------------------------------------------------

// 
// 生成信号事件
// 
void geneSignal(RooRealVar* m, RooRealVar* pt, std::ofstream& out, int nsig) 
{
    std::cout << "call geneSignal()" << std::endl;

    RooRealVar* CBL = new RooRealVar("CBL", "", 2.0);
    RooRealVar* CBR = new RooRealVar("CBR", "", 8.0);

    RooRealVar* alpha_L = new RooRealVar("alpha_L", "alpha_L", 2.05);
    RooRealVar* alpha_R = new RooRealVar("alpha_R", "alpha_R", 1.89);

    RooRealVar* mean = new RooRealVar("mean", "mean", 5279.31);
    RooRealVar* sigma = new RooRealVar("sigma", "sigma", 17.3);

    RooAbsPdf* signal = new MyCB("signal", "signal", *m, *mean, *sigma, *alpha_L, *CBL, *alpha_R, *CBR);
}
