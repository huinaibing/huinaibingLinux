#define Myroot_class_cxx
#include "Myroot_class.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void Myroot_class::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Myroot_class.C
//      root> Myroot_class t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // 这个fChain指向TTree
      // 使用上面这样的方法，告诉函数ID，
      // TTree会把值给这个类的变量们
      // 具体的看头文件定义
      // if (Cut(ientry) < 0) continue;

      if (Mass > 5400)
      {
         std::cout << "Mass = " << Mass << std::endl;
         std::cout << "这是事例的ID" << std::endl;
         // 显然，上面这个for循环会遍历所有的事例
      }

   }
}
