#include "TFile.h"
#include "TTree.h"
#include "Ana_mass.hpp"
#include "Read_file.hpp"
using namespace std;

int main()
{
    Ana_mass* mass_algo = new Ana_mass();
    Read_file* read_file = new Read_file();

    ifstream input("../DATA_FILES/data_input.txt");
    vector<double> px_vec;
    vector<double> py_vec;
    vector<double> pz_vec;
    vector<double> pe_vec;

    read_file->ReadFileToVector(input, px_vec, py_vec, pz_vec, pe_vec);
    
    vector<double> mass_vec;

    for (int i = 0; i < px_vec.size(); i++)
    {
        mass_vec.push_back(
            mass_algo->calMass(
                px_vec[i],
                py_vec[i],
                pz_vec[i],
                pe_vec[i]
            )
        );
    }

    TFile* file_group = new TFile(
        "bmass_test.root",
        "recreate"
    );

    TTree* tree = new TTree("myTuple", "myTuple");
    // 新建一颗树
    // 显然，第一个是id，给机器看的，
    // 第二个是title之类的给人看的

    double bmass_tmp, bpx_tmp, bpy_tmp, bpz_tmp, bpe_tmp;

    tree->Branch("Mass", &bmass_tmp, "Mass/D");
    // 第一个是id，第二个是变量的地址
    // 为什么要地址？看后面就知道了
    // 第三个是Tree里面的名字以及变量类型
    // 这里有个问题，第一个名字和后面一个名字分别是用在哪里的名字？
    // D代表double
    tree->Branch("Px", &bpx_tmp, "Px/D");
    tree->Branch("Py", &bpy_tmp, "Py/D");
    tree->Branch("Pz", &bpz_tmp, "Pz/D");
    tree->Branch("Pe", &bpe_tmp, "Pe/D");

    for (int i = 0; i < mass_vec.size(); i ++)
    {
        bmass_tmp = mass_vec[i];
        bpx_tmp = px_vec[i];
        bpy_tmp = py_vec[i];
        bpz_tmp = pz_vec[i];
        bpe_tmp = pe_vec[i];
        tree->Fill();
        // 显然，Fill函数会直接去访问这些变量的地址
        // 然后去填写数值
    }
    file_group->Write();
    // 显然，这个函数会把所有的Tree全部写入文件
    file_group->Close();
}