#include <iostream>
#include "Utils.h"

//#define __hw1__
#define __hw2__

using namespace std;

int main()
{
    #ifdef __hw1__
    float cel;
    cout << "输入一个摄氏度" << endl;
    cin >> cel;

    cout << Utils::cvrt_Cel_2_Fah(cel) << endl;
    #endif

    #ifdef __hw2__
    long a = 3;
    long b = 4;
    float c = 5;
    float d = 6;
    cout << "average of a, b\t" << Utils::average(a, b) << endl;
    cout << "average of c, d\t" << Utils::average(c, d) << endl;
    #endif

    return 0;
}