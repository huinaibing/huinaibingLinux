#include <iostream>
#include "Utils.h"
#define hw1
#define hw2

using namespace std;

int main()
{
    #ifdef hw1
    float cel;
    cout << "输入一个摄氏度" << endl;
    cin >> cel;

    cout << Utils::cvrt_Cel_2_Fah(cel) << endl;
    #endif

    return 0;
}