#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <initializer_list>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <typeinfo>

#include <new>


class array_exception : public std::exception {
public:
	array_exception(std::string m) : msg(m){}
	~array_exception(){}
	//const char* what() {return msg.c_str();}

private:
	std::string msg;
};



#ifdef NDEBUG
#define DEBUG_INFO(message, func)
#define DEBUG_PRINT(message)
#else 
#define DEBUG_INFO(message, func) std::cout<<"LINE : "<<__LINE__<<std::endl<<message<<std::endl; func
#define DEBUG_PRINT(message) std::cout<<message<<std::endl<<std::endl<<std::endl;
#endif

#ifdef NDEBUG
#define ASSERT(cond, exp) if (!(cond)) {\
                          std::cout<<"ASSERTION FAILED"<<std::endl; \
                          std::cout<<"FILE : "<<__FILE__<<std::endl;\
                          std::cout<<"LINE : "<<__LINE__<<std::endl;\
                          std::cout<<"MESSAGE : "<<(exp)<<std::endl;\
                          abort();}
 #else
 #define ASSERT(cond, exp)
 #endif

 #ifdef NDEBUG
 #define SOFT_ASSERT(cond, exp)
 #else
 #define SOFT_ASSERT(cond, exp)
 #endif                         



/*void* operator new[] (std::size_t size, int val) {
	void* mem = operator new[] (size);
	memset(mem, 0, size);
	return mem;
}*/

void* operator new[] (std::size_t size, int val) {
	void* mem = operator new[] (size);
	memset(mem, val, size);
	return mem;
}

//void operator delete[] (void* ptr) noexcept{
	
//}

//void operator delete[] (void* ptr, size_t) noexcept{
//	free(ptr);
//}

#endif //ARRAY_UTILS_H