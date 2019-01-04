/******************************************************************
* File:        StdFunction.hpp
* Description: template class and its member functions that used to
*              invoke _stdcall functions. This function object stores
*              all necessary arguments of its target function when
*              it is created. The caller just to call 'call' only
*              to invoke the target function.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "DynamicFunction.h"
#include "functioninternal.hpp"
#include <cstring>

template <class ...Args> class StdInvoker;

template <class Ret> class StdInvoker<Ret> {
	Ret _retStorage;
public:
	void* _fx;
	StdInvoker(void* fx, void** data) : _fx(fx) { *data = &_retStorage; }
	inline void call() {
		typedef Ret(_stdcall *FunctionType)();
		_retStorage = ((FunctionType)_fx)();
	}
	inline bool pushParam(void*) { return false; }
	inline void* popParam() { return nullptr; }
};

template <class Ret, class T1>
BEGIN_INVOKER1(StdInvoker, Ret, T1)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*);
	_retStorage = ((FunctionType)_fx)(_params[0]);
}
END_INVOKER1;


template <class Ret, class T1, class T2>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2, T3)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2, T3, T4)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2, T3, T4, T5)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER1(StdInvoker, Ret, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef Ret(_stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7]);
}
END_INVOKER1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <> class StdInvoker<void> {
public:
	void* _fx;
	StdInvoker(void* fx, void** data) : _fx(fx) { *data = nullptr; }
	inline void call() {
		typedef void(_stdcall *FunctionType)();
		((FunctionType)_fx)();
	}
	inline bool pushParam(void*) { return false; }
	inline void* popParam() { return nullptr; }
};

template <class T1>
BEGIN_INVOKER2(StdInvoker, T1)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*);
	((FunctionType)_fx)(_params[0]);
}
END_INVOKER2;


template <class T1, class T2>
BEGIN_INVOKER2(StdInvoker, T1, T2)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*);
	((FunctionType)_fx)(_params[0], _params[1]);
}
END_INVOKER2;

template <class T1, class T2, class T3>
BEGIN_INVOKER2(StdInvoker, T1, T2, T3)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2]);
}
END_INVOKER2;

template <class T1, class T2, class T3, class T4>
BEGIN_INVOKER2(StdInvoker, T1, T2, T3, T4)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3]);
}
END_INVOKER2;

template <class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER2(StdInvoker, T1, T2, T3, T4, T5)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4]);
}
END_INVOKER2;

template <class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER2(StdInvoker, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5]);
}
END_INVOKER2;

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER2(StdInvoker, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6]);
}
END_INVOKER2;

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER2(StdInvoker, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef void(_stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7]);
}
END_INVOKER2;

template <class Ret, class ...Args>
class StdFunction :
	public DFunction
{
public:
	typedef Ret(_stdcall *FuncType)(Args...);
protected:
	StdInvoker<Ret, Args...> invoker;
public:
	typedef FuncType STDFunc;
	StdFunction(FuncType function) : StdFunction(*((void**)&function)) {}
	StdFunction(void* function) : invoker(function, &_ret) {}
	virtual ~StdFunction() {}

	virtual Ret operator()(Args...args) {
		return ((FuncType)invoker._fx)(args...);
	}

	inline void call() override {
		invoker.call();
	}

	bool pushParam(void* param) override {
		return invoker.pushParam(param);
	}

	void* popParam() override {
		return invoker.popParam();
	}

	DFunction* clone() override {
		StdFunction* newInstance = new StdFunction((void*)nullptr);
		newInstance->invoker = invoker;
		return newInstance;
	}
};