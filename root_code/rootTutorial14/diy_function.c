#include "../XQY_Utils.h"

TH1D* diy_function()
{
    get_simple_canvas();
    TH1D* hist = get_simple_hist(100, 0, 10);

    TF1* my_func = new TF1("my_Func", "abs(sin(x))", 0, 10);
    // 显然，这是一个根据函数表达式画函数的函数

    for(int i = 0; i < 100000; i++) 
    {
        hist->Fill(my_func->GetRandom());
        // 这个抽样是把自定义的函数当成**分布函数**，然后在抽样，
        // 也就是TF1里面的函数越高的地方，对应的x就越容易被抽到
    }
    hist->Draw();

    return hist;
}