/******************************************************************
* File:        FunctionDelegate2.hpp
* Description: template class and its member functions that used to
*              invoke functions. This function object does
*              not store any arguments of its target function when
*              it is created. The caller need to pass arguments
*              through and array of void* to invoke the target
*              function.
*              This function object does not support pass argument
*              that its type is real type(float,double) for any type
*              that size of it is larger than current size of an
*              element in stack(of C++ program).
**             This template class is deprecated.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "DynamicFunction2.h"
#include "functioninternal2.hpp"
#include <cstring>

#define DECLARE_CLASS_INVOKER_TEMPLATE2(className, Ret, ...) \
class className<Ret, ##__VA_ARGS__> { \
public: \
	typedef Ret (*Fx)(__VA_ARGS__);\
	Fx _fx;\
	className(void* fx) { \
		_fx = (Fx)fx; \
	} \
	inline void call(void* pReturnVal, void* params[]); \
}

template <class Ret, class ...Args>
class FunctionDelegate2;

template <class Ret, class ...Args>
class CdeclInvoker2 {
public:
	typedef Ret(*Fx)(Args...);
	Fx _fx;
	CdeclInvoker2(void* fx) {
		_fx = (Fx)fx;
	}
	inline void call(void* pReturnVal, void* params[]);
};


//tempalte functions for return type is void
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, void);

//tempalte functions for return type is general type
template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2, Arg3, Arg4);

template <class Ret, class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2, Arg3);

template <class Ret, class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg1, Arg2);

template <class Ret, class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret, Arg);

template <class Ret>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, Ret);

template <class Ret, class ...Args>
class FunctionDelegate2 :
	public DFunction2
{
public:
	typedef Ret(*FuncType)(Args...);
	typedef FunctionDelegate2<Ret, Args...> ClassType;
protected:
	static const int maxParam = sizeof...(Args);
	CdeclInvoker2<Ret, Args...> invoker;
public:	
	FunctionDelegate2(FuncType function) : invoker(function) {
	}
	
	FunctionDelegate2(void* function) : invoker(function) {
	}

	virtual ~FunctionDelegate2() {
	}
	void call(void* pReturnVal, void* params[]) override {
		invoker.call(pReturnVal, params);	
	}

	DFunction2* clone() override {
		ClassType* newInstance = new ClassType(invoker._fx);
		return newInstance;
	}
	friend class CdeclInvoker2 < Ret, Args ... >;
};

/*current cdecl function support call function with maximum 8 parameters*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);	
	((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*);
	((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*, void*, void*);
	((FunctionType)_fx)(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*, void*);
	((FunctionType)_fx)(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void CdeclInvoker2<void, Arg1, Arg2>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*, void*);
	((FunctionType)_fx)(params[0], params[1]);
}

template <class Arg>
inline void CdeclInvoker2<void, Arg>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)(void*);
	((FunctionType)_fx)(params[0]);
}


inline void CdeclInvoker2<void>::call(void* pReturnVal, void* params[]) {
	typedef void(*FunctionType)();
	((FunctionType)_fx)();
}


/* cdecl functions with return type general*/
template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1], params[2], params[3], params[4]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1], params[2], params[3]);
}

template <class Ret, class Arg1, class Arg2, class Arg3>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1], params[2]);
}

template <class Ret, class Arg1, class Arg2>
inline void CdeclInvoker2<Ret, Arg1, Arg2>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0], params[1]);
}

template <class Ret, class Arg>
inline void CdeclInvoker2<Ret, Arg>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)(void*);
	*((Ret*)pReturnVal) = ((FunctionType)_fx)(params[0]);
}

template <class Ret>
inline void CdeclInvoker2<Ret>::call(void* pReturnVal, void* params[]) {
	typedef Ret(*FunctionType)();
	*((Ret*)pReturnVal) = ((FunctionType)_fx)();
}