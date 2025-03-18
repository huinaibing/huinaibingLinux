#include "cal_inv_mass_.h"
#include <fstream>
#include <math.h>
CalInvMass::CalInvMass(char* file_path)
{
    this->in = new ifstream(file_path);

    string _;
    float px;
    float py;
    float pz;
    float E;

    while (!this->in->eof())
    {
        vector<float> tmp;
        *in >> _ >> px >> py >> pz >> E;
        tmp.push_back(px);
        tmp.push_back(py);
        tmp.push_back(pz);
        tmp.push_back(E);
        this->datas.push_back(tmp);
        this->counter += 1;
    }
    
}

CalInvMass::~CalInvMass()
{
}

void CalInvMass::test_my_class()
// 测试这个类，如果output和原本输入两文件数据是一样的话那就没有问题
{
    ofstream out("text.output");
    for(int i=0; i < this->counter; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out << this->datas[i][j] << "\t";
        }
        out << endl;
    }
}

void CalInvMass::divide_data_by_pt()
{
    ofstream out1("0-10GeV.csv");
    ofstream out2("10-20GeV.csv");
    ofstream out3("20-30GeV.csv");
    ofstream out4("30-40GeV.csv");

    for (int i = 0; i < this->counter; i++)
    {
        
    }
    
}

float CalInvMass::calculate_inv_mass(float px, float py, float pz, float E)
{
    return sqrt(pow(E, 2) - pow(px, 2) - pow(py, 2) - pow(pz, 2));
}

void CalInvMass::calculate_inv_mass()
{
    for (int i=0; i < this->counter; i++)
    {
        this->data_inv_mass.push_back(
            this->calculate_inv_mass(
                this->datas[i][0],
                this->datas[i][1],
                this->datas[i][2],
                this->datas[i][3]
            )
        );
    }
}

float calculate_pt(float px, float py)
{
    return sqrt(pow(px, 2) + pow(py, 2));
}