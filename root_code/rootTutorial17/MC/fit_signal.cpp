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

void fit_signal()
{
    //----------------------------------
    // 显然我们就改了一下文件的路径就没了
    // 这里的代码复用性很差，纯史山，但是我不想改
    //----------------------------------

    // 加载root文件
    // 使用chain指向myTuple
    TChain* chain = new TChain();
    chain->Add("/home/huinaibing/huinaibingLinux/root_code/rootTutorial17/MC/BUILD/PseudoData_signal.root/myTuple");

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

    // 这下面的是水晶球函数的尾巴参数
    RooRealVar* CBL = new RooRealVar("CBL", "", 2.0, 0.0, 10);
    RooRealVar* CBR = new RooRealVar("CBR", "", 4.0, 0.0, 10);

    RooRealVar* alpha_L = new RooRealVar("alpha_L", "alpha_L", 2.05, 0, 100.0);
    RooRealVar* alpha_R = new RooRealVar("alpha_R", "alpha_R", 1.89, 0.0, 100.0);

    // 水晶球的中心值
    RooRealVar* mean = new RooRealVar("mean", "mean", 5279.0, 5100.0, 5340.0);
    
    // 信号和本底的数字
    RooRealVar* ns = new RooRealVar("ns", "sig ets", 237651, 0, 400000);
    RooRealVar* nb = new RooRealVar("nb", "sig ets", 237651, 0, 900000);

    // 水晶球函数的分辨率（为啥是分辨率？我不造啊）
    RooRealVar* sigma = new RooRealVar("sigma", "sigma", 10.4, 4, 20);

    // 定义水晶球函数
    RooAbsPdf* signal = new MyCB("signal", "signal", *m, *mean, *sigma, *alpha_L, *CBL, *alpha_R, *CBR);

    // 把信号数和概率密度函数结合在一起
    RooExtendPdf* signex = new RooExtendPdf("signex", "mass pdf", *signal, *ns);

    /*****************************************/
    // 开始拟合
    // 使用binned log-likelihood拟合
    // 这是啥？我也不知道
    RooDataHist* dhist_0 = data->binnedClone();
    RooFitResult* fitr = signex->fitTo(*dhist_0, Save());
    
    // 从拟合变量中获取画图框架
    // 为什么要从这里拿？很奇怪的代码
    RooPlot* jframe = m->frame();

    // 设置画图的各种格式
    dhist_0->plotOn(jframe, Name("kaon_fit"), MarkerSize(0.8));
    signex->plotOn(jframe, Components("signal"), LineColor(6), LineStyle(kDashed));
    signex->plotOn(jframe, Components("signex"), LineColor(kRed));
    signex->plotOn(jframe, Name("kaon_all"), LineColor(kBlue));

    // 画各种标志
    TLatex* myTex = new TLatex(0.18, 0.80, "#splitline{B_{u}^{+}#rightarrow J/#psi K^{+}}{MC}");
    myTex->SetTextFont(132);
    myTex->SetTextSize(0.05);
    myTex->SetLineWidth(2);
    myTex->SetNDC();

    char name_input[200];
    sprintf(name_input, "#sigma_{B} = %.2f #pm %.2f MeV", sigma->getVal(), sigma->getError());
    TLatex* myTex1 = new TLatex(0.51, 0.80, name_input);
    myTex1->SetTextFont(132);
    myTex1->SetTextSize(0.05);
    myTex1->SetLineWidth(2);
    myTex1->SetNDC();

    sprintf(name_input, "N_{s} = %d", int(ns->getVal()));
    TLatex* myTex2 = new TLatex(0.51, 0.75, name_input);
    myTex2->SetTextFont(132);
    myTex2->SetTextSize(0.05);
    myTex2->SetLineWidth(2);
    myTex2->SetNDC();

    sprintf(name_input, "M_{B} = %.2f #pm %.2f MeV", mean->getVal(), mean->getError());
    TLatex* myTex3 = new TLatex(0.51, 0.70, name_input);
    myTex3->SetTextFont(132);
    myTex3->SetTextSize(0.05);
    myTex3->SetLineWidth(2);
    myTex3->SetNDC();

    jframe->addObject(myTex);
    jframe->addObject(myTex1);
    jframe->addObject(myTex2);
    jframe->addObject(myTex3);
    jframe->SetTitle("");
    TCanvas* c_Data = new TCanvas("c_Data", "", 700, 600);
    // 这个divide函数是把画布分成几块，
    // 里面参数的意义我也不知道
    c_Data->Divide(1, 2, 0, 0, 0);

    // 把图像画在画布的第二块上
    c_Data->cd(2);
    gPad->SetTopMargin(0.0);
    gPad->SetLeftMargin(0.12);
    gPad->SetPad(0.02, 0.02, 0.98, 0.77);
    jframe->Draw();

    // 画图的第一块放pull图
    c_Data->cd(1);
    gPad->SetTopMargin(0.0);
    gPad->SetLeftMargin(0.12);
    gPad->SetPad(0.02, 0.77, 0.98, 0.98);
    // 画pull图
    // 从jframe里面拉取pull图
    RooHist* hpull1 = jframe->pullHist();
    // 下面这个和上面的一样，都是从m这里拉取画图框架
    RooPlot* mframeh1 = m->frame(Title("pull dis"));
    hpull1->SetFillColor(15);
    hpull1->SetFillStyle(3001);
    mframeh1->addPlotable(hpull1, "L3");
    mframeh1->GetYaxis()->SetNdivisions(505);
    mframeh1->GetYaxis()->SetLabelSize(0.2);
    mframeh1->SetMinimum(-5);
    mframeh1->SetMaximum(5);
    mframeh1->Draw();

    TFile* f = new TFile("fit_signal.root", "recreate");
    c_Data->Write();
    f->Close();
}