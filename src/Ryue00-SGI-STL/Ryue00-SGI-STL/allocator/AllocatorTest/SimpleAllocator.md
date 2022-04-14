## 标准空间配置器接口
* 分配空间 allocate()
* 释放空间 deallocate()
* 构造对象 construct()
* 析构对象 destroy()

主要思想是将内存分配和构造分开来，使用::operator new分配空间、placement new构造对象，

[C++中使用placement new](https://blog.csdn.net/linuxheik/article/details/80449059)
