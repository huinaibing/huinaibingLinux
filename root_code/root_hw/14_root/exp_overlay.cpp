#include <TCanvas.h>
#include <TRandom3.h>
#include <TFile.h>
#include <TH1D.h>


void exp_overlay()
{
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    TFile* store_file = new TFile("exp_overlay.root", "recreate");
    
    // 产生两个随机数产生器
    TRandom3* r1 = new TRandom3(gRandom->Integer(10000));
    TRandom3* r2 = new TRandom3(gRandom->Integer(10000));

    // TH1D hist
    TH1D* h_overlay = new TH1D("h_overlay", "exp overlay", 200, 0, 5);
    TH1D* h_exp5 = new TH1D("exp5", "exp(5)", 200, 0, 5);
    TH1D* h_exp3 = new TH1D("exp3", "exp(3)", 200, 0, 5);

    for (int i = 0; i < 10000; i++)
    {
        h_overlay->Fill(r1->Exp(5));
        h_overlay->Fill(r2->Exp(3));

        h_exp5->Fill(r1->Exp(5));
        h_exp3->Fill(r2->Exp(3));
    }

    h_overlay->Write();
    h_exp5->Write();
    h_exp3->Write();

    store_file->Close();
}
