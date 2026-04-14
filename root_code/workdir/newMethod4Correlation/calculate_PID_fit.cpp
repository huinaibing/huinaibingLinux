#include "PIDReader.h"
#include "flowContainerReader.h"
#include "util.h"

// #define XQYFILE                                                                                                        \
//     "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
//     "smallsystem.root"

// #define XQYFILE                                                                                                        \
//     "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
//     "closure_right_small.root"

#define XQYFILE                                                                                                        \
    "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
    "pidTPCTOF_purity_big.root"

//#define XQYFILE "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/closure_test_smallzzhpass5.root"

#define XQYDIRNAME "pid-flow-pt-corr_TPCANDTOF"
// #define XQYDIRNAME "pid-flow-pt-corr"
#define XQYFLOWCONTAINERNAME "FlowContainerPi"
#define XQYPARTICLETYPE 1
#define OUTPUTXQYFILE "result_fit/res_test.root"
#define XQYMEANPTNAME "hPionMeanptWeightPidflow"

#define NSIGMA 3
#define INTNSIGMA 5


#pragma region // switch

// #define USE_ONE_FIT

#pragma endregion //switch

double strange_function(double *x, double *p)
{

    double term1 = (x[0] - p[1]) / p[2];
    double gauss1 = p[0] * std::exp(-0.5 * term1 * term1);

    // 第二个高斯函数 gaus(3): p[3] * exp( -0.5 * ((x-p[4])/p[5])^2 )
    double term2 = (x[0] - p[4]) / p[5];
    double gauss2 = p[3] * std::exp(-0.5 * term2 * term2);


    return (gauss1 + gauss2) *
           (x[0] * x[0] * x[0] * x[0] * p[6] + x[0] * x[0] * x[0] * p[7] + x[0] * x[0] * p[8] + x[0] * p[9] + p[10]);
}

// double strange_function(double *x, double *p)
// {

//     // --- 1. 单高斯函数部分 (参数: p[0]-p[2]) ---
//     // 公式: p[0] * exp( -0.5 * ((x-p[1])/p[2])^2 )
//     double term_gauss = (x[0] - p[1]) / p[2];
//     double gauss = p[0] * std::exp(-0.5 * term_gauss * term_gauss);

//     // --- 2. 朗道函数部分 (参数: p[3]-p[5]) ---
//     // 公式: p[3] * TMath::Landau(x, p[4], p[5])
//     // 其中:
//     //   p[3]: 朗道幅度 (归一化系数)
//     //   p[4]: 最可几值 (MPV, 峰位位置)
//     //   p[5]: 朗道宽度 (sigma)
//     double landau = p[3] * TMath::Landau(x[0], p[4], p[5]);

//     // --- 3. 三次多项式部分 (参数: p[6]-p[9]) ---
//     // 公式: p[6]*x^3 + p[7]*x^2 + p[8]*x + p[9]
//     double pol3 = p[6] * x[0] * x[0] * x[0] + p[7] * x[0] * x[0] + p[8] * x[0] + p[9];

//     // --- 4. 组合返回: (高斯 + 朗道) × 三次多项式 ---
//     return (gauss + landau) * pol3;
// }


void calculate_PID_fit()
{
#pragma region // init
    PIDReader *rd = new PIDReader(XQYFILE, XQYDIRNAME, XQYPARTICLETYPE);
    FlowContainerReader *fcReader_pid =
        new FlowContainerReader(XQYFILE, XQYDIRNAME, XQYFLOWCONTAINERNAME, XQYFLOWCONTAINERNAME, true);
    FlowContainerReader *fcReader_ch =
        new FlowContainerReader(XQYFILE, XQYDIRNAME, "FlowContainerCharged", "FlowContainerCharged", false);

    double centAxis[12] = {0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90};

    TH1D *rho_PID = new TH1D("hrhoPID", "", 11, centAxis);

    std::cout << XQYMEANPTNAME << std::endl;

    double res[11] = {0};
    double res_error[11] = {0};
#pragma endregion // init

#pragma region // val calculation
    for (int i = 1; i <= 11; i++)
    {
        // 计算rho的下半部分
        double varpt = fcReader_pid->get_var_meanpt(i);
        double varc22 =
            Utils::calculate_var_pid(i, fcReader_ch->get_profile2d_reader(), fcReader_pid->get_profile2d_reader());

        if (varpt == 0 || varc22 == 0)
            continue;
        double meanPtInCent = fcReader_pid->get_ptave(i);
        // end 计算rho的下半部分

        std::cout << "cent " << i << std::endl;
        std::cout << "varpt " << varpt << " varc22 " << varc22 << std::endl;
        std::cout << "===" << std::endl;

// get graph to fit
#ifdef USE_ONE_FIT
        auto valProfile = rd->getSpecifiedCentBin(i);
#else
        auto upProfile = rd->getUpGraph(i);
        auto downProfile = rd->getDownGraph(i);
#endif
        auto weightHist = rd->getdWeightdMeanpt(i);

        /// @note rebin
        // valProfile->Rebin(5);
        // weightHist->Rebin(5);


        // double gaus fit
        std::vector params = Utils::get_double_gaus_params(weightHist);
        auto doubleGaus = new TF1(Form("doubleGaus%d", i), strange_function, 0, 3, 10);
        doubleGaus->SetNpx(3000);

        std::cout << params[0] << params[1] << params[2] << params[3] << params[4] << params[5] << std::endl;
        doubleGaus->SetParameter(0, params[0]); // c
        doubleGaus->SetParameter(1, params[1]); // mean
        doubleGaus->SetParameter(2, params[2]); // width
        doubleGaus->SetParameter(3, params[3]); // c
        doubleGaus->SetParameter(4, params[4]); // mean
        doubleGaus->SetParameter(5, params[5]); // width

        // doubleGaus->SetParameters(1.2e16, // p0: 高斯幅度
        //                           0.88,   // p1: 高斯均值
        //                           0.05,   // p2: 高斯宽度
        //                           3e10,   // p3: 朗道幅度
        //                           0.80,   // p4: 朗道MPV
        //                           0.15,   // p5: 朗道宽度
        //                           1e-4,   // p6: x^3系数
        //                           -5e-3,  // p7: x^2系数
        //                           0.01,   // p8: x系数
        //                           1.0     // p9: 常数项
        // );


        weightHist->Fit(doubleGaus, "RSQ", "", 0.6, 1.2);
        // end double gaus fit

        // get the range to fit <2'>^2 / <2>
        double mu1 = params[1];
        double sigma1 = params[2];
        double mu2 = params[4];
        double sigma2 = params[5];

        double low1 = mu1 - NSIGMA * sigma1;
        double high1 = mu1 + NSIGMA * sigma1;
        double low2 = mu2 - NSIGMA * sigma2;
        double high2 = mu2 + NSIGMA * sigma2;
        // 取并集：最小左边界和最大右边界
        double xMin = TMath::Min(low1, low2);
        double xMax = TMath::Max(high1, high2);
        std::cout << "xMin " << xMin << " xmax " << xMax << std::endl;


#ifdef USE_ONE_FIT
        auto fitProfileFunc = new TF1(Form("func%d", i), "[0] * x * x + [1] * x + [2] + [3] * x * x", 0, 3);
        // auto fitProfileFunc = new TF1(Form("func%d", i), "[0] * exp([1] * x) + [2]", 0, 3);
        // auto fitProfileFunc = new TF1(Form("func%d", i), "[0]*x*x*x*x + [1]*x*x*x + [2]*x*x + [3]*x + [4]", 0, 3);
        // auto fitProfileFunc = new TF1(Form("func%d", i), "[0] + [1] * TMath::Power(x - [2], [3])", 0, 3);
        // auto fitProfileFunc = new TF1(Form("func%d", i), "[0] * x  + [1]", 0, 3);
        // fitProfileFunc->SetParameters(0.001, 0.03, 0.55, 2.2);
        fitProfileFunc->SetParameter(0, 0.2);
        fitProfileFunc->SetParameter(1, 0.2);
        valProfile->Fit(fitProfileFunc, "WLQ", "", xMin, xMax);
        // end get the range to fit <2'>^2 / <2>

        auto c1 = new TCanvas(Form("c1%d", i), "Val Profile Fit Result", 800, 600);
        c1->cd();
        c1->Clear();
        valProfile->Draw();
        c1->Update();
        c1->SaveAs(Form("fitgraph/c1_cent%d.png", i));
#else
        auto fitProfileUpFunc = new TF1(Form("funcUp%d", i), "[0] * x * x + [1] * x + [2]", 0, 3);
        auto fitProfileDownFunc = new TF1(Form("funcDown%d", i), "[0] * x * x + [1] * x + [2]", 0, 3);
        fitProfileUpFunc->SetParameters(0.2, -0.08, 0.04);
        fitProfileDownFunc->SetParameters(0.2, -0.03, 0.015);

        upProfile->Fit(fitProfileUpFunc, "WLQ", "", xMin, xMax);
        downProfile->Fit(fitProfileDownFunc, "WLQ", "", xMin, xMax);

        auto cComb = new TCanvas(Form("cComb%d", i), "Up & Down Comparison", 1600, 600);
        cComb->Divide(2, 1); // 分割为 1行2列

        // 画左边 (Up)
        cComb->cd(1);
        upProfile->Draw();
        gPad->SetTitle("Up Profile"); // 可选：设置Pad标题

        // 画右边 (Down)
        cComb->cd(2);
        downProfile->Draw();
        gPad->SetTitle("Down Profile"); // 可选：设置Pad标题
        cComb->SaveAs(Form("fitgraph/cComb_cent%d.png", i));
#endif

        auto c2 = new TCanvas(Form("c2%d", i), Form("Weight Histogram Fit Result%d", i), 800, 600);
        weightHist->SetMarkerStyle(22);
        c2->cd();
        c2->SetLogy();
        weightHist->Draw();
        c2->Update();
        c2->SaveAs(Form("fitgraph/c2_cent%d.png", i));
        // end get graph to fit

        double leftBound1 = mu1 - INTNSIGMA * sigma1;
        double rightBound1 = mu1 + INTNSIGMA * sigma1;
        double leftBound2 = mu2 - INTNSIGMA * sigma2;
        double rightBound2 = mu2 + INTNSIGMA * sigma2;
        // 取并集：最小左边界和最大右边界
        double xMin_int = TMath::Min(leftBound1, leftBound2);
        double xMax_int = TMath::Max(rightBound1, rightBound2);

        // intergral
        double int_weight = doubleGaus->Integral(xMin_int, xMax_int);
        std::cout << "int_weight " << int_weight << std::endl;

#ifdef USE_ONE_FIT
        auto cov_up = new TF1(
            Form("cov_up%d", i),
            [&meanPtInCent, &fitProfileFunc, &doubleGaus](double *x, double *p) {
                return doubleGaus->Eval(x[0]) * fitProfileFunc->Eval(x[0]) * (x[0] - meanPtInCent);
            },
            xMin_int,
            xMax_int,
            0);
#else
        auto cov_up = new TF1(
            Form("cov_up%d", i),
            [&meanPtInCent, &fitProfileUpFunc, &fitProfileDownFunc, &doubleGaus](double *x, double *p) {
                return doubleGaus->Eval(x[0]) * fitProfileUpFunc->Eval(x[0]) * fitProfileUpFunc->Eval(x[0]) *
                       (x[0] - meanPtInCent) / fitProfileDownFunc->Eval(x[0]);
            },
            xMin_int,
            xMax_int,
            0);
#endif


        double res_cov_up = cov_up->Integral(xMin_int, xMax_int);
        // std::cout << "cov_up " << res_cov_up << std::endl;
        std::cout << "cov centBIn " << i << " res" << res_cov_up / int_weight << std::endl;
        // endl intergral
        std::cout << "rho " << res_cov_up / int_weight / varc22 / varpt << std::endl;
        res[i - 1] = res_cov_up / int_weight / varc22 / varpt;
        // res[i - 1] = res_cov_up / int_weight;
    } // end loop all the cent bin
#pragma endregion // val calculation

    // return;

    // #pragma region // jack knife || bootstrap

    //     for (int idxbs = 0; idxbs < 30; idxbs++)
    //     {
    //         for (int i = 11; i >= 1; i--)
    //         {
    //             // 计算rho的下半部分
    //             double varpt = fcReader_pid->get_var_meanpt(i, idxbs);
    //             double varc22 = Utils::calculate_var_pid(i,
    //                                                      fcReader_ch->get_profile2d_reader(idxbs),
    //                                                      fcReader_pid->get_profile2d_reader(idxbs));

    //             if (varpt == 0 || varc22 == 0)
    //                 continue;
    //             double meanPtInCent = fcReader_pid->get_ptave(i, idxbs);
    //             // end 计算rho的下半部分

    //             // get graph to fit
    //             auto valProfile = rd->getSpecifiedCentBin(i, idxbs);
    //             auto weightHist = rd->getdWeightdMeanpt(i, idxbs);

    //             /// @note rebin
    //             // valProfile->Rebin(5);
    //             // weightHist->Rebin(5);

    //             // double gaus fit
    //             std::vector params = Utils::get_double_gaus_params(weightHist);
    //             auto doubleGaus = new TF1(Form("doubleGaus%dbs%d", i, idxbs), strange_function, 0, 3, 11);
    //             doubleGaus->SetNpx(3000);

    //             // std::cout << params[0] << params[1] << params[2] << params[3] << params[4] << params[5] << std::endl;
    //             doubleGaus->SetParameter(0, params[0]); // c
    //             doubleGaus->SetParameter(1, params[1]); // mean
    //             doubleGaus->SetParameter(2, params[2]); // width
    //             doubleGaus->SetParameter(3, params[3]); // c
    //             doubleGaus->SetParameter(4, params[4]); // mean
    //             doubleGaus->SetParameter(5, params[5]); // width


    //             weightHist->Fit(doubleGaus, "RSQ", "", 0, 3);
    //             // end double gaus fit

    //             // get the range to fit <2'>^2 / <2>
    //             double mu1 = params[1];
    //             double sigma1 = params[2];
    //             double mu2 = params[4];
    //             double sigma2 = params[5];

    //             double low1 = mu1 - NSIGMA * sigma1;
    //             double high1 = mu1 + NSIGMA * sigma1;
    //             double low2 = mu2 - NSIGMA * sigma2;
    //             double high2 = mu2 + NSIGMA * sigma2;
    //             // 取并集：最小左边界和最大右边界
    //             double xMin = TMath::Min(low1, low2);
    //             double xMax = TMath::Max(high1, high2);
    //             std::cout << "xMin " << xMin << " xmax " << xMax << std::endl;

    //             auto fitProfileFunc = new TF1(Form("func%dbs%d", i, idxbs), "[0] * x * x + [1] * x + [2]", 0, 3);
    //             // auto fitProfileFunc = new TF1(Form("func%d", i), "[0] * exp([1] * x) + [2]", 0, 3);
    //             // auto fitProfileFunc = new TF1(Form("func%d", i), "[0]*x*x*x*x + [1]*x*x*x + [2]*x*x + [3]*x + [4]", 0, 3);
    //             // auto fitProfileFunc = new TF1(Form("func%d", i), "[0] + [1] * TMath::Power(x - [2], [3])", 0, 3);
    //             // auto fitProfileFunc = new TF1(Form("func%d", i), "[0] * x  + [1]", 0, 3);
    //             // fitProfileFunc->SetParameters(0.001, 0.03, 0.55, 2.2);
    //             fitProfileFunc->SetParameter(0, 0.2);
    //             fitProfileFunc->SetParameter(1, 0.2);
    //             valProfile->Fit(fitProfileFunc, "WLQ", "", xMin, xMax);
    //             // end get the range to fit <2'>^2 / <2>

    //             // auto c1 = new TCanvas(Form("c1%d", i), "Val Profile Fit Result", 800, 600);
    //             // c1->cd();
    //             // c1->Clear();
    //             // valProfile->Draw();
    //             // c1->Update();
    //             // c1->SaveAs(Form("fitgraph/c1_cent%d.png", i));

    //             // auto c2 = new TCanvas(Form("c2%d", i), Form("Weight Histogram Fit Result%d", i), 800, 600);
    //             // weightHist->SetMarkerStyle(22);
    //             // c2->cd();
    //             // c2->Clear();
    //             // c2->SetLogy();
    //             // weightHist->Draw();
    //             // c2->Update();
    //             // c2->SaveAs(Form("fitgraph/c2_cent%d.png", i));
    //             // end get graph to fit

    //             double leftBound1 = mu1 - INTNSIGMA * sigma1;
    //             double rightBound1 = mu1 + INTNSIGMA * sigma1;
    //             double leftBound2 = mu2 - INTNSIGMA * sigma2;
    //             double rightBound2 = mu2 + INTNSIGMA * sigma2;
    //             // 取并集：最小左边界和最大右边界
    //             double xMin_int = TMath::Min(leftBound1, leftBound2);
    //             double xMax_int = TMath::Max(rightBound1, rightBound2);

    //             // intergral
    //             double int_weight = doubleGaus->Integral(xMin_int, xMax_int);
    //             std::cout << "int_weight " << int_weight << std::endl;

    //             auto cov_up = new TF1(
    //                 Form("cov_up%dbs%d", i, idxbs),
    //                 [&meanPtInCent, &fitProfileFunc, &doubleGaus](double *x, double *p) {
    //                     return doubleGaus->Eval(x[0]) * fitProfileFunc->Eval(x[0]) * (x[0] - meanPtInCent);
    //                 },
    //                 xMin_int,
    //                 xMax_int,
    //                 0);
    //             double res_cov_up = cov_up->Integral(xMin_int, xMax_int);
    //             // std::cout << "cov_up " << res_cov_up << std::endl;
    //             // std::cout << "cov centBIn " << i << " res" << res_cov_up / int_weight << std::endl;
    //             // endl intergral
    //             // std::cout << "rho " << res_cov_up / int_weight / varc22 / varpt << std::endl;
    //             double res_bs = res_cov_up / int_weight / varc22 / varpt;
    //             // double res_bs = res_cov_up / int_weight;
    //             res_error[i - 1] += TMath::Power(res_bs - res[i - 1], 2);
    //         } // end loop all the cent bin
    //     } // end all bootstrap

    //     for (int i = 0; i < 11; i++)
    //     {
    //         res_error[i] = TMath::Sqrt(res_error[i] / 29);
    //     }

    // #pragma endregion // jack knife || bootstrap

#pragma region // draw

    auto output_file = new TFile(OUTPUTXQYFILE, "RECREATE");
    auto res_hist = new TH1D("rho", "rho", 11, centAxis);
    for (int i = 0; i < 11; i++)
    {
        if (abs(res[i]) <= 1)
            res_hist->SetBinContent(i + 1, res[i]);
        res_hist->SetBinError(i + 1, res_error[i]);


        std::cout << "final res " << i << " " << res[i] << std::endl;
        std::cout << "final error " << i << " " << res_error[i] << std::endl;
    }
    res_hist->Write();
    output_file->Close();

#pragma endregion // draw
}
