#include<iostream>

void random_test()
{
    for (int i = 0; i < 10 ; i++)
    {
        std::cout << gRandom->Uniform(0.5) << endl;
        // 显然，这个东西由于seed的问题所以永远输出的一样的东西
        // Uniform里面接受一个数字，这个就是seed
    }
}