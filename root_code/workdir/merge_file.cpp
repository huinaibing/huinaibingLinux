#include "TFileMerger.h"

void merge_file()
{
    TFileMerger merger;
    merger.SetPrintLevel(1); // Optional: Set print level for logging
    merger.AddFile("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_544095.root");
    merger.AddFile("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_544122.root");
    merger.AddFile("/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/AnalysisResults_544124.root");
    // Add more files as needed

    bool mergeStatus = merger.Merge("merged_output.root");
    merger.CloseOutputFile();
}