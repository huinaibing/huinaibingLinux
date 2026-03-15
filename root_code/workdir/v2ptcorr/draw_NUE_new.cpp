#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include "TLegend.h"
#include "TH2.h"
#include <iostream>

#define MC_PATH "/home/huinaibing/Documents/datas4o2/AnalysisResults.root"

void draw_NUE_new()
{
    // open file and dir (保持不变)
    TFile *file = TFile::Open(MC_PATH);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    // end open file and dir

    // ------------------ 修改开始：定义粒子类型并循环处理 ------------------
    // 1. 定义粒子标识数组：pi(π)、ka(K)、pr(p)
    const char *particleTypes[] = {"Pi", "Ka", "Pr"};
    int nParticles = sizeof(particleTypes) / sizeof(particleTypes[0]);

    // 2. 循环处理每一种粒子
    for (int i = 0; i < nParticles; ++i)
    {
        const char *part = particleTypes[i];

        // 3. 动态构建直方图名称（假设你的直方图命名规则是 hPtCentMcGenPi / hPtCentMcRecPi）
        TString histGenName = Form("hPtCentMcGen%s", part); // 例如：hPtCentMcGenpi
        TString histRecName = Form("hPtCentMcRec%s", part); // 例如：hPtCentMcRecpi

        // 4. 获取对应粒子的MC生成和重建二维直方图
        TH2D *hMcCentPt_2D = (TH2D *)correction->Get(histGenName);
        TH2D *hDataCentPt_2D = (TH2D *)correction->Get(histRecName);

        // 安全检查：确保二维直方图成功读取
        if (!hMcCentPt_2D || !hDataCentPt_2D)
        {
            Error("EfficiencyCalc", "Histogram for %s not found! Skipping...", part);
            continue;
        }

        // ------------------ 核心修改：2D投影到1D (对y轴积分) ------------------
        // 5. 将2D直方图投影到x轴 (默认对整个y轴范围积分)，得到1D的pT谱
        TH1D *hMcPt_1D = (TH1D *)hMcCentPt_2D->ProjectionX(Form("hMcPt_%s_1D", part));
        TH1D *hDataPt_1D = (TH1D *)hDataCentPt_2D->ProjectionX(Form("hDataPt_%s_1D", part));

        // 6. 计算1D效率 (Reco_1D / Gen_1D)
        TH1D *hEfficiency_1D = (TH1D *)hDataPt_1D->Clone(Form("ccdb_object_%s", part));
        hEfficiency_1D->Divide(hMcPt_1D);

        // 7. 绘制1D效率图
        TCanvas *c = new TCanvas(Form("c_%s", part), Form("Efficiency_%s", part), 1800, 1000);
        hEfficiency_1D->SetMarkerStyle(kFullCircle);
        hEfficiency_1D->SetMarkerColor(kBlack);
        hEfficiency_1D->SetLineColor(kBlack);
        hEfficiency_1D->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        hEfficiency_1D->GetYaxis()->SetTitle("Efficiency");
        hEfficiency_1D->Draw("EP"); // "EP" 表示带误差棒的点图

        // 8. 保存1D效率图到root文件
        TString outFileName = Form("corrections/NUE1D_%s.root", part);
        TFile *file_efficiency = new TFile(outFileName, "RECREATE");
        hEfficiency_1D->SetName("ccdb_object");
        hEfficiency_1D->Write();
        file_efficiency->Close();

        // 可选：清理内存
        delete c;
        delete hEfficiency_1D;
        delete hMcPt_1D;
        delete hDataPt_1D;
    }

    // 关闭原始输入文件
    file->Close();
    return;
}