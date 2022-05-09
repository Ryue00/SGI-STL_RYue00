#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

void before() __attribute__((constructor));
void after() __attribute__((destructor));
void before(){
    printf("this is function %s\n",__func__);
    return;
}

void after(){
    printf("this is function %s\n",__func__);
    return;
}

typedef void *function ();

class CultureAttribute
{
    
}

int main(int argc, char** argv)
{
    //  std::cout << "hello world!" << std::endl;

        std::string strSource;
    char ch;
    cin >> strSource;
    cin >> ch;
    
    char count = 0;
    for (int i = 0; i < strSource.size(); i++)
    {
        if (ch == strSource[0])
            count ++;
    }
    cout << count;

    system("pause");
    return 0;
}

// 输出结果
// this is function before
// this is function main
// this is function after


