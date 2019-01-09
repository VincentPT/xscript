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
//////////////////////////////////////////////////////////////////////////////////////////////////
template <class Ret, class ...Args> class InvokerInternal1 {
protected:
	static const int _maxParam = sizeof...(Args);
	int _paramCount;
	Ret _retStorage;
public:
	void* _params[_maxParam];
	void* _fx;
	InvokerInternal1(void* fx, void** data) : _fx(fx), _paramCount(0)
	{
		*data = &_retStorage;
	}

	inline bool pushParam(void* param) {
		if (_maxParam <= _paramCount) {
			return false;
		}
		_params[_paramCount++] = param;
		return true;
	}
	inline void* popParam() {
		if (_paramCount == 0) return nullptr;
		return _params[--_paramCount];
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////
template <class ...Args> class InvokerInternal2 {
protected:
	static const int _maxParam = sizeof...(Args);
	int _paramCount;
public:
	void* _params[_maxParam];
	void* _fx;
	InvokerInternal2(void* fx, void** data) : _fx(fx), _paramCount(0)
	{
		*data = nullptr;
	}

	inline bool pushParam(void* param) {
		if (_maxParam <= _paramCount) {
			return false;
		}
		_params[_paramCount++] = param;
		return true;
	}
	inline void* popParam() {
		if (_paramCount == 0) return nullptr;
		return _params[--_paramCount];
	}
};


#define BEGIN_INVOKER1(className, ...) \
class className<__VA_ARGS__> : public InvokerInternal1<__VA_ARGS__> { \
public: \
	className(void* fx, void** data) : InvokerInternal1<__VA_ARGS__>(fx, data) {}

#define END_INVOKER1 }

#define BEGIN_INVOKER2(className, ...) \
class className<void, ##__VA_ARGS__> : public InvokerInternal2<__VA_ARGS__> { \
public: \
	className(void* fx, void** data) : InvokerInternal2<__VA_ARGS__>(fx, data) {}

#define END_INVOKER2 }