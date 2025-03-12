#include <iostream>

using namespace std;


string ask_grade(int grade)
{
    if (grade < 60)
    {
        return string("Fail");
    } else if (grade < 80)
    {
        return string("normal");
    }else if (grade < 90)
    {
        return string("nice");
    }else
    {
        return string("excellent");
    }
}

