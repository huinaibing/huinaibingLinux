#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"

struct MyTask : AnalysisTask
{
    // Filter<Tracks> ptFilter = track::pt > 1;
    SliceCache cache;
    Partition<Tracks> leftTracks = track::pt < 0;
    Partition<Tracks> rightTracks = track::pt >= 0;
    HistogramRegistry histos("histos", {});
    AxisSpec phiAxis = {100, 0., 2, *M_PI};
    Configurable<int> nBinPhi(
        /*id*/"nBinPhi", 
        /*number*/100, 
        /*description*/"N bins");

    init () {
        histos.add(
        /*id*/    "hphi",
        /*title*/    "hphi", 
        /*table definition*/{HistType::KTH1F, {phiAxis}}
        );
    }

    // void process(soa::Filtered<o2::aod::Tracks> &tracks)
    // process(
    //     o2::aod::Collision const& collision, 
    //     o2::aod::Tracks &tracks
    //     )
    process(
        o2::aod::Collision const& collision,
        soa::Join<o2::aod::Tracks, o2::aod::TracksExtras> const& myTracks
    )
    {
        auto leftColl = leftTracks->sliceByCached(
            aod::track::collisionId, 
            collision.globalIndex(),
            cache
        );
        for (auto track : leftColl) 
        {
            histos.fill(HIST("hphi", track.phi()));
        }
    }
};







WorkflowSpec defineDataProcessing() {
    return {
        adaptAnalysisTask<MyTask>(TaskName{"name111"});
    };
}





