void draw_3() 
{
    TCanvas* c1 = new TCanvas("c1", "c1 title", 700, 500);

    TH1D* hist_test = new TH1D("hist", "hist title", 100, 5000, 5400);
    hist_test->Fill(5201);

    TLine* line1 = new TLine(5000, 0.9, 5400, 0.9);
    // 这是两坐标，前面是线的起点，后面是线的终点
    line1->SetLineColor(4);
    // 颜色
    line1->SetLineWidth(2);
    // 宽度
    line1->SetLineStyle(3);
    // 样子

    hist_test->Draw("ep");
    // 和之前的一样，e代表误差棒子，p代表中心点

    line1->Draw("same");
    // 和latex的一样，先draw直方图在draw线

    c1->Print("./draw_3.png");
    // 保存成文件，把后缀换一下还可以是pdf等等
}