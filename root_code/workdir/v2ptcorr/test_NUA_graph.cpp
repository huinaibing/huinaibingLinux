#include "TFile.h"
#include "TDirectory.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "THn.h"
#include <iostream>
#include "GFWWeights.h"

void test_NUA_graph()
{
    TFile *weight = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/correction_littlecut/pass5zzh_NUA_544116.root");
    TFile *file = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_new_cut_small_errornua.root");
    TFile *origin = TFile::Open("/home/huinaibing/Downloads/AnalysisResults (50).root");
    THnSparseD *thn = (THnSparseD *)((TDirectory *)((TDirectory *)file->Get("pid-flow-pt-corr"))->Get("debug"))->Get("hRunNumberPhiEtaVertexWeight");

    thn->GetAxis(0)->SetRange(2, 2);
    TH3 *h = thn->Projection(1, 2, 3);
    // TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    double jobs = 479.;

    std::vector<std::tuple<double, double, double>> highbin;

    std::ofstream outfile("output.txt");

    for (int i = 1; i <= h->GetNbinsX(); i++)
    {
        for (int j = 1; j <= h->GetNbinsY(); j++)
        {
            for (int k = 1; k <= h->GetNbinsZ(); k++)
            {
                double content = h->GetBinContent(i, j, k);

                h->SetBinContent(i, j, k, content / jobs);
                if (content / jobs > 2)
                {
                    outfile << "position (" << h->GetXaxis()->GetBinCenter(i) << "," << h->GetYaxis()->GetBinCenter(j) << "," << h->GetZaxis()->GetBinCenter(k) << ")"
                            << " has content: " << content / jobs << std::endl;
                    highbin.push_back(std::make_tuple(h->GetXaxis()->GetBinCenter(i), h->GetYaxis()->GetBinCenter(j), h->GetZaxis()->GetBinCenter(k)));
                }
            }
        }
    }

    // h->Draw();
    GFWWeights *weights = (GFWWeights *)weight->Get("ccdb_object");

    for (auto &bin : highbin)
    {
        double phi = std::get<0>(bin);
        double eta = std::get<1>(bin);
        double vtxz = std::get<2>(bin);

        double w = weights->getNUA(phi, eta, vtxz);
        outfile << "weight for (" << phi << "," << eta << "," << vtxz << ") is " << w << std::endl;
    }

    THnSparseD *thn_origin = (THnSparseD *)((TDirectory *)((TDirectory *)origin->Get("pid-flow-pt-corr"))->Get("correction"))->Get("hRunNumberPhiEtaVertex");
    // thn_origin->GetAxis(0)->SetRange(6, 6);
    TH3 *h_origin = thn_origin->Projection(1, 2, 3);

    for (auto &bin : highbin)
    {
        double phi = std::get<0>(bin);
        double eta = std::get<1>(bin);
        double vtxz = std::get<2>(bin);

        int binx = h_origin->GetXaxis()->FindBin(phi);
        int biny = h_origin->GetYaxis()->FindBin(eta);
        int binz = h_origin->GetZaxis()->FindBin(vtxz);

        double content_origin = h_origin->GetBinContent(binx, biny, binz);
        outfile << "original content for (" << phi << "," << eta << "," << vtxz << ") is " << content_origin << std::endl;
    }

    int max = -1;
    int min = 999999999;
    int total = 0;

    for (int i = 1; i <= h_origin->GetNbinsX(); i++)
    {
        for (int j = 1; j <= h_origin->GetNbinsY(); j++)
        {
            for (int k = 1; k <= h_origin->GetNbinsZ(); k++)
            {
                double content_origin = h_origin->GetBinContent(i, j, k);
                total += content_origin;
                if (content_origin > max)
                    max = content_origin;
                if (content_origin < min && content_origin != 0)
                    min = content_origin;

                if (content_origin != 0)
                    outfile << "all the contents in original: position (" << h_origin->GetXaxis()->GetBinCenter(i) << "," << h_origin->GetYaxis()->GetBinCenter(j) << "," << h_origin->GetZaxis()->GetBinCenter(k) << ")"
                            << " has content: " << content_origin << std::endl;
            }
        }
    }

    TFile *file_NUAOther = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/NUAOther.root");
    TH3D *h_NUAOther = file_NUAOther->Get<TDirectory>("flow-task_origin")->Get<TDirectory>("544116")->Get<TH3D>("hPhiEtaVtxz");
    for (auto &bin : highbin)
    {
        double phi = std::get<0>(bin);
        double eta = std::get<1>(bin);
        double vtxz = std::get<2>(bin);

        int binx = h_NUAOther->GetXaxis()->FindBin(phi);
        int biny = h_NUAOther->GetYaxis()->FindBin(eta);
        int binz = h_NUAOther->GetZaxis()->FindBin(vtxz);

        double content_NUAOther = h_NUAOther->GetBinContent(binx, biny, binz);
        outfile << "NUA from other source for (" << phi << "," << eta << "," << vtxz << ") is " << content_NUAOther << std::endl;
    }

    // outfile << "max content in original: " << max << ", min content in original: " << min << " total " << total << std::endl;

    // int thn_total = 0;
    // for (int i = 1; i <= thn_origin->GetAxis(0)->GetNbins(); i++)
    // {
    //     for (int j = 1; j <= thn_origin->GetAxis(1)->GetNbins(); j++)
    //     {
    //         for (int k = 1; k <= thn_origin->GetAxis(2)->GetNbins(); k++)
    //         {
    //             for (int l = 1; l <= thn_origin->GetAxis(3)->GetNbins(); l++)
    //             {
    //                 int bins[4] = {i, j, k, l};
    //                 double content_origin = thn_origin->GetBinContent(bins);
    //                 thn_total += content_origin;
    //             }
    //         }
    //     }
    // }
    // outfile << "total entries in thn_origin: " << thn_total << std::endl;

    outfile.close();
}