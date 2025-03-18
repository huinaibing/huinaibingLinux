#ifndef __UTILS__
#define __UTILS__


class Utils
{
public:
    static float cvrt_Cel_2_Fah(float temper);

    template<class T1, class T2>
    static double average(T1 a, T2 b);
};

#endif