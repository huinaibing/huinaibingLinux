#include <map>
#include <string>
#include <iostream>
#include <vector>
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
#include "MyCB.h"

#pragma cling load("../BUILD/MyCB_cxx.so")

using namespace RooFit;

void fit_mass()
{
    // 加载root文件
    // 使用chain指向myTuple
    TChain* chain = new TChain();
    chain->Add("/home/huinaibing/huinaibingLinux/root_code/root_hw/15_root/make_class_method/bmass_test.root/myTuple");

    // 把变量绑定到叶子Mass上
    Double_t bmass;
    chain->SetBranchAddress("Mass", &bmass);

    // 读取数据的数量
    Int_t Num = chain->GetEntries();
    std::cout << "Num = " << Num << std::endl;

    // 创建一个RooRealVar对象
    RooRealVar* m = new RooRealVar("m", "M(B^{+})", 5100, 5550, "MeV");
    // 这个对象是我们要拟合的变量
    // 它也和我们将要画的图有关

    // 设置画图的时候使用的bin数量
    m->setBins(100);

    // 这是一个dataset，里面包含了我们要拟合的数据
    // 而上面的RealVar是需要拟合的变量
    RooDataSet* data = new RooDataSet("data", "data", RooArgSet(*m));

    // 填入数据
    for(int i = 0; i < Num; i++)
    {
        // 获取变量
        chain->GetEntry(i);
        
        if (bmass < 5100 || bmass > 5550)
        { 
            continue; // 过滤数据，
            // 这个过滤的阈值是我们定义RealVar的时候写的边界值
        }
        
        *m = bmass; // 赋值
        // 上面这个地方注意到RooRealVar类型的变量
        // 指向了Double_t

        data->add(RooArgSet(*m)); // 添加到dataset中
        // 我们在上面定义dataset的时候
        // 以经把*m指针给他了怎么还要我传一遍
    }
    //以上代码已经将数据读入到DataSet里面
    
    // 下面开始拟合
    RooRealVar* C = new RooRealVar("C", "", 1.0);
    
}