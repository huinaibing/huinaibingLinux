#include "inv_mass_root.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "输入文件名字" << std::endl;
        return 1;
    }

    CalInvMassRoot* cal = new CalInvMassRoot(argv[1]);
    cal->draw_hist();
    return 0;
}