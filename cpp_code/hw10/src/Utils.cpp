#include "Utils.h"

float Utils::cvrt_Cel_2_Fah(float temper)
{
    return temper * 9 / 5 + 32;
}

float Utils::average(long a, long b)
{
    return (a + b) / 2.0;
}

float Utils::average(float a, float b)
{
    return (a + b) / 2;
}