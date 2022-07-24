#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
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



int main(int argc, char** argv)
{
    //  std::cout << "hello world!" << std::endl;

    int a = 100;
    int b =10;
    int& pa = a;
    int&& pb = std::move(b);
    std::cout << pa << std::endl;

    std::cout << pb << std::endl;

    system("pause");
    return 0;
}
// 输出结果
// this is function before
// this is function main
// this is function after


