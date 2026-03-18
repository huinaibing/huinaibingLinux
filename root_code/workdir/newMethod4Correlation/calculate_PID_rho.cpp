#include "PIDReader.h"


#define FILE "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/new_method_test.root"
#define DIRNAME "pid-flow-pt-corr"


void calculate_PID_rho()
{
    PIDReader rd(FILE, DIRNAME, 1);
    TCanvas *c1 = new TCanvas("c1", "", 800, 600);
    rd.get_res()->Draw();
}
