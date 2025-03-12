#include <iostream>
#include "cal_inv_mass.h"
#include <fstream>
#include "math.h"

using namespace std;

void cal_inv_mass(char* file_path)
{

    float px;
    float py;
    float pz;
    float energy;
    string _;

    ifstream in(file_path);
    ofstream out("data_output.csv");

    while(!in.eof())
    {
        in >> _ >> px >> py >> pz >> energy;
        out << sqrt(pow(energy, 2) - pow(px, 2) - pow(py, 2) - pow(pz, 2)) << endl;
    }
}