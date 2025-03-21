#include "Read_file.hpp"

void Read_file::ReadFileToVector(
    ifstream &input,
    vector<double> &px,
    vector<double> &py,
    vector<double> &pz,
    vector<double> &pe
)
{
    while (!input.eof())
    {
        string name;
        double dpx, dpy, dpz, dpe;
        input >> dpx >> dpy >> dpz >> dpe;
        px.push_back(dpx);
        py.push_back(dpy);
        pz.push_back(dpz);
        pe.push_back(dpe);
    }
}