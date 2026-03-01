#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "FlowContainer.h"
#include <vector>
#include "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/WorkDirUtils/Utils4V2pTCorr.h"

#define FILE "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_new_cut_small_errornua.root"

#define BLOCK1 // event count check
#define BLOCK2 // phi correction check
#define BLOCK3 // pt spectrum and efficiency check
#define BLOCK4 // v2{2} and v2{4} for charged
#define BLOCK5 // v2{2} cmparison
#define BLOCK6 // diff pt v2 comparison

namespace lambdaFunction
{
    TH1D *get_v22_primeprime(TProfile2D *, TProfile2D *);
    TH1D *get_v22_prime(TProfile2D *, TProfile2D *);
}

namespace qaUtil4Fc
{
    void drawFlowV2Comparison(const TString &particleName, const TString &particleLatex,
                              const TString &ptBin, const TString &ptRange,
                              FlowContainerManager &fcm)
    {
        // 获取两个v2直方图
        TH1D *v22_primeprime = fcm.getHistFrom2FC(lambdaFunction::get_v22_primeprime);
        TH1D *v22_prime = fcm.getHistFrom2FC(lambdaFunction::get_v22_prime);

        // 创建画布（动态拼接ptBin，避免不同pt区间画布重名）
        TCanvas *c = new TCanvas(Form("c_%s_%s", particleName.Data(), ptBin.Data()),
                                 Form("v2 comparation for %s pt %s", particleName.Data(), ptBin.Data()),
                                 800, 600);

        // 设置直方图样式（统一风格，pt区间动态显示在标题中）
        v22_primeprime->SetLineColor(kRed);
        v22_prime->SetLineColor(kBlue);
        v22_primeprime->SetTitle(Form("v_{2} comparation, %s, %s GeV/c;Centrality (%%);v_{2}",
                                      particleLatex.Data(), ptRange.Data()));
        v22_primeprime->SetStats(0); // 关闭统计框

        // 绘制直方图
        v22_primeprime->Draw();
        v22_prime->Draw("SAME");

        // 创建并绘制图例
        TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.8);
        leg->AddEntry(v22_primeprime, "v_{2}\"{2}");
        leg->AddEntry(v22_prime, "v_{2}\'{2}");
        leg->Draw("SAME");

        // 2. 计算 v22_primeprime / v22_prime 比值图
        // 克隆v22_primeprime作为比值图载体（避免修改原直方图数据）
        TH1D *v22_ratio = (TH1D *)v22_prime->Clone(Form("v22_ratio_%s_%s", particleName.Data(), ptBin.Data()));
        // 逐元相除：Divide(分母直方图, 误差处理模式)
        // kDivideByBinContent：按bin内容相除；kSameErrors：保持原直方图误差（适合对比场景）
        for (int i = 1; i < v22_ratio->GetNbinsX(); i++)
        {
            double denom = v22_primeprime->GetBinContent(i);
            if (denom != 0)
            {
                double ratio = v22_ratio->GetBinContent(i) / denom;
                v22_ratio->SetBinContent(i, ratio);
                // 误差计算：假设原误差为σ1和σ2，使用误差传播公式计算比值误差
                double err1 = v22_ratio->GetBinError(i);
                double err2 = v22_primeprime->GetBinError(i);
                double ratio_err = ratio * TMath::Sqrt((err1 / v22_ratio->GetBinContent(i)) * (err1 / v22_ratio->GetBinContent(i)) +
                                                       (err2 / denom) * (err2 / denom));
                v22_ratio->SetBinError(i, ratio_err);
            }
        }
        TCanvas *c_ratio = new TCanvas(Form("c_ratio_%s_%s", particleName.Data(), ptBin.Data()),
                                       Form("v_{2}\'{2} / v_{2}\"{2} for %s pt %s", particleName.Data(), ptBin.Data()),
                                       800, 600);
        v22_ratio->SetTitle(Form("v_{2}\'{2} / v_{2}\"{2}, %s, %s GeV/c;Centrality (%%);Ratio", particleLatex.Data(), ptRange.Data()));
        v22_ratio->SetStats(0);
        v22_ratio->GetYaxis()->SetRangeUser(0.8, 1.2); // 设置y轴范围便于观察偏离1的情况
        v22_ratio->Draw();
    }

    /**
     * @brief Get the v22 v24 object
     *
     * @param fc_ch
     * @param option IMPORTANT, if opt != 1, the v22 is v22pure for pid, v24 is meaningless
     * @return std::vector<TH1D *>
     */
    std::vector<TH1D *> get_v22_v24(FlowContainer *fc_ch, int option = 1)
    {
        TProfile2D *h_prof_ch = fc_ch->GetProfile();

        TProfile *h_c22 = 0;
        if (option == 1)
        {
            h_c22 = h_prof_ch->ProfileX("c22", 1, 1);
        }
        else
        {
            h_c22 = h_prof_ch->ProfileX("c22", 16, 16);
        }
        TProfile *h_c24 = h_prof_ch->ProfileX("c24", 3, 3);

        TH1D *v22 = new TH1D("v22", "v2{2} for charged;Centrality (%);v2{2}", h_c22->GetNbinsX(), h_c22->GetXaxis()->GetXbins()->GetArray());
        TH1D *v24 = new TH1D("v24", "v2{4} for charged;Centrality (%);v2{4}", h_c24->GetNbinsX(), h_c24->GetXaxis()->GetXbins()->GetArray());

        // fill v2{2} and v2{4}
        for (int i = 1; i <= h_c22->GetNbinsX(); i++)
        {
            double c22 = h_c22->GetBinContent(i);
            if (c22 < 0)
            {
                v22->SetBinContent(i, 0);
            }
            else
            {
                v22->SetBinContent(i, TMath::Sqrt(c22));
            }

            double c24 = h_c24->GetBinContent(i);
            if (2 * c22 * c22 - c24 < 0)
            {
                v24->SetBinContent(i, 0);
            }
            else
            {
                v24->SetBinContent(i, TMath::Sqrt(TMath::Sqrt(2 * c22 * c22 - c24)));
            }
        }
        // end fill v2{2} and v2{4}

        // calculate error for v2{2} and v2{4}
        std::vector<double> error4eachbin;
        std::vector<double> error4eachbin_v24;
        for (int i = 1; i <= v22->GetNbinsX(); i++)
        {
            error4eachbin.push_back(0.0);
            error4eachbin_v24.push_back(0.0);
        }
        TObjArray *arr = fc_ch->GetSubProfiles();
        int nsub = arr->GetEntriesFast();

        // loop all subevents
        for (int i = 0; i < nsub; i++)
        {
            TProfile2D *h_sub_prof = (TProfile2D *)arr->At(i);
            TProfile *h_sub_c22 = 0;
            if (option == 1)
            {
                h_sub_c22 = h_sub_prof->ProfileX("sub_c22", 1, 1);
            }
            else
            {
                h_sub_c22 = h_sub_prof->ProfileX("sub_c22", 16, 16);
            }
            TProfile *h_sub_c24 = h_sub_prof->ProfileX("sub_c24", 3, 3);
            for (int j = 1; j <= h_sub_c22->GetNbinsX(); j++)
            {
                double c22_sub = h_sub_c22->GetBinContent(j);
                double c24_sub = h_sub_c24->GetBinContent(j);

                double v2_sub = 0.0;
                if (c22_sub >= 0)
                {
                    v2_sub = TMath::Sqrt(c22_sub);
                }
                double diff = v2_sub - v22->GetBinContent(j);
                error4eachbin[j - 1] += diff * diff;

                double v4_sub = 0.0;
                if (2 * c22_sub * c22_sub - c24_sub >= 0)
                {
                    v4_sub = TMath::Sqrt(TMath::Sqrt(2 * c22_sub * c22_sub - c24_sub));
                }
                double diff4 = v4_sub - v24->GetBinContent(j);
                error4eachbin_v24[j - 1] += diff4 * diff4;
            }
        }
        // end loop all subevents
        for (int i = 0; i < error4eachbin.size(); i++)
        {
            double stddev = TMath::Sqrt(error4eachbin[i] / (nsub - 1));
            v22->SetBinError(i + 1, stddev);

            double stddev4 = TMath::Sqrt(error4eachbin_v24[i] / (nsub - 1));
            v24->SetBinError(i + 1, stddev4);
        }
        // end calculate error for v2{2}

        return {v22, v24};
    }
}

namespace lambdaFunction
{
    TH1D *get_v22_primeprime(TProfile2D *, TProfile2D *prof2d)
    {
        TProfile *prof = prof2d->ProfileX("c22pure", FCGraphName::c22pure, FCGraphName::c22pure);
        TH1D *hres = new TH1D("1", "", prof->GetNbinsX(), prof->GetXaxis()->GetXbins()->GetArray());
        for (int i = 1; i <= prof->GetNbinsX(); i++)
        {
            double res = prof->GetBinContent(i);
            hres->SetBinContent(i, res > 0 ? TMath::Sqrt(res) : 0);
        }
        return hres;
    }

    TH1D *get_v22_prime(TProfile2D *prof2d_ch, TProfile2D *prof2d_pid)
    {
        TProfile *prof_ch = prof2d_ch->ProfileX("c22ch", FCGraphName::c22, FCGraphName::c22);
        TProfile *prof_pid_ch = prof2d_pid->ProfileX("c22chpid", FCGraphName::c22, FCGraphName::c22);
        TH1D *hres = new TH1D("1", "", prof_ch->GetNbinsX(), prof_ch->GetXaxis()->GetXbins()->GetArray());

        for (int i = 1; i <= prof_ch->GetNbinsX(); i++)
        {
            double data_ch = prof_ch->GetBinContent(i);
            double data_pid = prof_pid_ch->GetBinContent(i);
            if (data_ch < 0)
            {
                hres->SetBinContent(i, 0);
            }
            hres->SetBinContent(i, data_pid / TMath::Sqrt(data_ch));
        }
        return hres;
    }
}

void QA()
{
    // init
    TFile *f = TFile::Open(FILE);
    TDirectory *dir_main = (TDirectory *)f->Get("pid-flow-pt-corr");
    FlowContainer *fc_ch = (FlowContainer *)dir_main->Get("FlowContainerCharged");
    FlowContainer *fc_pi = (FlowContainer *)dir_main->Get("FlowContainerPi");
    FlowContainer *fc_ka = (FlowContainer *)dir_main->Get("FlowContainerKa");
    FlowContainer *fc_pr = (FlowContainer *)dir_main->Get("FlowContainerPr");
    // end init

    /**
     * @brief block 1
     * @note check event count, note that need to divide a num(deside by how many function)\
     * @note due to code modification, need to implement a new one
     */
    {
#ifdef BLOCK1
#endif
    }
    // end block 1

    /**
     * @brief block 2
     * @note phi correction
     */
    {
#ifdef BLOCK2
        const double rangeMin = 160e6;
        const double rangeMax = 220e6;
        TH1D *h_phi = (TH1D *)dir_main->Get("hPhi");
        TH1D *h_phi_corr = (TH1D *)dir_main->Get("hPhicorr");
        TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
        h_phi->SetLineColor(kRed);
        h_phi->GetXaxis()->SetTitle("#phi (rad)");
        h_phi->GetYaxis()->SetTitle("Counts");
        h_phi->SetStats(0);
        h_phi->GetYaxis()->SetRangeUser(rangeMin, rangeMax);
        h_phi->Draw();
        h_phi_corr->SetLineColor(kBlue);
        h_phi_corr->Draw("SAME");
        TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
        leg->AddEntry(h_phi, "before correction");
        leg->AddEntry(h_phi_corr, "after correction");
        leg->Draw("SAME");
#endif
    }
    // end block 2

    /**
     * @brief block 3
     * @note pt spectrum and efficiency
     */
    {
#ifdef BLOCK3
        const char *ptEffFile = "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/corrections/Eff.root";
        TFile *f_eff = TFile::Open(ptEffFile);
        TH1D *h_eff = (TH1D *)f_eff->Get("ccdb_object");
        TH1D *h_pt = (TH1D *)dir_main->Get("hPt");
        h_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_pt->GetYaxis()->SetTitle("Counts");
        h_eff->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_eff->GetYaxis()->SetTitle("Efficiency");
        TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);
        h_eff->Draw();
        TCanvas *c4 = new TCanvas("c4", "c4", 800, 600);
        h_pt->Draw();
#endif
    }
    // end block 3

    /**
     * @brief block 4
     * @note v2{2} and v2{4} for charged
     */
    {
#ifdef BLOCK4
        std::vector<TH1D *> v2s_ch = qaUtil4Fc::get_v22_v24(fc_ch);
        TFile *file_run2 = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/run2v2cent.root");
        TDirectory *dir_run2 = (TDirectory *)file_run2->Get("Table 1");
        TH1D *h_run2 = (TH1D *)dir_run2->Get("Hist1D_y1");
        for (int i = 1; i <= h_run2->GetNbinsX(); i++)
        {
            h_run2->SetBinError(i, 0.00001);
        }

        TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);
        v2s_ch[0]->SetTitle("v_{2} for charged;Centrality (%);v_{2}");
        v2s_ch[0]->SetStats(0);
        v2s_ch[0]->GetYaxis()->SetRangeUser(0, 0.12);
        v2s_ch[0]->Draw();
        v2s_ch[1]->SetLineColor(kRed);
        // v2s_ch[1]->Draw("SAME");
        h_run2->SetLineColor(kGreen);
        h_run2->Draw("same");
        TLegend *leg1 = new TLegend(0.6, 0.7, 0.8, 0.8);
        leg1->AddEntry(v2s_ch[0], "v_{2}{2}");
        // leg1->AddEntry(v2s_ch[1], "v_{2}{4}");
        leg1->AddEntry(h_run2, "run2 v_{2}{2}");
        leg1->Draw("SAME");
#endif
    }
    // end block 4

    /**
     * @brief block 5
     * @note compare <<2'>> and <<2''>>
     */
    {
#ifdef BLOCK5
        // ==================== 1. π（pi）粒子绘图（原有代码，优化命名和注释） ====================
        FlowContainerManager fcm_pi(fc_ch, fc_pi); // pi粒子的FlowContainerManager
        TH1D *v22_primeprime_pi = fcm_pi.getHistFrom2FC(lambdaFunction::get_v22_primeprime);
        TH1D *v22_prime_pi = fcm_pi.getHistFrom2FC(lambdaFunction::get_v22_prime);

        TCanvas *c_pi = new TCanvas("c_pi", "v2 comparation for pi", 800, 600); // 重命名画布，更易识别
        v22_primeprime_pi->SetLineColor(kRed);
        v22_prime_pi->SetLineColor(kBlue);
        v22_primeprime_pi->SetTitle("v_{2} comparation, #pi;Centrality (%);v_{2}");
        v22_primeprime_pi->SetStats(0); // 关闭统计框
        v22_primeprime_pi->Draw();
        v22_prime_pi->Draw("SAME");

        TLegend *leg_pi = new TLegend(0.6, 0.7, 0.8, 0.8);
        leg_pi->AddEntry(v22_primeprime_pi, "v_{2}\'{2}");
        leg_pi->AddEntry(v22_prime_pi, "v_{2}\"{2}");
        leg_pi->Draw("SAME");

        // ==================== 2. Ka（K粒子）绘图 ====================
        FlowContainerManager fcm_ka(fc_ch, fc_ka); // ka粒子的FlowContainerManager（需确保fc_ka已定义）
        TH1D *v22_primeprime_ka = fcm_ka.getHistFrom2FC(lambdaFunction::get_v22_primeprime);
        TH1D *v22_prime_ka = fcm_ka.getHistFrom2FC(lambdaFunction::get_v22_prime);

        TCanvas *c_ka = new TCanvas("c_ka", "v2 comparation for ka", 800, 600);
        v22_primeprime_ka->SetLineColor(kGreen);                                  // 用绿色区分ka粒子的v22_primeprime
        v22_prime_ka->SetLineColor(kMagenta);                                     // 用品红区分ka粒子的v22_prime
        v22_primeprime_ka->SetTitle("v_{2} comparation, K;Centrality (%);v_{2}"); // 标题改为K粒子
        v22_primeprime_ka->SetStats(0);
        v22_primeprime_ka->Draw();
        v22_prime_ka->Draw("SAME");

        TLegend *leg_ka = new TLegend(0.6, 0.7, 0.8, 0.8);
        leg_ka->AddEntry(v22_primeprime_ka, "v_{2}\'{2}");
        leg_ka->AddEntry(v22_prime_ka, "v_{2}\"{2}");
        leg_ka->Draw("SAME");

        // ==================== 3. Pr（质子）绘图 ====================
        FlowContainerManager fcm_pr(fc_ch, fc_pr); // pr粒子的FlowContainerManager（需确保fc_pr已定义）
        TH1D *v22_primeprime_pr = fcm_pr.getHistFrom2FC(lambdaFunction::get_v22_primeprime);
        TH1D *v22_prime_pr = fcm_pr.getHistFrom2FC(lambdaFunction::get_v22_prime);

        TCanvas *c_pr = new TCanvas("c_pr", "v2 comparation for pr", 800, 600);
        v22_primeprime_pr->SetLineColor(kOrange);                                 // 用橙色区分pr粒子的v22_primeprime
        v22_prime_pr->SetLineColor(kCyan);                                        // 用青色区分pr粒子的v22_prime
        v22_primeprime_pr->SetTitle("v_{2} comparation, p;Centrality (%);v_{2}"); // 标题改为质子p
        v22_primeprime_pr->SetStats(0);
        v22_primeprime_pr->Draw();
        v22_prime_pr->Draw("SAME");

        TLegend *leg_pr = new TLegend(0.6, 0.7, 0.8, 0.8);
        leg_pr->AddEntry(v22_primeprime_pr, "v_{2}\'{2}");
        leg_pr->AddEntry(v22_prime_pr, "v_{2}\"{2}");
        leg_pr->Draw("SAME");
#endif
    }
    // end block 5

    /**
     * @brief block 6
     * @note compare pt diff for v22' and v22''
     */
    {
#ifdef BLOCK6
        const char *main_file = "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/pt_diff_close_its.root";
        TFile *f_main = TFile::Open(main_file);
        {
            TDirectory *dir_02to05 = (TDirectory *)f_main->Get("pid-flow-pt-corr_pt02to05_id44937");
            FlowContainer *fc_ch_02to05 = (FlowContainer *)dir_02to05->Get("FlowContainerCharged");
            FlowContainer *fc_pi_02to05 = (FlowContainer *)dir_02to05->Get("FlowContainerPi");
            FlowContainer *fc_ka_02to05 = (FlowContainer *)dir_02to05->Get("FlowContainerKa");
            FlowContainer *fc_pr_02to05 = (FlowContainer *)dir_02to05->Get("FlowContainerPr");

            const TString pt_bin_02to05 = "02to05";
            const TString pt_range_02to05 = "0.2 < p_{T} < 0.5";

            FlowContainerManager fcm_pi_02to05(fc_ch_02to05, fc_pi_02to05);
            qaUtil4Fc::drawFlowV2Comparison("pi", "#pi", pt_bin_02to05, pt_range_02to05, fcm_pi_02to05);

            FlowContainerManager fcm_ka_02to05(fc_ch_02to05, fc_ka_02to05);
            qaUtil4Fc::drawFlowV2Comparison("ka", "K", pt_bin_02to05, pt_range_02to05, fcm_ka_02to05);

            FlowContainerManager fcm_pr_02to05(fc_ch_02to05, fc_pr_02to05);
            qaUtil4Fc::drawFlowV2Comparison("pr", "p", pt_bin_02to05, pt_range_02to05, fcm_pr_02to05);
        }

        {
            TDirectory *dir_05to10 = (TDirectory *)f_main->Get("pid-flow-pt-corr_pt05to10_id44937");
            FlowContainer *fc_ch_05to10 = (FlowContainer *)dir_05to10->Get("FlowContainerCharged");
            FlowContainer *fc_pi_05to10 = (FlowContainer *)dir_05to10->Get("FlowContainerPi");
            FlowContainer *fc_ka_05to10 = (FlowContainer *)dir_05to10->Get("FlowContainerKa");
            FlowContainer *fc_pr_05to10 = (FlowContainer *)dir_05to10->Get("FlowContainerPr");

            const TString pt_bin_05to10 = "05to10";
            const TString pt_range_05to10 = "0.5 < p_{T} < 1.0";

            FlowContainerManager fcm_pi_05to10(fc_ch_05to10, fc_pi_05to10);
            qaUtil4Fc::drawFlowV2Comparison("pi", "#pi", pt_bin_05to10, pt_range_05to10, fcm_pi_05to10);

            FlowContainerManager fcm_ka_05to10(fc_ch_05to10, fc_ka_05to10);
            qaUtil4Fc::drawFlowV2Comparison("ka", "K", pt_bin_05to10, pt_range_05to10, fcm_ka_05to10);

            FlowContainerManager fcm_pr_05to10(fc_ch_05to10, fc_pr_05to10);
            qaUtil4Fc::drawFlowV2Comparison("pr", "p", pt_bin_05to10, pt_range_05to10, fcm_pr_05to10);
        }

        {
            TDirectory *dir_10to15 = (TDirectory *)f_main->Get("pid-flow-pt-corr_pt10to15_id44937");
            FlowContainer *fc_ch_10to15 = (FlowContainer *)dir_10to15->Get("FlowContainerCharged");
            FlowContainer *fc_pi_10to15 = (FlowContainer *)dir_10to15->Get("FlowContainerPi");
            FlowContainer *fc_ka_10to15 = (FlowContainer *)dir_10to15->Get("FlowContainerKa");
            FlowContainer *fc_pr_10to15 = (FlowContainer *)dir_10to15->Get("FlowContainerPr");

            const TString pt_bin_10to15 = "10to15";
            const TString pt_range_10to15 = "1.0 < p_{T} < 1.5";

            FlowContainerManager fcm_pi_10to15(fc_ch_10to15, fc_pi_10to15);
            qaUtil4Fc::drawFlowV2Comparison("pi", "#pi", pt_bin_10to15, pt_range_10to15, fcm_pi_10to15);

            FlowContainerManager fcm_ka_10to15(fc_ch_10to15, fc_ka_10to15);
            qaUtil4Fc::drawFlowV2Comparison("ka", "K", pt_bin_10to15, pt_range_10to15, fcm_ka_10to15);

            FlowContainerManager fcm_pr_10to15(fc_ch_10to15, fc_pr_10to15);
            qaUtil4Fc::drawFlowV2Comparison("pr", "p", pt_bin_10to15, pt_range_10to15, fcm_pr_10to15);
        }

        {
            TDirectory *dir_15to20 = (TDirectory *)f_main->Get("pid-flow-pt-corr_pt15to20_id44937");
            FlowContainer *fc_ch_15to20 = (FlowContainer *)dir_15to20->Get("FlowContainerCharged");
            FlowContainer *fc_pi_15to20 = (FlowContainer *)dir_15to20->Get("FlowContainerPi");
            FlowContainer *fc_ka_15to20 = (FlowContainer *)dir_15to20->Get("FlowContainerKa");
            FlowContainer *fc_pr_15to20 = (FlowContainer *)dir_15to20->Get("FlowContainerPr");

            const TString pt_bin_15to20 = "15to20";
            const TString pt_range_15to20 = "1.5 < p_{T} < 2.0";

            FlowContainerManager fcm_pi_15to20(fc_ch_15to20, fc_pi_15to20);
            qaUtil4Fc::drawFlowV2Comparison("pi", "#pi", pt_bin_15to20, pt_range_15to20, fcm_pi_15to20);

            FlowContainerManager fcm_ka_15to20(fc_ch_15to20, fc_ka_15to20);
            qaUtil4Fc::drawFlowV2Comparison("ka", "K", pt_bin_15to20, pt_range_15to20, fcm_ka_15to20);

            FlowContainerManager fcm_pr_15to20(fc_ch_15to20, fc_pr_15to20);
            qaUtil4Fc::drawFlowV2Comparison("pr", "p", pt_bin_15to20, pt_range_15to20, fcm_pr_15to20);
        }

        {
            TDirectory *dir_25to30 = (TDirectory *)f_main->Get("pid-flow-pt-corr_pt25to30_id44937");
            FlowContainer *fc_ch_25to30 = (FlowContainer *)dir_25to30->Get("FlowContainerCharged");
            FlowContainer *fc_pi_25to30 = (FlowContainer *)dir_25to30->Get("FlowContainerPi");
            FlowContainer *fc_ka_25to30 = (FlowContainer *)dir_25to30->Get("FlowContainerKa");
            FlowContainer *fc_pr_25to30 = (FlowContainer *)dir_25to30->Get("FlowContainerPr");

            const TString pt_bin_25to30 = "25to30";
            const TString pt_range_25to30 = "2.5 < p_{T} < 3.0";

            FlowContainerManager fcm_pi_25to30(fc_ch_25to30, fc_pi_25to30);
            qaUtil4Fc::drawFlowV2Comparison("pi", "#pi", pt_bin_25to30, pt_range_25to30, fcm_pi_25to30);

            FlowContainerManager fcm_ka_25to30(fc_ch_25to30, fc_ka_25to30);
            qaUtil4Fc::drawFlowV2Comparison("ka", "K", pt_bin_25to30, pt_range_25to30, fcm_ka_25to30);

            FlowContainerManager fcm_pr_25to30(fc_ch_25to30, fc_pr_25to30);
            qaUtil4Fc::drawFlowV2Comparison("pr", "p", pt_bin_25to30, pt_range_25to30, fcm_pr_25to30);
        }

        {
            TDirectory *dir_30to40 = (TDirectory *)f_main->Get("pid-flow-pt-corr_pt30to40_id44937");
            FlowContainer *fc_ch_30to40 = (FlowContainer *)dir_30to40->Get("FlowContainerCharged");
            FlowContainer *fc_pi_30to40 = (FlowContainer *)dir_30to40->Get("FlowContainerPi");
            FlowContainer *fc_ka_30to40 = (FlowContainer *)dir_30to40->Get("FlowContainerKa");
            FlowContainer *fc_pr_30to40 = (FlowContainer *)dir_30to40->Get("FlowContainerPr");

            const TString pt_bin_30to40 = "30to40";
            const TString pt_range_30to40 = "3.0 < p_{T} < 4.0";

            FlowContainerManager fcm_pi_30to40(fc_ch_30to40, fc_pi_30to40);
            qaUtil4Fc::drawFlowV2Comparison("pi", "#pi", pt_bin_30to40, pt_range_30to40, fcm_pi_30to40);

            FlowContainerManager fcm_ka_30to40(fc_ch_30to40, fc_ka_30to40);
            qaUtil4Fc::drawFlowV2Comparison("ka", "K", pt_bin_30to40, pt_range_30to40, fcm_ka_30to40);

            FlowContainerManager fcm_pr_30to40(fc_ch_30to40, fc_pr_30to40);
            qaUtil4Fc::drawFlowV2Comparison("pr", "p", pt_bin_30to40, pt_range_30to40, fcm_pr_30to40);
        }
#endif
    }
    // end block 6
}