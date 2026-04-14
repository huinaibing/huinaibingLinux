#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH2D.h"
#include "TH3D.h"
#include <iostream>

#define DATA_PID "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/pid/data4pid/detectorPIDQA.root"

void nsigma_check()
{
    TFile *file = TFile::Open(DATA_PID);
    TDirectory *dir = file->Get<TDirectory>("pid-flow-pt-corr")->Get<TDirectory>("DetectorPidPerformace");
    if (!dir)
    {
        std::cout << "dir no found" << std::endl;
        return;
    }

    TH3D *TPCvsTOFKa = dir->Get<TDirectory>("TPCvsTOF")->Get<TH3D>("Ka");
    if (!TPCvsTOFKa)
    {
        std::cout << "no found" << std::endl;
        return;
    }
    // axis z is PT
    TPCvsTOFKa->GetZaxis()->SetRangeUser(2, 3);

    TPCvsTOFKa->GetXaxis()->SetRangeUser(-5, 5);
    TPCvsTOFKa->GetYaxis()->SetRangeUser(-5, 5);

    TPCvsTOFKa->GetXaxis()->SetTitle("TPC");
    TPCvsTOFKa->GetYaxis()->SetTitle("TOF");

    TH2D *kaProject = (TH2D *)TPCvsTOFKa->Project3D("yx");


    // 上面已经做完投影得到2d的图，现在要再次cutY然后project x
    kaProject->GetYaxis()->SetRangeUser(-1.5, 1.5);
    // kaProject->Smooth(1);
    // kaProject->Smooth(1);
    // kaProject->Smooth(1);
    // kaProject->Smooth(1);
    // kaProject->Smooth(1);

    TH1D *pureGraph = kaProject->ProjectionX();
    pureGraph->Smooth(5000);
    pureGraph->SetMarkerStyle(22);
    pureGraph->Sumw2();

    double fit_min = -4.0;
    double fit_max = 4.0;

    // 得到图后，开始拟合双高斯
    TF1 *f_double_gaus = new TF1("f_double_gaus", "gaus(0)+gaus(3)", fit_min, fit_max);
    f_double_gaus->SetParameter(0, 8e6); // [0] 幅度（峰值）
    f_double_gaus->SetParameter(1, 0); // [1] 均值（峰位）
    f_double_gaus->SetParameter(2, 1.5); // [2] sigma（宽度）

    // 第2个高斯：本底峰（π介子等本底，峰位~0，幅度~1e7）
    f_double_gaus->SetParameter(3, 5.0e6); // [3] 幅度
    f_double_gaus->SetParameter(4, 2);   // [4] 均值
    f_double_gaus->SetParameter(5, 1);   // [5] sigma

    pureGraph->Fit(f_double_gaus, "RSV");

    double p0 = f_double_gaus->GetParameter(0); // 信号幅度
    double p1 = f_double_gaus->GetParameter(1); // 信号均值
    double p2 = f_double_gaus->GetParameter(2); // 信号sigma
    double p3 = f_double_gaus->GetParameter(3); // 本底幅度
    double p4 = f_double_gaus->GetParameter(4); // 本底均值
    double p5 = f_double_gaus->GetParameter(5); // 本底sigma

    // 定义【信号高斯】函数
    TF1 *f_signal = new TF1("f_signal", "gaus", fit_min, fit_max);
    f_signal->SetParameters(p0, p1, p2); // 传入信号参数
    f_signal->SetLineColor(kGreen + 2);  // 颜色：深绿
    f_signal->SetLineWidth(3);           // 线宽
    f_signal->SetLineStyle(2);           // 线型：虚线（方便区分）

    // 定义【本底高斯】函数
    TF1 *f_background = new TF1("f_background", "gaus", fit_min, fit_max);
    f_background->SetParameters(p3, p4, p5); // 传入本底参数
    f_background->SetLineColor(kRed);        // 颜色：红色
    f_background->SetLineWidth(3);           // 线宽
    f_background->SetLineStyle(2);           // 线型：虚线


    TCanvas *c1 = new TCanvas("c1", "c1", 1600, 1000);
    pureGraph->Draw();
    f_signal->Draw("SAME");
    f_background->Draw("SAME");
}

void pid_check()
{
    nsigma_check();
}
