#include "PIDReader.h"
#include "flowContainerReader.h"
#include "util.h"

#define XQYFILE                                                                                                        \
    "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/newMethod4Correlation/data4newmethod/"                \
    "pidTPCTOF_purity_big.root"
//#define XQYFILE "/home/huinaibing/git_repo/huinaibingLinux/root_code/workdir/datas/closure_test_smallzzhpass5.root"
#define XQYDIRNAME "pid-flow-pt-corr_TPCANDTOF"
#define XQYFLOWCONTAINERNAME "FlowContainerPi"
#define XQYPARTICLETYPE 1
#define OUTPUTXQYFILE "result/res_pion.root"
#define XQYMEANPTNAME "hPionMeanptWeightPidflow"
#include <deque>


std::deque<double> processDeque(std::deque<double> original, int n)
{
    // 1. 第一步：删除 <0.57 和 >0.84 的元素
    // 因为是单调递增，直接找边界迭代器
    auto it_low = std::lower_bound(original.begin(), original.end(), 0.60);
    auto it_high = std::upper_bound(original.begin(), original.end(), 0.8);

    // 将有效范围复制到临时 deque 中进行后续处理
    std::deque<double> temp(it_low, it_high);

    // 2. 第二步：准备结果容器
    std::deque<double> result;

    // 定义区间边界
    const double zone1_start = 0.60, zone1_end = 0.64;
    const double zone2_start = 0.76, zone2_end = 0.80;

    int skip_counter = 0;            // 用于记录当前需要跳过（删除）的元素数量
    bool is_processing_zone = false; // 标记当前是否在需要抽样的区间内

    // 3. 第三步：遍历并处理
    for (double num : temp)
    {
        // 判断是否在目标抽样区间内
        bool in_zone = (num >= zone1_start && num <= zone1_end) || (num >= zone2_start && num <= zone2_end);

        if (in_zone)
        {
            // 如果刚进入抽样区间，重置计数器
            if (!is_processing_zone)
            {
                is_processing_zone = true;
                skip_counter = 0; // 0表示当前这个要保留
            }

            // 核心抽样逻辑：保留1个，跳过n个
            if (skip_counter == 0)
            {
                result.push_back(num); // 保留
                skip_counter = n;      // 接下来要跳过 n 个
            }
            else
            {
                skip_counter--; // 跳过（删除）当前元素
            }
        }
        else
        {
            // 在中间区域 (0.64, 0.76)，全部保留
            is_processing_zone = false;
            result.push_back(num);
        }
    }

    // result.push_back(3);
    // result.push_front(0);

    return result;
}


void calculate_PID_rho()
{
    std::deque<double> dq;
    for (double i = 0.0; i <= 3.0; i += 0.001)
    {
        dq.push_back(i);
    }

    std::cout << dq.size() << "dq.size" << std::endl;
    std::deque<double> result = processDeque(dq, 5);

    std::cout << result.size() << "result.size" << std::endl;

    double *arr = new double[result.size()];
    std::copy(result.begin(), result.end(), arr);

    for (int i = 0; i < result.size(); i++)
    {
        std::cout << arr[i] << std::endl;
    }

    PIDReader *rd = new PIDReader(XQYFILE, XQYDIRNAME, XQYPARTICLETYPE);
    FlowContainerReader *fcReader_pid =
        new FlowContainerReader(XQYFILE, XQYDIRNAME, XQYFLOWCONTAINERNAME, XQYFLOWCONTAINERNAME, true);
    FlowContainerReader *fcReader_ch =
        new FlowContainerReader(XQYFILE, XQYDIRNAME, "FlowContainerCharged", "FlowContainerCharged", false);


    double centAxis[12] = {0, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90};

    TH1D *rho_PID = new TH1D("hrhoPID", "", 11, centAxis);

    double res[11] = {0};

    std::cout << XQYMEANPTNAME << std::endl;

    /// @note this for loop is used to calculate center val
    for (int i = 1; i <= 11; i++)
    {
        double varpt = fcReader_pid->get_var_meanpt(i);
        double varc22 =
            Utils::calculate_var_pid(i, fcReader_ch->get_profile2d_reader(), fcReader_pid->get_profile2d_reader());

        if (varpt == 0 || varc22 == 0)
            continue;

        double meanPtInCent = fcReader_pid->get_ptave(i);
        double covInCent = Utils::get_cov(rd->getSpecifiedCentBin(i), // arr, result.size() - 1),
                                          meanPtInCent);
        //rd->getSpecifiedCentBin(i, XQYMEANPTNAME)); //, result.size() - 1, arr));

        std::cout << covInCent << " cov" << std::endl;

        res[i] = covInCent / varpt / varc22;
    }
    // end center val

    double bserror[11] = {0};

    // /// @note bootstrap
    // for (int bsIDX = 0; bsIDX < 30; bsIDX++)
    // {
    //     std::cout << "=====================" << std::endl;
    //     for (int i = 1; i <= 11; i++)
    //     {
    //         double varpt = fcReader_pid->get_val_meanpt_jackknife(i, bsIDX);
    //         double varc22 = Utils::calculate_var_pid(i,
    //                                                  fcReader_ch->get_profile2d_reader_jackknife(bsIDX),
    //                                                  fcReader_pid->get_profile2d_reader_jackknife(bsIDX));

    //         if (varpt == 0 || varc22 == 0)
    //             continue;

    //         double meanPtInCent = fcReader_pid->get_ptave_jackknife(i, bsIDX);
    //         double covInCent = Utils::get_cov(rd->getSpecifiedCentBin_jackknife(i, bsIDX), meanPtInCent);
    //         //rd->getSpecifiedCentBin_jackknife(i, XQYMEANPTNAME, bsIDX));

    //         double res_bs = covInCent / varpt / varc22;


    //         std::cout << "------------------------" << std::endl;
    //         std::cout << "covInCent = " << covInCent << std::endl;
    //         std::cout << "varpt     = " << varpt << std::endl;
    //         std::cout << "varc22    = " << varc22 << std::endl;
    //         std::cout << "res_bs " << res_bs << std::endl;


    //         bserror[i - 1] += TMath::Power(res_bs - res[i - 1], 2);
    //     }
    // }

    // for (int i = 0; i < 11; i++)
    // {
    //     bserror[i] = TMath::Sqrt(bserror[i] / 29);
    // }
    // // end bootstrap

    for (int i = 1; i <= 11; i++)
    {
        if (std::abs(res[i]) <= 1)
        {
            rho_PID->SetBinContent(i, res[i]);
            rho_PID->SetBinError(i, bserror[i]);
        }
    }

    TFile *res_file = TFile::Open(OUTPUTXQYFILE, "RECREATE");
    rho_PID->Write();
    res_file->Close();

    std::cout << "1111111111111111111111============================" << std::endl;
}
