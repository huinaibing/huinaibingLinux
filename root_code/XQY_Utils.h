#include<iostream>

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