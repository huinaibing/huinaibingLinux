


void draw_1()
{
    TCanvas* c1 = new TCanvas("c1", "c1 title", 700, 500);
    // 显然，第一个是id， 第二个是显示给大家的东西
    // 这个是画布
    TH1D* hist_test = new TH1D("hist", "hist title", 100, 5000, 5400);
    // 显然， 100是bin的数量， 5000 - 5400是坐标轴范围
    // TH1D后面的D表示数据类型为double

    hist_test->Fill(5201);

    hist_test->SetLineColor(3);
    // 2 是红色 3 是绿色
    hist_test->SetLineWidth(20);
    hist_test->SetLineStyle(3);
    // 2是点线 3是横直线
    // 显然，这些东西一个设置颜色，一个设置线的宽度，一个设置线的样式

    hist_test->SetMarkerColor(2);
    hist_test->SetMarkerStyle(24);
    hist_test->SetMarkerSize(1.2);
    // 设置数据点的样子，如果你的图没有数据点，那就没有了，所以在Draw里面要写个e参数，
    // 这个点是这一堆数据的中心点

    hist_test->SetTitle("");
    // 为什么要在这里设置？为什么不在定义的时候设置，是吃饱了称的吗
    hist_test->SetXTitle("this is x axis title p_{T}");
    // 函数如其名，好像可以用一些latex表达公式
    hist_test->SetYTitle("this is y axis title");


    hist_test->Draw("e");
    // e表示画error bar

    
    
    
}