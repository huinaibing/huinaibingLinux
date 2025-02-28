void uniform_random()
{
    TCanvas* c1 = new TCanvas("c1", "canvas title", 700, 500);

    TH1D* hist = new TH1D("hist_uniform", "title", 100, 0, 1);
    // binNum = 100, range [0, 1]

    

    for (int i = 0; i < 1000000; i ++)
    {
        // hist->Fill(gRandom->Uniform());
        // 这个是0-1均匀抽样

        // hist->Fill(gRandom->Gaus(0.5, 1));
        // 这个一眼高斯，第一个参数是均值，第二个是sigma

        // hist->Fill(gRandom->Exp(5));
        // 这个是指数分布，参数是Tau，我猜测应该是exp(Tau * x)这样的


    }

    // cout << gRandom->Integer(10000);
    // 生成一个小于参数的正整数，他一定是正的（我没见到负数）

    hist->Draw("ep");
}