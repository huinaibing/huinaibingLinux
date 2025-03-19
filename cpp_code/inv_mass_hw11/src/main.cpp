#include <iostream>
#include<unistd.h>
#include <string.h>
#include "inv_mass_overload.h"
#include "/home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/include/cal_inv_mass_.h"

// #define __DEBUG__
using namespace std;

int main(int argc, char** argv)
{
    char ch;
    char* input_file_name = new char[256];
    char* output_file_name = new char[256];

    while((ch = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (ch)
        {
            case 'i':
                #ifdef __DEBUG__
                cout << "i " << optarg << endl;
                #endif

                // input_file_name = string(optarg);
                strncpy(input_file_name, optarg, 255);
                
                break;
            case 'o':
                #ifdef __DEBUG__
                cout << "o " << optarg << endl;
                #endif
                
                // output_file_name = string(optarg);
                strncpy(output_file_name, optarg, 255);

                break;
            default:
                cout << "invalid parms" << endl;
                return 127;
        }
    }

    #ifdef __DEBUG__
    cout << input_file_name << "  ";
    // cout << output_file_name << endl;
    #endif

    #ifdef __DEBUG__
    CalInvMass* Cl = new CalInvMass("data_input.txt");
    Cl->test_my_class();
    #endif


    NewCalInvMass* Ctrler = new NewCalInvMass(input_file_name);

    // Ctrler->test_my_class();

    #ifdef __DEBUG__
    cout << "-----" << endl;
    //Ctrler->test_my_class();
    #endif
    return 0;
}