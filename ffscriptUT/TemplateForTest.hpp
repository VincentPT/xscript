/******************************************************************
* File:        TemplateForTest.hpp
* Description: declare common functions used in unit test project.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once

template <class T, class Ret, class ...args>
inline void* getFunctionPointer(Ret(T::*memberFunction) (args...)) {

	const int n = sizeof...(args);
	return ((void*)*((void**)&memberFunction));
}

template <class T, class Ret = void, class ...args>
inline void* getFunctionPointer(void(T::*memberFunction) (args...)) {

	const int n = sizeof...(args);
	return ((void*)*((void**)&memberFunction));
}

template <class T, class Ret = double, class ...args>
inline void* getFunctionPointer(double(T::*memberFunction) (args...)) {

	const int n = sizeof...(args);
	return ((void*)*((void**)&memberFunction));
}

template <class T, class Ret = float, class ...args>
inline void* getFunctionPointer(float(T::*memberFunction) (args...)) {

	const int n = sizeof...(args);
	return ((void*)*((void**)&memberFunction));
}

template <class T, class ...Args>
T sum(Args... args) {
	T a[] = { args ... };
	const int n = sizeof...(Args);
	T s = 0;
	for (int i = 0; i < n; i++) {
		s += a[i];
	}

	return s;
}

template <class T>
T sum() {
	return 0;
}


/*classA for general*/
template <class T, class ...Args>
class classA {
public:
	T f(T val, Args... args) { return val * sum<T, Args...>(args...); }
};

/*classA for specialized int*/
template <class ...Args>
class classA<int, Args...> {
public:
	int f(int val, Args... args) { return val + sum<int, Args...>(args...); }
};

//#define PRINT_TEST_MESSAGE Logger::WriteMessage
#define PRINT_TEST_MESSAGE(x)
