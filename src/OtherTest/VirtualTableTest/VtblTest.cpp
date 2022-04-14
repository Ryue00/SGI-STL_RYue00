#include <iostream>
#include <vector>
#include<iostream>
#include<windows.h>
#include<fstream>
#include <map>
#include<time.h>
#include <memory>
#include <stdlib.h>
using namespace std;
typedef  void (*Fun) (void);

class Father
{
public:

protected:

private:
	virtual void printFatherA() { cout << "printFatherA" << endl; }
	virtual void printFatherB() { cout << "printFatherB" << endl; }

	void GetChildVirtualFunction()
	{
		// 父类访问 子类特有的 虚函数
		reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 0)) + 2))();
	}
};
class Mother
{
protected:
	virtual void printMotherB() { cout << "printMotherB" << endl; }

private:
	virtual void printMotherA() { cout << "printMotherA" << endl; }

public:
	virtual void printMotherC() { cout << "printMotherC" << endl; }
};

class Children
	: public Father
	, public Mother
{

public:

	void GetFatherPrivate()
	{
		// father  子类访问基类 non-public 函数
		reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 0)) + 0))();
		reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 0)) + 1))();
		// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 0)) + 2))();
	}

	void GetMotherPrivate()
	{
		// mother 子类访问基类 non-public 函数
		reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 1)) + 0))();
		reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 1)) + 1))();
		reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(this) + 1)) + 2))();
	}

	virtual  void FatherCallChildVirtualFunction() { cout << "FatherCallChildVirtualFunction" << endl; }
private:

	int b;
};

//1. (int*)(&this)取vptr地址，该地址存储的是指向vtbl的指针
//2. (int*)*(int*)(&this)取vtbl地址，该地址存储的是虚函数表数组
//3. (Fun) * ((int*)*(int*)(&this) + 0)，取vtbl数组的第一个元素，即Father中第一个虚函数f的地址
//4. (Fun) * ((int*)*(int*)(&this) + 1)，取vtbl数组的第二个元素
int main()
{
	Children children;

	children.GetFatherPrivate();
	children.GetMotherPrivate();
	// (children.GetFatherPrivate())();

	// father  子类访问基类 non-public 函数
	// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(&children) + 0)) + 0))();
	// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(&children) + 0)) + 1))();
	// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(&children) + 0)) + 2))();
	// mother 子类访问基类 non-public 函数
	// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(&children) + 1)) + 0))();
	// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(&children) + 1)) + 1))();
	// reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(&children) + 1)) + 2))();

	Father* father = new Children();
	// 父类访问 子类特有的 虚函数
	 // father->FatherCallChildVirtualFunction(); // 编译出错
	reinterpret_cast<Fun>(*(reinterpret_cast<int*>(*(reinterpret_cast<int*>(father) + 0)) + 2))();


    system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
