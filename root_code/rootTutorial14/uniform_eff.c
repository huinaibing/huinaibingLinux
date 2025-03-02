#include "../XQY_Utils.h"

void uniform_eff()
{
    TFile*  file_new = new TFile("eff_test.root", "recreate");
    // 显然，第一个参数是文件的名字，第二个是对文件的操作，
    // recreate就是如果文件存在就重新创建

    LazyContainer* hist_container = get_th1d_and_canvas(
    );

    TEfficiency* eff = new TEfficiency(
        "eff", "eff",
        100, 0, 10
    );
    // 这就是所谓的效率图，用处不明

    TF1* sin = new TF1("sin", "abs(sin(x))", 0, 10);

    int tmp;

    for (int i=0; i < 1000000; i ++)
    {
        // hist_container->simple_th1d->Fill(gRandom->Uniform());
        // 上面的先放在一边
        
        
    }

    

}