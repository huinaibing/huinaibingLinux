#ifndef BASEFILE
#define BASEFILE
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
class BaseFileReader
{
protected:
    TDirectory *dir;

public:
    BaseFileReader(const char *file_path, const char *dir_name)
    {
        TFile *f = TFile::Open(file_path);
        dir = (TDirectory *)f->Get(dir_name);
    }
};


#endif
