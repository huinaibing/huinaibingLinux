void random_seed_legend()
{
    TCanvas* c1 = new TCanvas("c1", "", 700, 500);

    TH1D* hist1 = new TH1D("gaus1", "", 100, 0, 20);
    TH1D* hist2 = new TH1D("gaus2", "", 100, 0, 20);

    int seed1 = gRandom->Integer(10000);
    int seed2 = gRandom->Integer(10000);
    // 产生两个随机数

    TRandom3* rand1 = new TRandom3(seed1);
    TRandom3* rand2 = new TRandom3(seed2);
    // 注意这里，之前我们是直接使用gRandom去调用gaus，
    // 但是这里我们先初始化一个生成器，并且给他一个seed

    for (int i = 0; i < 1000000; i++) 
    {
        hist1->Fill(rand1->Gaus(10, 2));
        hist2->Fill(rand2->Gaus(10, 3));
    }

    TLegend* leg = new TLegend(0.2, 0.5, 0.4, 0.7);
    // 初始化一个legend，
    // 显然前两个数字是左上角的坐标，后两个是右下角的坐标，都是0-1的

    leg->AddEntry(hist1, "word description mean = 2", "l");
    leg->AddEntry(hist2, "word description mean = 3", "l");
    // 显然，第一个参数是图线，如果你把图线颜色变了这里也会变，
    // 第二个是文字描述，第三个是神秘参数ppt没写

    hist1->SetLineColor(2);
    hist2->SetLineColor(4);
    // 设置线的颜色，在这里设置同时会改变legend的颜色

    hist1->Draw();
    hist2->Draw("same");
    leg->Draw("same");
    // 记得要画leg
}