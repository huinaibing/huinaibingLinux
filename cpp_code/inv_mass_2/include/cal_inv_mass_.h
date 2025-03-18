#ifndef __cal_inv_mass__
#define __cal_inv_mass__

#include<iostream>
#include<fstream>
#include<vector>
using namespace std;


class CalInvMass 
{
private:
    ifstream* in;
    vector<vector<float>> datas;
    vector<float> data_inv_mass;
    int counter = 0;

public:
    CalInvMass(char* file_path);
    ~CalInvMass();
    void divide_data_by_pt();
    float calculate_inv_mass(float px, float py, float pz, float E);
    void calculate_inv_mass();
    void test_my_class();
};


#endif