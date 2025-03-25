#include<iostream>
#include<TCanvas.h>
#include<TRandom3.h>
#include<TFile.h>
#include<TF1.h>
#include<TH1D.h>
#include<TMath.h>

void square_func_fit()
{
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    TFile* store_file = new TFile("square_func_fit.root", "recreate");

    TF1* sample_function = new TF1("sample_function", "x * x - 20 * x + 100", -20, 30);
    TH1D* h_sample = new TH1D("h1d", "", 200, -20, 30);
    h_sample->Sumw2();

    for (int i = 0; i < 100000; i++)
    {
        h_sample->Fill(sample_function->GetRandom());
    }

    TF1* fit_function = new TF1(
        "fit_function",
        [](double* x, double* par){return par[0] * x[0] * x[0] + par[1] * x[0] + par[2];},
        -20,
        30,
        3
    );

    h_sample->Fit(fit_function, "", "", -20, 30);

    h_sample->Write();
    store_file->Close();
}
