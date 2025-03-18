#include <iostream>
#include <vector>
#include "cal_inv_mass_.h"

// #define __DEBUG__
using namespace std;


int main(int args, char** argv)
{
    if (args == 1)
    {
        cout << "invalid params number" << endl;
        return 1;
    }

    CalInvMass* cl = new CalInvMass(argv[1]);

    #ifdef __DEBUG__
    cl->test_my_class();
    #endif

    return 0;
}