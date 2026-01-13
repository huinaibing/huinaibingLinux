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
    TFile *file = TFile::Open("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/corrections_pass3/NUA_544122.root");
    GFWWeights *th3 = (GFWWeights *)file->Get("ccdb_object");
    fstream o("log_NUA.txt", ios::out);
    TH3D *h_wacc = new TH3D("h_acc", "h_acc", 60, 0, 2 * TMath::Pi(), 64, -1.6, 1.6, 40, -10, 10);

    for (int j = 1; j <= 64; j++)
    {
        for (int k = 1; k <= 40; k++)
        {
            for (int i = 1; i <= 60; i++)
            {
                double phi = h_wacc->GetXaxis()->GetBinCenter(i);
                double eta = h_wacc->GetYaxis()->GetBinCenter(j);
                double vz = h_wacc->GetZaxis()->GetBinCenter(k);
                h_wacc->SetBinContent(i, j, k, th3->getNUA(phi, eta, vz));
                o << phi << " " << eta << " " << vz << " " << th3->getNUA(phi, eta, vz) << endl;
            }
        }
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
    h_wacc->Draw();
    o.close();
}