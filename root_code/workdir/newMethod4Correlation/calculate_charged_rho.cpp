#include "chargedReader.h"
#include "flowContainerReader.h"
#include "util.h"


#define FILE                                                                                                           \
    "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
    "big_valueerror.root"
#define DIRNAME "pid-flow-pt-corr_id50251"


void calculate_charged_rho()
{
    FlowContainerReader *fcReader =
        new FlowContainerReader(FILE, DIRNAME, "FlowContainerCharged", "FlowContainerCharged", false);
    chargedReader *chReader = new chargedReader(FILE, DIRNAME);

    double centAxis[12] = {0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90};

    TH1D *rho_charged = new TH1D("hrhocharged", "", 11, centAxis);

    double res[11] = {0};

    for (int i = 1; i <= 11; i++)
    {
        double varpt = fcReader->get_var_meanpt(i);
        double varc22 = fcReader->get_var_c22(i);

        if (varpt == 0 || varc22 == 0)
            continue;

        // double cov_part1 =
        //     Utils::getMeanXY(chReader->getSpecifiedCentBin(i, true), chReader->getSpecifiedCentBin(i, false));
        // double pt_ave = fcReader->get_ptave(i);
        // double c22_TrackWeighted = fcReader->get_c22_trackweight(i);

        // double meanPtInCent = fcReader->get_meanpt(i);
        // std::cout << "meanPtInCent " << meanPtInCent << std::endl;

        // std::cout << cov_part1 << std::endl;
        // std::cout << pt_ave * c22_TrackWeighted << "123456" << std::endl;
        // std::cout << cov_part1 - pt_ave * c22_TrackWeighted << " abcdefg" << std::endl;
        double meanPtInCent = fcReader->get_ptave(i);
        double cov = Utils::get_cov(chReader->getSpecifiedCentBin(i, true),
                                    meanPtInCent,
                                    chReader->getSpecifiedCentBin(i, false));

        std::cout << "cov" << cov << std::endl;

        res[i] = cov / varpt / varc22;
        // res[i] = (cov_part1 - pt_ave * c22_TrackWeighted) / varpt / varc22;
    }

    double bserror[11] = {0};

    // for (int bsIDX = 0; bsIDX < 30; bsIDX++)
    // {
    //     for (int i = 1; i <= 11; i++)
    //     {
    //         double varpt = fcReader->get_var_meanpt(i, bsIDX);
    //         double varc22 = fcReader->get_var_c22(i, bsIDX);

    //         if (varpt == 0 || varc22 == 0)
    //             continue;

    //         double cov_part1 = Utils::getMeanXY(chReader->getSpecifiedCentBin(i, bsIDX, true),
    //                                             chReader->getSpecifiedCentBin(i, bsIDX, false));
    //         double pt_ave = fcReader->get_ptave(i, bsIDX);
    //         double c22_TrackWeighted = fcReader->get_c22_trackweight(i, bsIDX);

    //         double rho_bs = (cov_part1 - pt_ave * c22_TrackWeighted) / varpt / varc22;

    //         bserror[i - 1] += TMath::Power(rho_bs - res[i - 1], 2);
    //     }
    // }

    // for (int i = 0; i < 11; i++)
    // {
    //     bserror[i] = TMath::Sqrt(bserror[i] / 29);
    // }

    for (int i = 1; i <= 11; i++)
    {
        rho_charged->SetBinContent(i, res[i]);
        rho_charged->SetBinError(i, bserror[i]);
    }

    TCanvas *c1 = new TCanvas("c1", "", 800, 600);
    rho_charged->Draw();
}
