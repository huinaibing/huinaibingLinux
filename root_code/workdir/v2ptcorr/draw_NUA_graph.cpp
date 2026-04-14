#include "GFWWeights.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH3.h"
#include "THn.h"
#include "THnSparse.h"
#include <iostream>

using namespace std;

#define FILE "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/correction/NUAFile/arpass3_NUA_bugfix.root"

void draw_NUA_graph()
{
    // get graph
    int runlist[] = {559781, 559802, 559803, 559827, 559843, 559856, 559917, 559933, 559966, 559968,
                     559969, 559970, 559987, 560012, 560031, 560033, 560049, 560066, 560067, 560070,
                     560089, 560090, 560105, 560106, 560123, 560127, 560141, 560142, 560184};
    int numberOfRun = sizeof(runlist) / sizeof(runlist[0]);
    GFWWeights *fWeightsREF = nullptr;
    TFile *file = TFile::Open(FILE);
    TDirectory *dir_main = (TDirectory *)file->Get("pid-flow-pt-corr");
    TDirectory *correction = (TDirectory *)dir_main->Get("correction");
    THnSparseF *grh = (THnSparseF *)correction->Get("hRunNumberPhiEtaVertex");
    // end graph

    // ... (前面的代码不变)

    // loop all the runList
    for (int idxRun = 0; idxRun < numberOfRun; idxRun++)
    {
        if (runlist[idxRun] != 559827 && runlist[idxRun] != 560090 && runlist[idxRun] != 560123)
            continue;


        std::cout << "Processing run " << runlist[idxRun] << std::endl;

        // 1. 创建输出文件
        TFile *output_file =
            TFile::Open(Form("correction_arpass3_small/arpass3_NUA_%d.root", runlist[idxRun]), "RECREATE");
        if (!output_file || output_file->IsZombie())
            continue;

        // 2. 使用 new 在堆上创建 GFWWeights，这样才能安全地交给 ROOT 写入
        GFWWeights *fWeightsREF = new GFWWeights("ccdb_object");
        fWeightsREF->init(true, false);

        // 3. 处理直方图投影
        auto grhtmp = (THnSparseF *)grh->Clone("tmp");

        grhtmp->GetAxis(0)->SetRange(idxRun + 1, idxRun + 1);
        TH3D *NUA = (TH3D *)grhtmp->Projection(1, 2, 3);

        // 【关键】切断 NUA 与当前目录的联系，防止它被 output_file 意外接管
        // 因为我们只是用它来临时读取数据，用完就删掉
        if (NUA)
            NUA->SetDirectory(nullptr);

        // 4. 填充权重 (逻辑不变)
        if (NUA)
        {
            for (int phibin = 1; phibin <= 60; phibin++)
            {
                for (int etabin = 1; etabin <= 64; etabin++)
                {
                    for (int vtxzbin = 1; vtxzbin <= 40; vtxzbin++)
                    {
                        double weight = NUA->GetBinContent(phibin, etabin, vtxzbin);
                        double phiuse = NUA->GetXaxis()->GetBinCenter(phibin);
                        double etause = NUA->GetYaxis()->GetBinCenter(etabin);
                        double vtxzuse = NUA->GetZaxis()->GetBinCenter(vtxzbin);
                        fWeightsREF->fill(phiuse, etause, vtxzuse, 1, 0, 0, weight);
                    }
                }
            }
        }

        // 5. 写入文件
        // 此时 Write 会把 fWeightsREF 的副本写入文件，或者将所有权转移给文件
        fWeightsREF->Write();

        // 6. 清理内存
        // 先关闭文件（此时文件会拥有 Write 进去的对象副本，我们不需要管）
        output_file->Close();
        delete output_file; // 记得 delete TFile 指针

        // 手动 delete 临时直方图
        if (NUA)
            delete NUA;

        // 手动 delete 我们 new 出来的权重对象
        // 注意：如果 Write 时使用了 kSingleKey 或特殊选项导致所有权转移，这里可能不需要 delete
        // 但在大多数标准情况下，Write 写入的是快照，原对象还需自己删除。
        // 如果不确定，可以在 Write 后打印 fWeightsREF->IsOnHeap() 查看。
        // 最稳妥的做法是 Write 之后，只要你不再用它，就 delete。
        delete fWeightsREF;
    }

    // 最后记得关闭最初的输入文件
    file->Close();
    delete file;
}
