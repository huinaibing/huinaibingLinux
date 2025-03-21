#include "Ana_mass.hpp"
double Ana_mass::calMass(
    double px,
    double py, 
    double pz, 
    double pe
)
{
    return sqrt(pow(pe, 2) - pow(px, 2) - pow(py, 2) - pow(pz, 2));
}