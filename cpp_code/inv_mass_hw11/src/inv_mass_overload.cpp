#include "inv_mass_overload.h"
#include "/home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/include/cal_inv_mass_.h"
#include <fstream>
#include <iostream>
#include <vector>

// #define __DEBUG__
using namespace std;


void NewCalInvMass::divide_data_by_pt(int range=40, int gap=2, const char* basename="output")
// 默认0-40GeV 每2GeV一个文件
{
    vector<ofstream*> file_outers;
    
    for (int i = 0; i < range; i +=gap)
    {
        string name(basename);
        name += to_string(i);
        name += string("to");
        name += to_string(i + gap);
        file_outers.push_back(new ofstream(name));
    }

    int divider;
    for (auto i: this->datas)
    {
        divider = this->calculate_pt(i[0], i[1]) / (gap * 1000);

        if (divider >= range / gap)
        {
            continue;
        }

        #ifdef __DEBUG__
        cout << divider << endl;
        #endif

        *file_outers[divider] << this->calculate_inv_mass(i[0], i[1], i[2], i[3]) << endl;
    }

    #ifdef __DEBUG__
    cout << "------";
    #endif
    for (ofstream* i: file_outers)
    // 关闭文件
    {
        i->close();
    }
}
