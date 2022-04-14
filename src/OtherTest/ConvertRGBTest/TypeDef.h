#pragma once

#include <vector>
#include <deque>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <string>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


// 带业务性质的定义
typedef long long OBJID;
typedef	char NAMESTR[32];
typedef	char OBJECT_INFO[512];
// 此处定义常规的一条SQL语句，最长1KB，需要注意不要超过
typedef	char SQLBUF[16384];
// SQL的条件
typedef	char SQL_CONDITION[256];


//////////////////////////////////////////////////////////////////////////
// 纯类型定义
typedef char int8;
typedef unsigned char uint8;
typedef unsigned char UCHAR;
typedef unsigned char UINT8;


typedef short int16;
typedef unsigned short uint16;
typedef unsigned short USHORT;

typedef int int32;
typedef unsigned int uint32;
typedef unsigned int UINT;

#ifdef TARGET_WINDOWS
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

typedef unsigned long ULONG;
typedef long long INT64;
typedef unsigned long long UINT64;


// 定义容器
typedef std::vector<uint8>				        VEC_UINT8;
typedef std::vector<uint16>				        VEC_UINT16;
typedef std::vector<unsigned char>				VEC_UCHAR;
typedef std::vector<unsigned short>				VEC_USHORT;
typedef std::vector<int>						VEC_INT;
typedef std::vector<unsigned int>				VEC_UINT;
typedef std::vector<long long int>				VEC_INT64;
typedef std::vector<std::string>				VEC_STR;
typedef std::map<int, int>						MAP_INTINT;
typedef std::map<int, std::string>				MAP_INTSTR;
typedef std::map<std::string, int>				MAP_STRINT;
typedef std::map<std::string, unsigned int>		MAP_STRUINT;
typedef std::map<std::string, std::string>		MAP_STRSTR;
typedef std::map<std::string, std::string>::value_type   MAP_STRSTR_VALUE_TYPE;
typedef std::list<std::string>					LIST_STR;
typedef std::list<int>							LIST_INT;
