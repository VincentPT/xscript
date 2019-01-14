/******************************************************************
* File:        functioninternal.hpp
* Description: define some common template classes, functions and
*              macros used by derived classes of DFunction.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once

#include <cstddef>

#define DECLARE_CLASS_INVOKER_TEMPLATE(className, classOwner, ...) \
class className<__VA_ARGS__> { \
public: \
	classOwner<__VA_ARGS__>* mOwner; \
	className(classOwner<__VA_ARGS__>* pOwner) { \
		mOwner = pOwner; \
	} \
	inline void call(); \
}

///*to use this function all param must cast to void* before call */

template <class ...Args>
inline void pushParams(DFunction* pFunction, Args... args) {
	const int n = sizeof...(Args);
	void* refargs[n] = { (void*)(size_t)args...};
	void** pstart = refargs;
	void** pend = pstart + n;
	while(pstart < pend) {
		pFunction->pushParam(*pstart++);
	}
}

template <typename Fx1, typename Fx2>
struct FunctionConverter
{
	union {
		Fx1 fx1;
		Fx2 fx2;
	} conv;
	FunctionConverter(Fx1 fx1) {
		conv.fx1 = fx1;
	}
	static Fx2 convert(Fx1 fx1) {
		FunctionConverter converter(fx1);
		return converter.conv.fx2;
	}
};

template <class...Args>
class FunctionInvokerBase {
protected:
	static const int _maxParam = sizeof...(Args);
	int _paramCount;
	void* _args[_maxParam + 1];
protected:
	FunctionInvokerBase() : _paramCount(0) {}

public:
	inline bool pushParam(void* param) {
		if (_maxParam <= _paramCount) {
			return false;
		}
		_args[_paramCount++] = param;
		return true;
	}
	inline void* popParam() {
		if (_paramCount == 0) return nullptr;
		return _args[--_paramCount];
	}
};