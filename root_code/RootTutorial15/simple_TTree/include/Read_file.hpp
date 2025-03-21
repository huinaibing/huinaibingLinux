#ifndef __READ_FILE__
#define __READ_FILE__

#include<fstream>
#include<vector>
using namespace std;

class Read_file{
public:
    // Read_file();
    // ~Read_file();

    void ReadFileToVector(
        ifstream &input,
        vector<double> &px,
        vector<double> &py,
        vector<double> &pz,
        vector<double> &pe
    );
};

#endif