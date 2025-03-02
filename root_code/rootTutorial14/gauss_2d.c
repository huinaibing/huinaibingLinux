#include "../XQY_Utils.h"

void gauss_2d()
{
    get_simple_canvas();

    TH2D* hist_gaus = new TH2D(
        "2d", "",
        100, 0, 20,
        100, 0, 20
    );
    // 2d的hist，参数意义是什么一目了然

    for (int i=0; i < 10000000; i++)
    {
        hist_gaus->Fill(
            gRandom->Gaus(10, 5),
            gRandom->Gaus(10, 5)
        );
        // 显然，二维的图需要一个坐标，即两个数
    }

    hist_gaus->Draw("COLZ");
    // 这是一种彩色的等级图
    // 使用拟合的方法和一维图一样，但是尚不清楚bigaus和xygaus的区别
}