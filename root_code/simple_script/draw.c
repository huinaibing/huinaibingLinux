void draw()
{
    TCanvas* c1 = new TCanvas("c1", "c1 title", 700, 500);
    // 显然，第一个是id， 第二个是显示给大家的东西
    // 这个是画布
    TH1D* hist_test = new TH1D("hist", "hist title", 100, 5000, 5400);
    // 显然， 100是bin的数量， 5000 - 5400是坐标轴范围
    // TH1D后面的D表示数据类型为double

    hist_test->Fill(5201);

    hist_test->Draw();
}