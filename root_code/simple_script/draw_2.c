#include<iostream>
#include<stdio.h>

#include "TCanvas.h"
#include "TLatex.h"
#include "TH1.h" 


void draw_2()
{
    TCanvas* c1 = new TCanvas("c1", "c1 title", 700, 500);

    TH1D* hist_test = new TH1D("hist", "hist title", 100, 5000, 5400);
    hist_test->Fill(5201);

    TLatex* text1 = new TLatex(0.6, 0.7, "word");
    // 前两个参数是坐标，横纵都是使用0-1，第三个是想要输入的表达式，可以使用latex语法

    text1->SetTextFont(132);
    // 设置字体
    text1->SetTextSize(0.06);
    // 设置字的大小
    text1->SetNDC();
    // 据说是把字转化到0-1的坐标系下的，不写就看不到字

    hist_test->Draw();

    text1->Draw("same");
    // 一定要写这个，把latex画在和上面的直方图一个画布下
    // 先画直方图，再写这个
    
}