#include "Utils.h"

float Utils::cvrt_Cel_2_Fah(float temper)
{
    return temper * 9 / 5 + 32;
}

template<class T1, class T2>
double Utils::average(T1 a, T2 b)
{
    return double((a + b) / 2)
}