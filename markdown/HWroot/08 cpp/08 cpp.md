![image-20250311101433106](./img/08%20cpp/image-20250311101433106.png)

如下图所示，这是Sams书上的`NewGrader`程序原版，题目需要把它改成不带return（除了最后一个）的版本

![image-20250312114334157](./img/08%20cpp/image-20250312114334157.png)

修改后如下

```cpp
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
```

![image-20250312120025634](./img/08%20cpp/image-20250312120025634.png)

运行：

![image-20250312120203045](./img/08%20cpp/image-20250312120203045.png)

![image-20250312120252671](./img/08%20cpp/image-20250312120252671.png)

![image-20250312120328586](./img/08%20cpp/image-20250312120328586.png)

---

---

![image-20250311101528681](./img/08%20cpp/image-20250311101528681.png)

项目结构

![image-20250311182552375](./img/08%20cpp/image-20250311182552375.png)

`ask_grade.cpp`

![image-20250311182648710](./img/08%20cpp/image-20250311182648710.png)

`main.cpp`

![image-20250311182713128](./img/08%20cpp/image-20250311182713128.png)

`CMakeLists.txt`

![image-20250311183158271](./img/08%20cpp/image-20250311183158271.png)

输出结果

![image-20250311182845202](./img/08%20cpp/image-20250311182845202.png)

