#ifndef __ANA_MASS__
#define __ANA_MASS__

#include<iostream>
#include<string>
#include<vector>
#include<math.h>
using namespace std;


class Ana_mass
{
public:
    double calMass(
        double px,
        double py, 
        double pz, 
        double pe
    );
};

#endif