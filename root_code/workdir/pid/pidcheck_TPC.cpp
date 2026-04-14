#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH2D.h"
#include "TH3D.h"
#include "THnSparse.h"
#include <iostream>


#define DIRNAME "pid-di-hadron_pion"
#define FILE                                                                                                           \
    "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
    "pidTPCTOF_purity_big.root"


void pidcheck_TPC()
{
    TFile *file = TFile::Open(FILE);
    TDirectory *dir_main = (TDirectory *)file->Get(DIRNAME);
    THnSparseD *grh = (THnSparseD *)dir_main->Get("TpcdEdx_ptwise");

    grh->GetAxis(0)->SetRangeUser(1, 3);
    auto grh2D = grh->Projection(2, 1);

    auto c2 = new TCanvas("c2", "c2", 800, 600);
    grh2D->Draw();
    return;

    grh2D->GetYaxis()->SetRangeUser(-1.5, 1.5);
    auto grh1D = grh2D->ProjectionX();

    /// @note double gaus
    double fit_min = 35.0;
    double fit_max = 70.0;

    // 得到图后，开始拟合双高斯
    TF1 *f_double_gaus = new TF1("f_double_gaus", "gaus(0)+gaus(3)", fit_min, fit_max);
    f_double_gaus->SetParameter(0, 9e8); // [0] 幅度（峰值）
    f_double_gaus->SetParameter(1, 52);  // [1] 均值（峰位）
    f_double_gaus->SetParameter(2, 1.5); // [2] sigma（宽度）

    // 第2个高斯：本底峰（π介子等本底，峰位~0，幅度~1e7）
    f_double_gaus->SetParameter(3, 8e8); // [3] 幅度
    f_double_gaus->SetParameter(4, 55);  // [4] 均值
    f_double_gaus->SetParameter(5, 1.5);
    // end double gaus
    grh1D->Fit(f_double_gaus, "RSV");

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


    auto c1 = new TCanvas("c1", "c1", 800, 600);
    grh1D->Draw();
    f_signal->Draw("SAME");
    f_background->Draw("SAME");
}
