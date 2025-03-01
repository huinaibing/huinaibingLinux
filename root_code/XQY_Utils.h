#include<iostream>

#ifndef XQY_Utils
#define XQY_Utils

TCanvas* get_simple_canvas()
// 返回一个超级简单的canvas，大小700*500
// :return: 指向该canvas的指针
{
    return new TCanvas(
        std::to_string(gRandom->Integer(10000)).c_str(),
        // 随机生成一个id
        "simple_canvas",
        700,
        500
    );
}

TH1D* get_simple_hist(int binNum, float st, float ed)
// 返回一个超级简单的hist
// :params:
//      :binNum: bin的数量
//      :st: x轴的起始点
//      :ed: x轴的终点
// :return: 指向该hist的指针
{
    return new TH1D(
        std::to_string(gRandom->Integer(10000)).c_str(), 
        "simple_hist",
        binNum, 
        st, 
        ed
    );
}

TF1* get_gaus_fit(float st=0.0, float ed=0.0)
// 返回一个高斯拟合函数，用于拟合图线
// 这个st到ed的range是这个函数的range，实际拟合的时候还是用的被拟合函数的range
// ppt上可能有误/版本低了，目前这个st到ed对于拟合来说貌似没啥用
// :params: 
//      :st: x的起点
//      :ed: 终点
// :return: 指向该函数的指针
{
    return new TF1(
        std::to_string(gRandom->Integer(10000)).c_str(), 
        "gaus",
        st,
        ed
    );
}

#endif