//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 26 06:56:56 2025 by ROOT version 6.32.10
// from TTree myTuple/myTuple
// found on file: bmass_test.root
//////////////////////////////////////////////////////////

#ifndef myTuple_h
#define myTuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class myTuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        Mass;
   Double_t        Px;
   Double_t        Py;
   Double_t        Pz;
   Double_t        Pe;

   // List of branches
   TBranch        *b_Mass;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_Pe;   //!


   TH1D* pt;
   TH1D* rapidity;
   TFile* root_file;

   myTuple(TTree *tree=0);
   virtual ~myTuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef myTuple_cxx
myTuple::myTuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("bmass_test.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("bmass_test.root");
      }
      f->GetObject("myTuple",tree);

   }
   Init(tree);
}

myTuple::~myTuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t myTuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t myTuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void myTuple::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Mass", &Mass, &b_Mass);
   fChain->SetBranchAddress("Px", &Px, &b_Px);
   fChain->SetBranchAddress("Py", &Py, &b_Py);
   fChain->SetBranchAddress("Pz", &Pz, &b_Pz);
   fChain->SetBranchAddress("Pe", &Pe, &b_Pe);
   Notify();

   pt = new TH1D("pt", "pt histogram", 300, 0, 30000);
   rapidity = new TH1D("rapidity", "rapidity histogram", 100, 0, 6);
   root_file = new TFile("output.root", "RECREATE");
}

bool myTuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void myTuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t myTuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef myTuple_cxx
