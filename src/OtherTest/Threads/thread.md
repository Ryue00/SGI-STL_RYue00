# 并发与多线程

## 2.1 并发基本概念及实现，进程、线程基本概念
### 并发
两个或者更多的任务同时发生：一个程序同时之性能多个独立的任务

### 可执行程序
磁盘上的一个文件，如，windows下的 .exe文件

### 进程
系统进行资源分配和调度的基本单位，是操作系统结构的基础。

### 线程
操作系统能够进行运算调度的最小单位。它被包含在进程之中，是进程中的实际运作单位。
一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每条线程并行执行不同的任务。

## 2.2 线程启动、结束，创建线程多法、join，detach

### thread 类的简单介绍


| 函数名 | 功能 |
|--|--|
| thread() | 构造一个线程对象，没有关联任何线程函数，即没有启动任何线程 |
| thread(fn, args1, args2, …) | 构造一个线程对象，并关联线程函数fn，args1，args2，…为线程函数的参数
| get_id() | 获取线程id
| jionable() | 线程是否还在执行，joinable代表的是一个正在执行中的线程。
| jion() | 该函数调用后会阻塞住线程，当该线程结束后，主线程继续执行
| detach() |在创建线程对象后马上调用，用于把被创建线程与线程对象分离开，分离的线程变为后台线程，创建的线程的"死活"就与主线程无关

## 2.3 线程传参详解,成员函数做线程函数
### 线程函数

* 函数指针
* lambda表达式
* 函数对象
``` cpp

void Fun1()
{
    std::cout << "函数指针作线程函数" << std::endl;
    return;
}

class Fun3
{
public:
    void operator()()
    {
        std::cout << "类对象作线程函数" << std::endl;
    }
};

class Fun4
{
public:
    void ThreadWork(int num)
    {
        std::cout << "类成员函数作线程函数 " << num << std::endl;
    }
};

int main()
{
    // 函数指针作线程函数
    std::thread t1(Fun1);
    t1.join();

	// lambda 表达式作线程函数
    std::thread t2([]() {std::cout << "lambda 表达式作线程函数" << std::endl; });
    t2.join();

	// 类对象作线程函数
    Fun3 f3;
    std::thread t3(f3);
    t3.join();

	// 类成员函数作线程函数
    Fun4 f4;
    std::thread t4(&Fun4::ThreadWork,f4,100);  
    t4.join();
	
    return 0;
}

```
### thread类是防拷贝的，不允许拷贝构造以及赋值，但是可以移动构造和移动赋值，即将一个线程对象关联线程的状态转移给其他线程对象，转移期间不影响线程的执行。
### 可以通过jionable()函数判断线程是否是有效的，如果是以下任意情况，则线程无效
* 采用无参构造函数构造的线程对象
* 线程对象的状态已经转移给其他线程对象
* 线程已经调用jion或者detach结束

### 并发与并行的区别？
* 并行是指两个或者多个事件在同一时刻发生；而并发是指两个或多个事件在同一时间间隔发生。
* 并行是在不同实体上的多个事件，并发是在同一实体上的多个事件。
* 在一台处理器上“同时”（这个同时实际上市交替“”）处理多个任务，在多台处理器上同时处理多个任务

### 线程函数传参
线程函数参数是以值拷贝的方式传入线程的栈空间，所以即使传入的是引用类型，在线程修改也无法对外部实参改变，并且一些编译器会被报错.

```cpp
void Fun1(int& num)
{
    num += 10;
    std::cout << "void Fun1(int& num) " << num <<  std::endl;
}

void Fun2(int* num)
{
    (*num) += 10;
    std::cout << "void Fun2(int* num) " << *num << std::endl;
}

void Fun3(std::unique_ptr<int> num)
{
    *num += 10;
    std::cout << "void Fun3(int* num) " << *num << std::endl;
}

void Fun4(std::shared_ptr<int> num)
{
    *num += 10;
    std::cout << "void Fun4(int* num) " << *num << std::endl;
    std::cout << "numSharedPtr.use_count()=" << num.use_count() << std::endl;
}

int main()
{
    int num = 10;

    // std::thread t0(Fun1, num); // 编译错误
    // t0.join();
    // std::cout << "int main() " << num << std::endl;

    // 引用传递的正确用法，需要借助std::ref() 函数
    std::thread t1(Fun1, std::ref(num)); // 正确， 同样的使用还有 std::bind() 的引用传参
    t1.join();
    std::cout << "int main() " << num << std::endl;

    // 地址传递
    std::thread t2(Fun2,&num);
    t2.join();
    std::cout << "int main() " << num << std::endl;

    // 传递独占式智能指针，借助std::move()函数完成语义转换，否则编译失败
    std::unique_ptr<int> numUniquePtr = std::make_unique<int>(100);
    std::thread t3(Fun3, std::move(numUniquePtr));
    t3.join();
    std::cout << "int main() " << numUniquePtr << std::endl;

    // 传递共享智能指针
    std::shared_ptr<int> numSharedPtr = std::make_shared<int>(100);
    std::cout << "numSharedPtr.use_count()=" << numSharedPtr.use_count() << std::endl;
    std::thread t4(Fun4, numSharedPtr);
    t4.join();
    std::cout << "numSharedPtr.use_count()=" << numSharedPtr.use_count() << std::endl;
    std::cout << "int main() " << numSharedPtr << std::endl;

    return 0;
}
```

## 2.4 创建多个线程、数据共享问题分析、案例代码
### 创建多个线程
使用容器管控线程
```cpp

std::vector<std::thread> threads;

void OnPrint(int i)
{
    std::cout << std::this_thread::get_id() << "--> i = " << i << std::endl;
}

constexpr int THREAD_COUNT_MAX = 10;
int main()
{
    for (int i= 0; i< THREAD_COUNT_MAX; i++ )
    {
        threads.push_back(std::thread(OnPrint, i));
    }
    std::vector<std::thread>::iterator ite = threads.begin();
	for (;ite != threads.end(); ++ite)
	{
        ite->join();
	}
    return 0;
}
```

### 数据共享问题分析

* only read data：安全稳定,不需要特殊处理
* read and write data：恶性条件竞争
C++标准提供保护共享数据的最基本方式就是互斥量mutex，加锁

## 2.5 互斥量概念、用法、死锁演示及解决详解
### 概念、用法
std::mutex 互斥是C++最基本的保护共享数据的措施，需要妥善组织和编排代码
```cpp

std::list<int> m_testList;
std::mutex m_data_mutex;  // 互斥量，一把锁
void CSharedData::RecvMsgProcess()
{
	for (int i = 0; i < 10000; i++)
	{
		// std::unique_lock<std::mutex> lock(m_data_mutex); 
		m_data_mutex.lock();
		if (!m_testList.empty())
		{
			int data = m_testList.front();
			std::cout << "线程ID=" << std::this_thread::get_id() << "收到消息：" << data << std::endl;
			m_testList.pop_front();
		}
		m_data_mutex.unlock();
		// lock.unlock();
	}
	
}

void CSharedData::SendMsgProcess()
{
	for (int i =0; i < 10000; i++)
	{
		// std::unique_lock<std::mutex> lock(m_data_mutex);
		m_data_mutex.lock();
		std::cout << "线程ID=" << std::this_thread::get_id() << "发送消息：" << i << std::endl;
		m_testList.push_back(i);
		// lock.unlock();
		m_data_mutex.unlock();
	}
}

```

### 死锁

当至少两个线程，都需同时锁住两个互斥量，才能进行某项操作，但两个线程都分别只锁住了一个互斥，都在等待另一个互斥加锁。
此为死锁。

```cpp
std::list<int> m_testList;
std::mutex m_data_mutex1;  // 互斥量，一把锁
std::mutex m_data_mutex2;  // 互斥量，一把锁
void CSharedData::RecvMsgProcess()
{
	for (int i = 0; i < 10000; i++)
	{
         // std::lock_guard<std::mutex> lock(m_data_mutex1);
         // std::lock_guard<std::mutex> lock(m_data_mutex2);
		// std::unique_lock<std::mutex> lock(m_data_mutex1);
		m_data_mutex2.lock();  // 顺序不一致，导致死锁
		m_data_mutex1.lock();
		if (!m_testList.empty())
		{
			int data = m_testList.front();
			std::cout << "线程ID=" << std::this_thread::get_id() << "收到消息：" << data << std::endl;
			m_testList.pop_front();
		}
		m_data_mutex1.unlock();
		m_data_mutex2.unlock();
		// lock.unlock();
	}
	
}

void CSharedData::SendMsgProcess()
{
	for (int i =0; i < 10000; i++)
	{
		// std::unique_lock<std::mutex> lock(m_data_mutex1);
		m_data_mutex1.lock();  // 顺序不一致，导致死锁
		m_data_mutex2.lock();
		std::cout << "线程ID=" << std::this_thread::get_id() << "发送消息：" << i << std::endl;
		m_testList.push_back(i);
		// lock.unlock();
		m_data_mutex2.unlock();
		m_data_mutex1.unlock();
	}
}

造成示例代码这种死锁，一般的解决方法可以使用 std::lock() 模板函数如下：
```cpp
std::list<int> m_testList;
std::mutex m_data_mutex1;  // 互斥量，一把锁
std::mutex m_data_mutex2;  // 互斥量，一把锁
void CSharedData::RecvMsgProcess()
{
	for (int i = 0; i < 10000; i++)
	{
		// std::unique_lock<std::mutex> lock(m_data_mutex1);
		// m_data_mutex2.lock();  // 顺序不一致，导致死锁
		// m_data_mutex1.lock();
		
		std::lock(m_data_mutex1, m_data_mutex2);
		std::lock_guard<std::mutex> lock1(m_data_mutex1,std::adopt_lock);
		std::lock_guard<std::mutex> lock2(m_data_mutex2, std::adopt_lock); // 
		// 
		if (!m_testList.empty())
		{
			int data = m_testList.front();
			std::cout << "线程ID=" << std::this_thread::get_id() << "收到消息：" << data << std::endl;
			m_testList.pop_front();
		}
		// m_data_mutex1.unlock();
		// m_data_mutex2.unlock();
	}

}

void CSharedData::SendMsgProcess()
{
	for (int i = 0; i < 10000; i++)
	{
		// m_data_mutex1.lock();  // 顺序不一致，导致死锁
		// m_data_mutex2.lock();
		std::lock(m_data_mutex2, m_data_mutex1);
		std::lock_guard<std::mutex> lock1(m_data_mutex1, std::adopt_lock);
		std::lock_guard<std::mutex> lock2(m_data_mutex2, std::adopt_lock);
		std::cout << "线程ID=" << std::this_thread::get_id() << "发送消息：" << i << std::endl;
		m_testList.push_back(i);
		// m_data_mutex2.unlock();
		// m_data_mutex1.unlock();
	}
}

```
std::lock 可以同时对多个互斥量进行加锁操作，std::lock_guard 使用的是C++ RAII(资源获取即初始化)风格，所以在构造时会对 mutex进行加锁操作
而std::adopt_lock参数，该结构体可以理解为标志为，加这个参数不会对互斥量进行加锁，但是在析构时还是会进行解锁。

## 2.6 unique_lock详解


## 2.7 单例设计模式共享数据分析、解决，call_once
## 2.8 condition_variable、wait、notify_one、notify_all
## 2.9 async、future、packaged_task、promise
## 2.10 future其他成员函数、shared_future、atomic
## 2.11 std::atomic续谈、std::async深入谈
## 2.12 windows临界区、其他各种mutex互斥量
## 2.13 补充知识、线程池浅谈、数量谈、总结
## 3.1 课程总结与展望