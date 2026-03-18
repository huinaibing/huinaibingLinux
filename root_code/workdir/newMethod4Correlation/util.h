#ifndef UTIL
#define UTIL
#include "TProfile.h"
#include "flowContainerReader.h"

class Utils
{
public:
    static double getMeanXY(TProfile *proMerged, TProfile *proMergedpt)
    {
        double cov = 0;
        double point = 0;

        for (int i = 1; i <= proMerged->GetNbinsX(); i++)
        {
            double weight = proMerged->GetBinEntries(i);

            if (weight == 0)
            {
                continue;
            }

            double meanpt = 0;
            if (proMergedpt)
            {
                meanpt = proMergedpt->GetBinContent(i);
            }
            else
            {
                meanpt = proMerged->GetBinCenter(i);
            }

            double val = proMerged->GetBinContent(i);
            cov += meanpt * val * weight;
            point += weight;
        }
        cov /= point;
        return cov;
    }

    static double calculate_var_pid(int centBin,
                                    FlowContainerTProfile2DReader ch2d_reader,
                                    FlowContainerTProfile2DReader pid2d_reader)
    {
        if (ch2d_reader.isPID != false || pid2d_reader.isPID != true)
        {
            std::cout << "reader type error !!!!!!!!!!!!!!!!!!!" << std::endl;
            return 0;
        }

        double pid_charged_c22_gap = pid2d_reader.h_c22->GetBinContent(centBin);
        double charged_c22_gap = ch2d_reader.h_c22->GetBinContent(centBin);

        double pid_charged_c22_full = pid2d_reader.h_c22Full->GetBinContent(centBin);
        double charged_c22_full = ch2d_reader.h_c22Full->GetBinCenter(centBin);
        double pid_charged_c24 = pid2d_reader.h_c24->GetBinContent(centBin);
        double charged_c24 = ch2d_reader.h_c24->GetBinContent(centBin);


        double v22pow4 = pow(pid_charged_c22_gap, 4) / charged_c22_gap / charged_c22_gap;

        double v24pow4 = pow(2 * pid_charged_c22_full * charged_c22_full - pid_charged_c24, 4) /
                         pow(2 * charged_c22_full * charged_c22_full - charged_c24, 3);

        double res = v22pow4 - v24pow4;

        if (res <= 0)
            return 0;

        return sqrt(res);
    }
};

#endif
