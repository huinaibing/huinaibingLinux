#ifndef __inv_mass_overload__
#define __inv_mass_overload__

#include "/home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/include/cal_inv_mass_.h"
// #include "/home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/src/cal_inv_mass_.cpp"

class NewCalInvMass: public CalInvMass
{
    using CalInvMass::CalInvMass;
public:
    void divide_data_by_pt(int, int, const char*);
};

#endif