#include <iostream>
#include <stdlib.h>
#include <stdio.h>

template <class T>
struct MyIter {
    typedef T value_type; // 内嵌型别声明
    T* ptr;
    MyIter(T* p = 0) : ptr(p) {}
    T& operator*() const { return *ptr; }
};

// class type
template <class T>
struct my_iterator_traits {
    typedef typename T::value_type value_type;
};

// 偏特化 1
template <class T>
struct my_iterator_traits<T*> {
    typedef T value_type;
};

// 偏特化 2
template <class T>
struct my_iterator_traits<const T*> {
    typedef T value_type;
};

// 首先询问 iterator_traits<I>::value_type,如果传递的 I 为指针,则进入特化版本,iterator_traits 直接回答;如果传递进来的 I 为 class type,就去询问 T::value_type.
template <class I>
typename my_iterator_traits<I>::value_type Func(I ite) {
    std::cout << "normal version" << std::endl;
    return *ite;
}
int main(int argc, const  char *argv[]) {

    MyIter<int> ite(new int(6));
    std::cout << Func(ite)<<std::endl;//print=> 6

    int *p = new int(7);
    std::cout<<Func(p)<<std::endl;//print=> 7

    const int k = 8;
    std::cout<<Func(&k)<<std::endl;//print=> 8
     system("pause");
}