#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>

void exp_overlay()
{
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    TF1* exp1 = new TF1("exp1", "(1 / 5) * exp(x / 5)", 0, 5);
    TF1* exp2 = new TF1("exp2", "(1 / 3) * exp(-x / 3)", 0, 5);

    exp1->SetLineColor(2);
    exp2->SetLineColor(4);

    exp1->Draw();
    exp2->Draw("same");

    TFile* my_file = new TFile("exp_overlay.root", "RECREATE");
    
    exp1->Write();
    exp2->Write();

    c1->Write();
    my_file->Close();
}
