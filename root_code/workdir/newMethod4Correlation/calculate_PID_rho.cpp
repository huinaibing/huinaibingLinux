#include "PIDReader.h"
#include "flowContainerReader.h"

#define FILE "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/new_method.root"
#define DIRNAME "pid-flow-pt-corr"
#define FLOWCONTAINERNAME "FlowContainerPi"
#define PARTICLETYPE 1
#define OUTPUTFILE "result/res_pion.root"

void calculate_PID_rho()
{
    PIDReader *rd = new PIDReader(FILE, DIRNAME, PARTICLETYPE);
    FlowContainerReader *fcReader_pid =
        new FlowContainerReader(FILE, DIRNAME, FLOWCONTAINERNAME, FLOWCONTAINERNAME, true);
    FlowContainerReader *fcReader_ch =
        new FlowContainerReader(FILE, DIRNAME, "FlowContainerCharged", "FlowContainerCharged", false);


    double centAxis[12] = {0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90};

    TH1D *rho_PID = new TH1D("hrhoPID", "", 11, centAxis);

    double res[11] = {0};


    /// @note this for loop is used to calculate center val
    for (int i = 1; i <= 11; i++)
    {
        double varpt = fcReader_pid->get_var_meanpt(i);
        double varc22 =
            Utils::calculate_var_pid(i, fcReader_ch->get_profile2d_reader(), fcReader_pid->get_profile2d_reader());

        if (varpt == 0 || varc22 == 0)
            continue;

        double meanPtInCent = fcReader_pid->get_meanpt(i);
        double covInCent = Utils::get_cov(rd->getSpecifiedCentBin(i), meanPtInCent);

        res[i] = covInCent / varpt / varc22;
    }
    // end center val

    double bserror[11] = {0};

    /// @note bootstrap
    for (int bsIDX = 0; bsIDX < 30; bsIDX++)
    {
        for (int i = 1; i <= 11; i++)
        {
            double varpt = fcReader_pid->get_var_meanpt(i, bsIDX);
            double varc22 = Utils::calculate_var_pid(i,
                                                     fcReader_ch->get_profile2d_reader(bsIDX),
                                                     fcReader_pid->get_profile2d_reader(bsIDX));

            if (varpt == 0 || varc22 == 0)
                continue;

            double meanPtInCent = fcReader_pid->get_meanpt(i, bsIDX);
            double covInCent = Utils::get_cov(rd->getSpecifiedCentBin(i, bsIDX), meanPtInCent);

            double res_bs = covInCent / varpt / varc22;

            bserror[i - 1] += TMath::Power(res_bs - res[i - 1], 2);
        }
    }

    for (int i = 0; i < 11; i++)
    {
        bserror[i] = TMath::Sqrt(bserror[i] / 29);
    }
    // end bootstrap

    for (int i = 1; i <= 11; i++)
    {
        if (std::abs(res[i]) <= 1)
        {
            rho_PID->SetBinContent(i, res[i]);
            rho_PID->SetBinError(i, bserror[i]);
        }
    }

    TFile *res_file = TFile::Open(OUTPUTFILE, "RECREATE");
    rho_PID->Write();
    res_file->Close();

    std::cout << "============================" << std::endl;
}
