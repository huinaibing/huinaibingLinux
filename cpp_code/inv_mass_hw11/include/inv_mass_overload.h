#ifndef __inv_mass_overload__
#define __inv_mass_overload__


class NewCalInvMass: public CalInvMass
{
public:
    void divide_data_by_pt();
};

#endif