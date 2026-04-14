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
// #define FILE                                                                                                           \
//     "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
//     "pid_purity_OO.root"

void pidcheck_TOF_pi()
{
    // init
    TFile *file = TFile::Open(FILE);
    TDirectory *dir_main = (TDirectory *)file->Get(DIRNAME);
    THnSparseD *grh = (THnSparseD *)dir_main->Get("TofTpcNsigma_before");
    // end init

    grh->GetAxis(0)->SetTitle("TPC");
    grh->GetAxis(1)->SetTitle("TOF");

    grh->GetAxis(2)->SetRangeUser(1.5, 2);
    auto grh2D = grh->Projection(1, 0);

    // grh2D->GetYaxis()->SetRangeUser(-1.5, 1.5);

    auto grh1D = grh2D->ProjectionX();
    grh1D->Sumw2();
    grh1D->SetMarkerStyle(22);
    grh1D->SetMarkerColor(kOrange + 2);
    auto c2 = new TCanvas("c2", "c2", 800, 600);
    grh2D->Draw();
    return;


    /// @note 完全自由三双高斯模型（每个峰的窄/宽分量均独立）
    double fit_min = -6.0;
    double fit_max = 20.0;

    // 定义拟合函数（共18个参数）：
    // [0-5]:   主峰双高斯 (窄[0-2] + 宽[3-5])
    // [6-11]:  K峰双高斯  (窄[6-8] + 宽[9-11])
    // [12-17]: 质子峰双高斯(窄[12-14] + 宽[15-17])
    TF1 *f_total_fit = new TF1("f_total_fit",
                               // 主峰双高斯（完全独立）
                               "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[4])/[5])^2) + "
                               // K峰双高斯（完全独立）
                               "[6]*exp(-0.5*((x-[7])/[8])^2) + [9]*exp(-0.5*((x-[10])/[11])^2) + "
                               // 质子峰双高斯（完全独立）
                               "[12]*exp(-0.5*((x-[13])/[14])^2) + [15]*exp(-0.5*((x-[16])/[17])^2)",
                               fit_min,
                               fit_max);
    f_total_fit->SetLineColor(kBlack);
    f_total_fit->SetNpx(3000);

    // ========== 设置初始参数 ==========
    // --- 1. 主峰双高斯（π介子） ---
    f_total_fit->SetParameter(0, 2.8e7); // 窄核心幅度
    f_total_fit->SetParameter(1, 0.0);   // 窄核心均值
    f_total_fit->SetParameter(2, 1.0);   // 窄核心sigma
    f_total_fit->SetParameter(3, 0.8e7); // 宽尾巴幅度
    f_total_fit->SetParameter(4, 0.2);   // 宽尾巴均值（允许略微偏移）
    f_total_fit->SetParameter(5, 2.2);   // 宽尾巴sigma

    // --- 2. K峰双高斯 ---
    f_total_fit->SetParameter(6, 0.9e7); // 窄核心幅度
    f_total_fit->SetParameter(7, 3.75);  // 窄核心均值
    f_total_fit->SetParameter(8, 1.0);   // 窄核心sigma
    f_total_fit->SetParameter(9, 0.3e7); // 宽尾巴幅度
    f_total_fit->SetParameter(10, 3.9);  // 宽尾巴均值（允许略微偏移）
    f_total_fit->SetParameter(11, 1.8);  // 宽尾巴sigma

    // --- 3. 质子峰双高斯 ---
    f_total_fit->SetParameter(12, 3.2e6); // 窄核心幅度
    f_total_fit->SetParameter(13, 14.2);  // 窄核心均值
    f_total_fit->SetParameter(14, 2.8);   // 窄核心sigma
    f_total_fit->SetParameter(15, 1.0e6); // 宽尾巴幅度
    f_total_fit->SetParameter(16, 14.5);  // 宽尾巴均值（允许略微偏移）
    f_total_fit->SetParameter(17, 4.2);   // 宽尾巴sigma

    // 执行一次性拟合
    grh1D->Fit(f_total_fit, "IL");

    // ========== 提取拟合参数 ==========
    double p0 = f_total_fit->GetParameter(0);
    double p1 = f_total_fit->GetParameter(1);
    double p2 = f_total_fit->GetParameter(2);
    double p3 = f_total_fit->GetParameter(3);
    double p4 = f_total_fit->GetParameter(4);
    double p5 = f_total_fit->GetParameter(5);

    double p6 = f_total_fit->GetParameter(6);
    double p7 = f_total_fit->GetParameter(7);
    double p8 = f_total_fit->GetParameter(8);
    double p9 = f_total_fit->GetParameter(9);
    double p10 = f_total_fit->GetParameter(10);
    double p11 = f_total_fit->GetParameter(11);

    double p12 = f_total_fit->GetParameter(12);
    double p13 = f_total_fit->GetParameter(13);
    double p14 = f_total_fit->GetParameter(14);
    double p15 = f_total_fit->GetParameter(15);
    double p16 = f_total_fit->GetParameter(16);
    double p17 = f_total_fit->GetParameter(17);

    // ========== 定义单独分量用于绘图（均合并窄+宽） ==========
    // 1. 总主峰（深绿虚线）
    TF1 *f_gaus_pi =
        new TF1("f_gaus_pi", "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[4])/[5])^2)", fit_min, fit_max);
    f_gaus_pi->SetParameters(p0, p1, p2, p3, p4, p5);
    f_gaus_pi->SetLineColor(kGreen + 2);
    f_gaus_pi->SetLineWidth(3);
    f_gaus_pi->SetLineStyle(2);

    // 2. 总K峰（红色虚线）
    TF1 *f_gaus_k =
        new TF1("f_gaus_k", "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[4])/[5])^2)", fit_min, fit_max);
    f_gaus_k->SetParameters(p6, p7, p8, p9, p10, p11);
    f_gaus_k->SetLineColor(kRed);
    f_gaus_k->SetLineWidth(3);
    f_gaus_k->SetLineStyle(2);

    // 3. 总质子峰（蓝色虚线）
    TF1 *f_gaus_p =
        new TF1("f_gaus_p", "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[4])/[5])^2)", fit_min, fit_max);
    f_gaus_p->SetParameters(p12, p13, p14, p15, p16, p17);
    f_gaus_p->SetLineColor(kBlue);
    f_gaus_p->SetLineWidth(3);
    f_gaus_p->SetLineStyle(2);

    // ========== 绘图 ==========
    auto c1 = new TCanvas("c1", "TOF nsigma Fit (Full Free Double Gaus)", 800, 600);
    grh1D->Draw();
    f_total_fit->Draw("SAME"); // 总拟合曲线（黑色实线）
    f_gaus_pi->Draw("SAME");   // 总主峰（深绿虚线）
    f_gaus_k->Draw("SAME");    // 总K峰（红色虚线）
    f_gaus_p->Draw("SAME");    // 总质子峰（蓝色虚线）
}
