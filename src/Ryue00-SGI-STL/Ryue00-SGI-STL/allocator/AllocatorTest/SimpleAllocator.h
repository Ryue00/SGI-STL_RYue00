#pragma once
/**
 * Copyright (c) 2020, Intretech Inc. All Rights Reserved.
 *
 * @Description: 简单的 空间配置器实现
 * @author: RYue00
 */
#include <cstddef>
#include <iostream>
#include <vector>
 // ReSharper disable once CppInconsistentNaming
namespace RYue00
{
	// 分配内存 申请size个 T 元素的大小内存空间
	template <class T>
	// ptrdiff_t类型变量通常用来保存两个指针减法操作的结果
	inline  T* _allocate(ptrdiff_t size, T*) 
	{
		// 若是new失败将会默认执行抛出bad_alloc异常
		std::set_new_handler(0);
		// std::set_new_handler([] {std::cout << "bad_alloc" << std::endl; });
		// 系统默认的全局::operator new(size_t size)也只是调用malloc分配内存，并且返回一个void* 指针
		// new: 1. 调用operator new分配内存；
		//      2. 调用构造函数生成类对象；
		//		3. 返回相应指针。
		// new比::operator new 多了步骤2
		T* tmp = const_cast<T*>(::operator new(size * sizeof(T)));
		if (nullptr == tmp)
		{
			// 为空 认为申请空间失败,退出
			std::cerr << "out of memory" << std::endl;
			exit(-1);
		}
		return tmp;
	}

	// 释放内存 
	template <class T>
	inline void _deallocate(T* buffer)
	{
		// 和::operator new 类似默认全局，可重载
		::operator delete(buffer);
	}

	// 构造对象
	template <class T1, class T2>
	inline void _construct(T1* p, const T2& value)
	{
		::new(p) T1(value);
	}

	// 析构对象
	template <class T>
	inline void _destroy(T* ptr)
	{
		ptr->~T();
	}

	// 提供外部使用 allocator
	template <class T>
	class CMyAllocator
	{
	public:
		// 对象的类型
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type; // ptrdiff_t 是两个指针相减结果的有符号整数类型

		template <class U>
		struct rebind
		{
			typedef CMyAllocator<U> other;
		};

		/**
		 * 对外接口 完成内存分配、内存释放、对象构造、对象析构
		 */
		pointer allocate(size_type n, const void* hin = 0)
		{
			return _allocate((difference_type)(n), pointer(0));
		}

		void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}

		void construct(pointer p, const T& value)
		{
			_construct(p, value);
		}

		void destroy(pointer p)
		{
			_destroy(p);
		}

		// 返回某个对象地址
		pointer address(reference x)
		{
			return pointer(&x);
		}

		// 返回某个 const 对象的地址
		const_pointer const_address(const_reference x) {
			return static_cast<const_pointer>(&x);
		}

		// 返回可成功分配的最大量  
		size_type max_size() const {
			return (size_type)(UINT_MAX / sizeof(T)); // UINT_MAX 是 unsigned long 及 unsigned long long 的最大值
		}

	};
}

