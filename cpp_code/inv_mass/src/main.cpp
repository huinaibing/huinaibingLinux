#include<iostream>
#include"cal_inv_mass.h"

int main(int argc, char** argv)
{
    // 若参数数量不对则报错
    if (argc == 1)
    {
        std::cout << "invalid parmas number" << std::endl;
        return 127;
    }

    cal_inv_mass(argv[1]);
    
    return 0;
}