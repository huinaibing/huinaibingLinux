#include "FlowContainer.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include <iostream>

#define FILE "/home/huinaibing/Documents/datas4o2/AnalysisResults.root"

// #define DEBUG


double get_cov_part1(int centBin)
{
    TFile *f = TFile::Open(FILE);
    TDirectory *dir = (TDirectory *)f->Get("pid-flow-pt-corr");

    TProfile3D *proOrigin = dir->Get<TDirectory>("meanptCentNbs")->Get<TProfile3D>("hCharged");

    TProfile *proMerged = new TProfile("proMerged", "proMerged", 300, 0, 3);
    // merge 成一个图
    for (int idxPt = 1; idxPt <= proOrigin->GetNbinsX(); idxPt++)
    {
        double weightValue = 0;
        double sumWeight = 0;
        for (int idxNbs = 1; idxNbs <= proOrigin->GetNbinsZ(); idxNbs++)
        {
            double weight = proOrigin->GetBinEntries(proOrigin->GetBin(idxPt, centBin, idxNbs));
            if (weight == 0)
            {
                continue;
            }

            weightValue += proOrigin->GetBinContent(idxPt, centBin, idxNbs) * weight;
            sumWeight += weight;
        }

        if (sumWeight == 0)
        {
            continue;
        }
        // std::cout << idxPt << " " << sumWeight << std::endl;
        proMerged->SetBinContent(idxPt, weightValue);
        proMerged->SetBinEntries(idxPt, sumWeight);

        //std::cout << weightValue / sumWeight << " " << sumWeight << std::endl;
    }
    // return proMerged;

    // std::cout << "===================================" << std::endl;


    double cov = 0;
    double point = 0;

    for (int i = 1; i <= proMerged->GetNbinsX(); i++)
    {
        double weight = proMerged->GetBinEntries(i);

        if (weight == 0)
        {
            continue;
        }
        // std::cout << i << " " << weight << std::endl;
        double meanpt = proMerged->GetBinCenter(i);
        double val = proMerged->GetBinContent(i);
        cov += meanpt * val * weight;
        point += weight;


        //std::cout << val << " " << weight << std::endl;
    }
    cov /= point;

    // std::cout << "cov " << cov << " ptBar " << ptBar << " varbar " << valBar << std::endl;
    return cov;
}


TH1D *calculate_v2pt_corr(TProfile2D *h_prof_total, const char *name)
{
    // get single profiles
    TProfile *h_c22 = h_prof_total->ProfileX(Form("c22%s", name), 1, 1);
    TProfile *h_c24 = h_prof_total->ProfileX(Form("c24%s", name), 3, 3);
    TProfile *h_covv2pt = h_prof_total->ProfileX(Form("cov_v2pt%s", name), 11, 11);
    TProfile *h_ptsquareave = h_prof_total->ProfileX(Form("ptSquareAve%s", name), 13, 13);
    TProfile *h_ptave = h_prof_total->ProfileX(Form("ptAve%s", name), 14, 14);
    TProfile *h_meanpt = h_prof_total->ProfileX(Form("meanpt%s", name), 15, 15);
    TProfile *h_c22_trackweighted = h_prof_total->ProfileX(Form("c22_trackweighted%s", name), 6, 6);
    TProfile *h_c22_full = h_prof_total->ProfileX(Form("c22full%s", name), 5, 5);
    // end get single profiles

    TH1D *output = new TH1D(name, name, h_c22->GetNbinsX(), h_c22->GetXaxis()->GetXbins()->GetArray());

    for (int i = 1; i <= h_c22->GetNbinsX(); i++)
    {
        // get value from each bin
        double cov_v2pt = h_covv2pt->GetBinContent(i);
        //double cov_v2pt = get_cov_part1(i);
        double c22 = h_c22->GetBinContent(i);
        double ptAve = h_ptave->GetBinContent(i);
        double c24 = h_c24->GetBinContent(i);
        double ptSquareAve = h_ptsquareave->GetBinContent(i);
        double c22_trackweighted = h_c22_trackweighted->GetBinContent(i);
        double meanpt = h_meanpt->GetBinContent(i);
        double c22full = h_c22_full->GetBinContent(i);
        // end get value from each bin

        // check negative sqrt
        if (ptSquareAve - 2 * ptAve * meanpt + meanpt * meanpt <= 0)
        {
            output->SetBinContent(i, 0);
            continue;
        }
        if (c24 - 2 * c22full * c22full + c22 * c22 <= 0)
        {
            output->SetBinContent(i, 0);
            continue;
        }
        // end check negative sqrt


        // calculate v2ptcorre
        double v2ptcorre = (cov_v2pt - ptAve * c22_trackweighted) /
                           sqrt(ptSquareAve - 2 * ptAve * meanpt + meanpt * meanpt) /
                           sqrt(c24 - 2 * c22full * c22full + c22 * c22);
        output->SetBinContent(i, v2ptcorre);
        // end calculate v2ptcorre
        // std::cout << "cov_v2pt = " << cov_v2pt << std::endl;
        // std::cout << "ptAve = " << ptAve << std::endl;
        // std::cout << "c22_trackweighted = " << c22_trackweighted << std::endl;
        // std::cout << "================================" << std::endl;
        std::cout << c22_trackweighted << std::endl;
    }

    // clean up
    delete h_c22;
    delete h_c24;
    delete h_covv2pt;
    delete h_ptsquareave;
    delete h_ptave;
    delete h_meanpt;
    delete h_c22_trackweighted;
    // end clean up

    return output;
}

void draw_v2pt_correlation()
{
    // init and get prof
    TFile *f = TFile::Open(FILE);
    TDirectory *dir = (TDirectory *)f->Get("pid-flow-pt-corr");
    FlowContainer *fc_ch = (FlowContainer *)dir->Get("FlowContainerCharged");
    TProfile2D *h_prof_ch = fc_ch->GetProfile();
    TObjArray *arr = fc_ch->GetSubProfiles();
    int nsub = arr->GetEntriesFast();
    std::cout << "Number of sub-profiles: " << nsub << std::endl;
    // end init and get prof

    // get value
    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    TH1D *res = calculate_v2pt_corr(h_prof_ch, "v2pt_correlation");
    res->Draw();
    // get error
    // std::vector<double> error4eachbin;
    // for (int i = 1; i <= res->GetNbinsX(); i++)
    // {
    //     error4eachbin.push_back(0.0);
    // }

    // for (int i = 0; i < nsub; i++)
    // {
    //     TProfile2D *h_sub_prof = (TProfile2D *)arr->At(i);
    //     TH1D *h_sub = calculate_v2pt_corr(h_sub_prof, std::to_string(i).c_str());
    //     for (int j = 1; j <= h_sub->GetNbinsX(); j++)
    //     {
    //         double diff_square = TMath::Power(h_sub->GetBinContent(j) - res->GetBinContent(j), 2);
    //         error4eachbin[j - 1] += diff_square;
    //     }
    // }

    // for (int i = 0; i < error4eachbin.size(); i++)
    // {
    //     double stddev = TMath::Sqrt(error4eachbin[i] / (nsub - 1));
    //     res->SetBinError(i + 1, stddev);
    // }
    // // end get value and error

    // // draw
    // TCanvas *c2 = new TCanvas("c2", "c2", 1800, 1000);
    // TFile *data_run2 = TFile::Open("/home/huinaibing/Downloads/run2v2ptcorr.root");
    // TDirectory *dir_run2 = (TDirectory *)data_run2->Get("Table 1");
    // TGraphErrors *v2pt_run2 = (TGraphErrors *)dir_run2->Get("Graph1D_y1");

    // TH2D *frame = new TH2D("frame", "v2pt correlation;Centrality (%);#rho(v_{2}^{2}{2}, p_{T})", 10, 0, 60, 10, 0, 0.3);
    // frame->SetStats(0);
    // frame->Draw();

    // res->SetLineColor(kRed);
    // res->Draw("SAME");
    // v2pt_run2->Draw("P SAME");
    // TLegend *leg = new TLegend(0.6, 0.7, 0.88, 0.88);
    // leg->AddEntry(res, "run 3", "lep");
    // leg->AddEntry(v2pt_run2, "run 2", "lep");
    // leg->Draw("SAME");
    // // end draw
}
