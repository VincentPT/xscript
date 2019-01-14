/******************************************************************
* File:        FunctionDelegate.hpp
* Description: template class and its member functions that used to
*              invoke functions. This function object stores
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

template <class Ret, class ...Args>
class FunctionInvoker;

//////////////////////////////////////////////////////////////////////////////////////////////////
#define DECLARE_FUNCTION_INVOKER_T(Ret, ...)\
	class FunctionInvoker<Ret, ##__VA_ARGS__> : public FunctionInvokerBase<__VA_ARGS__> {\
	protected:\
		Ret _retStorage;\
	public:\
		typedef Ret(*Fx)(__VA_ARGS__);\
		Fx _fx;\
		FunctionInvoker(Fx fx, void** data) : _fx(fx) {\
			*data = &_retStorage;\
		}\
		void call();\
	}

//////////////////////////////////////////////////////////////////////////////////////////////////
#define DECLARE_FUNCTION_INVOKER_VOID(...)\
	class FunctionInvoker<void, ##__VA_ARGS__> : public FunctionInvokerBase<__VA_ARGS__> {\
	public:\
		typedef void(*Fx)(__VA_ARGS__);\
		Fx _fx;\
		FunctionInvoker(Fx fx, void** data) : _fx(fx) {\
			*data = nullptr;\
		}\
		void call();\
	}

//template <>
//DECLARE_FUNCTION_INVOKER_VOID();

template <class T1>
DECLARE_FUNCTION_INVOKER_VOID(T1);

template <class T1, class T2>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2);

template <class T1, class T2, class T3>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2, T3);

template <class T1, class T2, class T3, class T4>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2, T3, T4);

template <class T1, class T2, class T3, class T4, class T5>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2, T3, T4, T5);

template <class T1, class T2, class T3, class T4, class T5, class T6>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2, T3, T4, T5, T6);

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2, T3, T4, T5, T6, T7);

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
DECLARE_FUNCTION_INVOKER_VOID(T1, T2, T3, T4, T5, T6, T7, T8);

template <class Ret>
DECLARE_FUNCTION_INVOKER_T(Ret);

template <class Ret, class T1>
DECLARE_FUNCTION_INVOKER_T(Ret, T1);

template <class Ret, class T1, class T2>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2);

template <class Ret, class T1, class T2, class T3>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2, T3);

template <class Ret, class T1, class T2, class T3, class T4>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2, T3, T4);

template <class Ret, class T1, class T2, class T3, class T4, class T5>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2, T3, T4, T5);

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2, T3, T4, T5, T6);

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2, T3, T4, T5, T6, T7);

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
DECLARE_FUNCTION_INVOKER_T(Ret, T1, T2, T3, T4, T5, T6, T7, T8);

template <>
class FunctionInvoker<void> : public FunctionInvokerBase<> {
public:
	typedef void(*Fx)();
	Fx _fx;
	FunctionInvoker(Fx fx, void** data) : _fx(fx) {
		*data = nullptr;
	}
	void call() {
		typedef void(*RFx)();
		((RFx)_fx)();
	}
};

template <class T1>
void FunctionInvoker<void, T1>::call() {
	typedef void(*RFx)(void*);
	((RFx)_fx)(this->_args[0]);
}

template <class T1, class T2>
void FunctionInvoker<void, T1, T2>::call() {
	typedef void(*RFx)(void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1]);
}

template <class T1, class T2, class T3>
void FunctionInvoker<void, T1, T2, T3>::call() {
	typedef void(*RFx)(void*, void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2]);
}

template <class T1, class T2, class T3, class T4>
void FunctionInvoker<void, T1, T2, T3, T4>::call() {
	typedef void(*RFx)(void*, void*, void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3]);
}

template <class T1, class T2, class T3, class T4, class T5>
void FunctionInvoker<void, T1, T2, T3, T4, T5>::call() {
	typedef void(*RFx)(void*, void*, void*, void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4]);
}


template <class T1, class T2, class T3, class T4, class T5, class T6>
void FunctionInvoker<void, T1, T2, T3, T4, T5, T6>::call() {
	typedef void(*RFx)(void*, void*, void*, void*, void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5]);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
void FunctionInvoker<void, T1, T2, T3, T4, T5, T6, T7>::call() {
	typedef void(*RFx)(void*, void*, void*, void*, void*, void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6]);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
void FunctionInvoker<void, T1, T2, T3, T4, T5, T6, T7, T8>::call() {
	typedef void(*RFx)(void*, void*, void*, void*, void*, void*, void*, void*);
	((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6], this->_args[7]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class Ret>
void FunctionInvoker<Ret>::call() {
	typedef Ret(*RFx)();
	_retStorage = ((RFx)_fx)();
}

template <class Ret, class T1>
void FunctionInvoker<Ret, T1>::call() {
	typedef Ret(*RFx)(void*);
	_retStorage = ((RFx)_fx)(this->_args[0]);
}

template <class Ret, class T1, class T2>
void FunctionInvoker<Ret, T1, T2>::call() {
	typedef Ret(*RFx)(void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1]);
}

template <class Ret, class T1, class T2, class T3>
void FunctionInvoker<Ret, T1, T2, T3>::call() {
	typedef Ret(*RFx)(void*, void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2]);
}

template <class Ret, class T1, class T2, class T3, class T4>
void FunctionInvoker<Ret, T1, T2, T3, T4>::call() {
	typedef Ret(*RFx)(void*, void*, void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3]);
}

template <class Ret, class T1, class T2, class T3, class T4, class T5>
void FunctionInvoker<Ret, T1, T2, T3, T4, T5>::call() {
	typedef Ret(*RFx)(void*, void*, void*, void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4]);
}


template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
void FunctionInvoker<Ret, T1, T2, T3, T4, T5, T6>::call() {
	typedef Ret(*RFx)(void*, void*, void*, void*, void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5]);
}

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
void FunctionInvoker<Ret, T1, T2, T3, T4, T5, T6, T7>::call() {
	typedef Ret(*RFx)(void*, void*, void*, void*, void*, void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6]);
}

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
void FunctionInvoker<Ret, T1, T2, T3, T4, T5, T6, T7, T8>::call() {
	typedef Ret(*RFx)(void*, void*, void*, void*, void*, void*, void*, void*);
	_retStorage = ((RFx)_fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6], this->_args[7]);
}


template <class Ret, class ...Args>
class FunctionDelegate :
	public DFunction
{
public:
	typedef Ret(*FuncType)(Args...);
protected:
	FunctionInvoker<Ret, Args...> invoker;
public:
	FunctionDelegate(FuncType function) : invoker(function, &_ret) {}
	virtual ~FunctionDelegate() {}

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
		FunctionDelegate* newInstance = new FunctionDelegate(invoker._fx);
		newInstance->invoker = invoker;
		return newInstance;
	}
};

template <class Ret, class ...Args>
inline Ret funcForward(Ret function(Args...),const Args&... args) {
	return function(args...);
}