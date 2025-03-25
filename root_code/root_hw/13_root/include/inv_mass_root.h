#ifndef __inv_mass_root__
#define __inv_mass_root__

// 继续继承第十个ppt的作业的代码
// 这就是面向对象的好处
#include "/home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/include/cal_inv_mass_.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <iostream>


class CalInvMassRoot: public CalInvMass
{
protected:
    TH1D* hist_mass;
public:
    CalInvMassRoot(char* file_path);
    void draw_hist();
};

#endif
