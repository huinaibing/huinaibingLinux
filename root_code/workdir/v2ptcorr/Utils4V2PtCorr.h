#ifndef UTILS4V2PTCORR_H
#define UTILS4V2PTCORR_H

#include <iostream>
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "TLegend.h"
double calculate_ratio_v24(double pid_charged_v4, double pid_charged_v2, double charged_v2, double charged_v4)
{
    double cn4 = charged_v4 - 2 * charged_v2 * charged_v2;
    if (cn4 > 0)
    {
        return 0;
    }

    return (2 * pid_charged_v2 * charged_v2 - pid_charged_v4) / pow(-cn4, 0.75);
}

double calculate_pure_v24(double pid_v4, double pid_v2)
{
    double cn4 = pid_v4 - 2 * pid_v2 * pid_v2;
    if (cn4 > 0)
    {
        return 0;
    }

    return pow(-cn4, 0.25);
}

double calculate_ratio_v22(double pid_charged_v2, double charged_v2)
{
    if (charged_v2 <= 0)
    {
        return 0;
    }
    return pid_charged_v2 / sqrt(charged_v2);
}
#endif