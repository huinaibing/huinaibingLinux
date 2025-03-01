#include "../XQY_Utils.h"
#include "./diy_function.c"
void simple_fit()
{
    TH1D* hist = diy_function();
    // 直接调用之前的函数，把abs(sin(x))画出来

    TF1* fit_function = get_gaus_fit();
    // 得到一个高斯函数

    fit_function->SetLineColor(4);

    hist->Fit(fit_function, "", "", 0, 3);
    // 这个fit会把你的fit function画在图上，
    // 根据fit function类型不同会使用不同拟合方法
    // 第一个参数是拟合函数，放函数的id或者指针都行
    // 第二个参数和第三个不知道，后两个是拟合的范围

    // 实际上完全不用初始化一个fit function指针，
    // 直接告诉Fit要拟合的函数类型即可，
    // 但是初始化指针的方法可以定义颜色
    // 如下面所示

    // hist->Fit("gaus", "", "", 3, 6);
}