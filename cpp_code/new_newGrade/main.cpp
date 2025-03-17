#include <iostream>
using namespace std;

int main()
{
    int grade;
    
    do
    // 若用户输入成绩不在0-100区间就让用户重新输入
    {
        cout << "enter a grade" << endl;
        cin >> grade;
        if (grade > 100 || grade < 0)
        {
            cout << "invalid grade number (0 - 100)" << endl;
            continue;
        }

        if (grade < 60)
        {
            cout << "Fail" << endl;
        }
        else if (grade < 80)
        {
            cout << "c grade" << endl;
        }
        else if (grade < 90)
        {
            cout << "b grade" << endl;
        }
        else
        {
            cout << "a grade" << endl;
        }
    }while (grade > 100 || grade < 0);
        
    return 0;
}