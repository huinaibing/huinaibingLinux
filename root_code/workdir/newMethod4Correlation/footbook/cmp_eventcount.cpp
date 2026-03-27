#define BIG_PATH                                                                                                       \
    "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
    "big_valueerror.root"

#define SMALL_PATH                                                                                                     \
    "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
    "small_valueerror.root"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
void cmp_eventcount()
{
    TFile *big = TFile::Open(BIG_PATH);
    TFile *small = TFile::Open(SMALL_PATH);

    TH1D *hcount_big =
        big->Get<TDirectory>("pid-flow-pt-corr_id44937")->Get<TDirectory>("hEventCount")->Get<TH1D>("processData");

    TH1D *hcount_small =
        small->Get<TDirectory>("pid-flow-pt-corr_id44937")->Get<TDirectory>("hEventCount")->Get<TH1D>("processData");

    Double_t norm_big = hcount_big->GetBinContent(1);
    hcount_big->Scale(1.0 / norm_big); // 所有 bin 除以第一个 bin


    Double_t norm_small = hcount_small->GetBinContent(1);
    hcount_small->Scale(1.0 / norm_small);

    TCanvas *c1 = new TCanvas("c1", "Event Count Normalization", 800, 600);

    // 可选：设置网格
    // c1->SetGrid();

    // 2. 设置直方图样式 (区分颜色和线宽)
    hcount_big->SetLineColor(kRed);
    hcount_big->SetLineWidth(2);
    hcount_big->SetTitle("Normalized Event Count"); // X轴/Y轴标题

    hcount_small->SetLineColor(kBlue);
    hcount_small->SetLineWidth(2);

    // 3. 绘制直方图
    hcount_big->Draw("hist");        // "hist" 表示绘制直方图线条
    hcount_small->Draw("hist same"); // "same" 表示在同一张画布上叠加

    // 4. 添加图例 (位置: x1, y1, x2, y2)
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(hcount_big, "Big Sample", "l"); // "l" 表示线条
    leg->AddEntry(hcount_small, "Small Sample", "l");
    leg->Draw();
}
