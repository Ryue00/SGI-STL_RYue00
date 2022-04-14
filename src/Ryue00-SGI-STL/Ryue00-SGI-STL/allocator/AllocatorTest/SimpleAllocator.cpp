#include "SimpleAllocator.h"
#include <vector>
#include <iostream>

using namespace std;

// myAllocator test 
int main()
{
	int ia[] = { 0, 1, 2, 3, 4 };
	std::vector<int, RYue00::CMyAllocator<int>> iVector(ia, ia + 5);
	for (auto i : iVector)
	{
		std::cout << i << std::endl;
	}
	getchar();
}