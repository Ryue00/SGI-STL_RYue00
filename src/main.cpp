// #include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <cstdarg>
using namespace std;

void before() __attribute__((constructor));
void after() __attribute__((destructor));
void before()
{
    printf("this is function %s\n", __func__);
    return;
}

void after()
{
    printf("this is function %s\n", __func__);
    return;
}

typedef void *function();

//输出不定参数，默认为int
// params_count :参数个数：
bool PrintParams(int params_count,...)
{
	if (params_count < 0)
	{
		return 0;
	}
	//设置不定参数的类型
	va_list args_ptr;
	va_start(args_ptr,params_count); //设置补丁参数类型为int
    *args_ptr = 55;

	va_end(args_ptr);


}

//     int a = 100;
//     int b = 10;
//     int &pa = a;
//     int &&pb = std::move(b);
//     //std::cout << pa << std::endl;
//     //std::cout << pb << std::endl;
//     int sss = 10;
//    // PrintParams(1,sss);

//     int test1 = 10;
//     int&  test2 = test1;
//     int*  test3 = &test1;
//     std::cout << test1 << std::endl;
//     std::cout << &test1 << std::endl;
//     std::cout << test2 << std::endl;
//     std::cout << &test2 << std::endl;
//     std::cout << *test3 << std::endl;
//     std::cout << &test3 << std::endl;
class B
{
    void testB() const
    {
        std::cout << "testB" <<std::endl;
    }
}
class A : public B
{
    void testA()
    { 
        B::testB();
    }
}
int main(int argc, char **argv)
{
    //  std::cout << "hello world!" << std::endl;
    A a;
    system("pause");
    return 0;
}
// 输出结果
// this is function before
// this is function main
// this is function after
