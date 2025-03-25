#include "inv_mass_root.h"


CalInvMassRoot::CalInvMassRoot(char* file_path)
    : CalInvMass(file_path)
{
    hist_mass = new TH1D("hist_mass", "Invariant Mass", 200, 5150, 5400);
    hist_mass->Sumw2();
    this->calculate_inv_mass();
}

void CalInvMassRoot::draw_hist()
{
    for (int i = 0; i < this->data_inv_mass.size(); i++)
    {
        hist_mass->Fill(this->data_inv_mass[i]);
    }
    hist_mass->Draw();

    TFile* my_rootfile = new TFile("inv_mass.root", "recreate");
    my_rootfile->cd();
    hist_mass->Write();
    my_rootfile->Close();
}