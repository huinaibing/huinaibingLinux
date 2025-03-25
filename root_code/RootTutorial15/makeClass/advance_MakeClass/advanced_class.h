//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 24 13:24:12 2025 by ROOT version 6.32.10
// from TTree myTuple/myTuple
// found on file: ../../simple_TTree/BUILD/bmass_test.root
//////////////////////////////////////////////////////////

#ifndef advanced_class_h
#define advanced_class_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class advanced_class {
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

   // 我的自己的一些变量
   TFile* my_rootfile;
   TH1D* hist_mass;
   // 上面定义了一个直方图，用于存储质量的分布

   advanced_class(TTree *tree=0);
   virtual ~advanced_class();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef advanced_class_cxx
advanced_class::advanced_class(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../simple_TTree/BUILD/bmass_test.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../simple_TTree/BUILD/bmass_test.root");
      }
      f->GetObject("myTuple",tree);

   }
   Init(tree);
}

advanced_class::~advanced_class()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t advanced_class::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t advanced_class::LoadTree(Long64_t entry)
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

void advanced_class::Init(TTree *tree)
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

   // 我自己定义的变量的初始化
   my_rootfile = new TFile("BEvents_skimmed_file.root", "RECREATE");
   hist_mass = new TH1D("hist_mass", "hist_mass", 100, 5200, 5400);
   hist_mass->Sumw2();
   // 上面这个Sumw2()函数是用来开启直方图的错误计算的
   // 直接在直方图初始化的时候调用这个函数就可以了
   // 可以参考这个链接：https://root.cern/doc/master/classTH1.html#a
   // 里面有一个关于Sumw2()的解释
   // 卧槽，上面的注释是Ai生成的
   // 我什么都没干，这也太夸张了把
}

bool advanced_class::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void advanced_class::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t advanced_class::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef advanced_class_cxx
