#include "TFile.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "iostream"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "GFWWeights.h"

void determineStamps01()
{
  int runs[] = {564468, 564472};

  int nRuns = sizeof(runs) / sizeof(runs[0]);

  o2::ccdb::CcdbApi ccdb_api;
  ccdb_api.init("https://alice-ccdb.cern.ch");
  std::map<string, string> metadataRCT, headers;

  for (int ii = 0; ii < nRuns; ii++)
  {

    headers = ccdb_api.retrieveHeaders(Form("RCT/Info/RunInformation/%i", runs[ii]), metadataRCT, -1);
    int64_t tsSOR = atol(headers["SOR"].c_str());
    int64_t tsEOR = atol(headers["EOR"].c_str());

    cout << "Run " << runs[ii] << " SOR " << tsSOR << " EOR " << tsEOR << endl;

    TFile *fileREF = new TFile(Form("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/v2ptcorr/correctionNeNe/afpass2_NUA_%d.root", runs[ii]), "READ");
    TObject *fWeightsREF = nullptr;
    fWeightsREF = fileREF->Get("ccdb_object");

    map<string, string> metadata; // can be empty
    metadata.insert(std::pair{"ObjectType", "GFWWeights"});
    metadata.insert(std::pair{"Description", Form("Acceptance correcrion for run %s", Form("%i", runs[ii]))});
    metadata.insert(std::pair{"Author", "Qiuyu"});

    cout << "Attempting CCDB upload..." << endl;
    try
    {
      ccdb_api.storeAsTFileAny(fWeightsREF, "Users/q/qiuyu/NeNe/afpass2/tr644077", metadata, tsSOR, tsEOR);
    }
    catch (std::exception const &e)
    {
      cout << "Failed at CCDB submission!" << endl;
    }
    cout << "Finished with upload of run " << runs[ii] << " update! " << endl;
  }
  cout << "Done!" << endl;
}
