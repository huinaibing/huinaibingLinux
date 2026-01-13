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
  /*
  int runs[] = {
      523142, 523148, 523182, 523186, 523298, 523306,
      523308, 523309, 523397, 523399, 523401, 523441,
      523541, 523559, 523669, 523671, 523677, 523728,
      523731, 523779, 523783, 523786, 523788, 523789,
      523792, 523797, 523821, 526463, 526465, 526466,
      526467, 526468, 526486, 526505, 526512, 526525,
      526526, 526528, 526559, 526596, 526606, 526612,
      526639, 526641, 526647, 526649, 526713, 526714,
      526715, 526716, 526719, 526720, 526776, 526886,
      526938, 526963, 526964, 526966, 526967, 526968,
      527015, 527016, 527028, 527031, 527033, 527034,
      527038, 527039, 527041, 527057, 527076, 527109,
      527237, 527240, 527259, 527260, 527261, 527262,
      527349, 527446, 527518, 527523, 527690, 527694,
      527731, 527734, 527736, 527821, 527825, 527826,
      527828, 527848, 527850, 527852, 527863, 527864,
      527865, 527869, 527871, 527895, 527898, 527899,
      527902, 527976, 527978, 527979, 528021, 528026,
      528036, 528094, 528097, 528105, 528107, 528109,
      528110, 528231, 528232, 528233, 528263, 528266,
      528292, 528294, 528316, 528319, 528328, 528329,
      528330, 528332, 528336, 528347, 528359, 528379,
      528381, 528386, 528448, 528451, 528461, 528463,
      528530, 528531, 528534, 528537, 528543, 528602,
      528604, 528617, 528781, 528782, 528783, 528784,
      528798, 528801, 529077, 529078, 529084, 529088,
      529115, 529116, 529117, 529128, 529208, 529209,
      529210, 529211, 529235, 529237, 529242, 529248,
      529252, 529270, 529306, 529317, 529320, 529324,
      529338, 529341, 529450, 529452, 529454, 529458,
      529460, 529461, 529462, 529542, 529552, 529554,
      529662, 529663, 529664, 529674, 529675, 529690,
      529691
  };
  */
  int runs[] = {
      544095, 544098, 544116, 544121, 544122, 544123, 544124
  };
  // int runs[] = {
  //     544013,
  //     544028, 544032,
  //     544124, 544095, 544098, 544116, 544121, 544122, 544123, 544124,
  //     544184, 544185, 544389, 544390, 544391, 544392,
  //     544451, 544454, 544474, 544475, 544476, 544477, 544490, 544491,
  //     544492, 544508, 544510, 544511, 544512, 544514, 544515, 544518,
  //     544548, 544549, 544550, 544551, 544564, 544565, 544567, 544568,
  //     544580, 544582, 544583, 544585,
  //     544614, 544640, 544674, 544754, 544767, 544652, 544653, 544672,
  //     544692, 544693, 544694, 544696, 544739, 544742,
  //     544794, 544795, 544797, 544813,
  //     544868, 544886, 544887, 544896, 544913, 544914, 544917, 544931,
  //     544947, 544961, 544963, 544964, 544968, 544992, 545009, 545044,
  //     545047, 545063, 545064, 545066, 545185, 545210, 545223, 545249,
  //     545291, 545294, 545295, 545296, 545312};

  // int runs[] = {
  //     544116, 544122
  // };
  // int nRuns = 193;
  // int nRuns = 8
  int nRuns = 92;
  TString name[] = {"REF", "K0s", "Lambda", "Xi", "Omega"};

  o2::ccdb::CcdbApi ccdb_api;
  ccdb_api.init("https://alice-ccdb.cern.ch");
  std::map<string, string> metadataRCT, headers;

  for (int ii = 0; ii < nRuns; ii++)
  {

    headers = ccdb_api.retrieveHeaders(Form("RCT/Info/RunInformation/%i", runs[ii]), metadataRCT, -1);
    int64_t tsSOR = atol(headers["SOR"].c_str());
    int64_t tsEOR = atol(headers["EOR"].c_str());

    cout << "Run " << runs[ii] << " SOR " << tsSOR << " EOR " << tsEOR << endl;

    // cout<<"Now performing dedicated object upload... please wait..."<<endl;
    // cout<<"Opening file"<<endl;

    TFile *fileREF = new TFile(Form("REF/NUAREF_%d.root", runs[ii]), "READ");
    TObject *fWeightsREF = nullptr;
    fWeightsREF = fileREF->Get("ccdb_object");
    // cout<<"Opening root file..."<<endl;

    // TH3F *hAccREF = (TH3F*) file->Get("flow-g-f-w-omega-xi/hEtaPhiVtxzREF");
    // TH3F *hAccK0s = (TH3F*) file->Get("flow-g-f-w-omega-xi/hEtaPhiVtxzPOIXi");
    // TH3F *hAccLambda = (TH3F*) file->Get("flow-g-f-w-omega-xi/hEtaPhiVtxzPOILambda");
    // TH3F *hAccXiMinus = (TH3F*) file->Get("flow-g-f-w-omega-xi/hEtaPhiVtxzPOIXi");
    // TH3F *hAccOmegaMinus = (TH3F*) file->Get("flow-g-f-w-omega-xi/hEtaPhiVtxzPOIOmega");

    // hAccREF->SetName("hAccREF");
    // hAccK0s->SetName("hAccK0s");
    // hAccLambda->SetName("hAccLambda");
    // hAccXiMinus->SetName("hAccXiMinus");
    // hAccOmegaMinus->SetName("hAccOmegaMinus");

    // cout<<"Defining metadata for this run..."<<endl;
    map<string, string> metadata; // can be empty
    metadata.insert(std::pair{"ObjectType", "GFWWeights"});
    metadata.insert(std::pair{"Description", Form("Acceptance correcrion for run %s", Form("%i", runs[ii]))});
    metadata.insert(std::pair{"Author", "Fuchun Cui"});

    // TList *listHistograms = new TList();
    // listHistograms->Add(fileREF);
    // listHistograms->Add(hAccK0s);
    // listHistograms->Add(hAccLambda);
    // listHistograms->Add(hAccXiMinus);
    // listHistograms->Add(hAccOmegaMinus);

    cout << "Attempting CCDB upload..." << endl;
    try
    {

      ccdb_api.storeAsTFileAny(fWeightsREF, "Users/f/fcui/apass5/NUA20251106/NUAREF", metadata, tsSOR, tsEOR);
    }
    catch (std::exception const &e)
    {
      LOG(fatal) << "Failed at CCDB submission!";
    }
    cout << "Finished with upload of run " << runs[ii] << " update! " << endl;
  }
  cout << "Done!" << endl;
}
