#include<iostream>
#include"ask_grade.h"
using namespace std;
int main()
{
    int grade;
    cout << "input your grade" << endl;
    cin >> grade;
    cout << ask_grade(grade) << endl;
}