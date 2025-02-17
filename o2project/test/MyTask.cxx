#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"

struct MyTask : AnalysisTask
{
    Histogram etaHisto;

    void process(o2::aod::EtaPhi const&etaphi)
    {
        etaHisto.fill(etaphi.eta());
    }
};







WorkflowSpec defineDataProcessing() {
    return {
        adaptAnalysisTask<MyTask>(TaskName{"name111"});
    };
}