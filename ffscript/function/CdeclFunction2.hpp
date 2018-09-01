/******************************************************************
* File:        CdeclFunction2.hpp
* Description: template class and its member functions that used to
*              invoke _cdecl functions. this function object do not
*              store any arguments of its target function when
*              it is created. The caller need to pass arguments
*              through and array of void* to invoke the target
*              function.
*              This function object does not support pass argument
*              that its type is real type(float,double) for any type
*              that size of it is larger than current size of an
*              element in stack(of C++ program).
***            This template class is deprecated.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "DynamicFunction2.h"
#include "functioninternal2.hpp"
#include <cstring>

template <class Ret, class ...Args>
class CdeclFunction2;

template <class Ret, class ...Args>
class CdeclInvoker2 {
public:
	CdeclFunction2<Ret, Args...>* mOwner;

	CdeclInvoker2(CdeclFunction2<Ret, Args...>* pOwner) {
		mOwner = pOwner;
	}
	inline void call(void* pReturnVal, void* params[]);
};


//tempalte functions for return type is void
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, void);

//tempalte functions for return type is general type
template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2, Arg3, Arg4);

template <class Ret, class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2, Arg3);

template <class Ret, class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg1, Arg2);

template <class Ret, class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret, Arg);

template <class Ret>
DECLARE_CLASS_INVOKER_TEMPLATE2(CdeclInvoker2, CdeclFunction2, Ret);

template <class Ret, class ...Args>
class CdeclFunction2 :
	public DFunction2
{
public:
	typedef Ret(_cdecl *FuncType)(Args...);
	typedef CdeclFunction2<Ret, Args...> ClassType;
protected:
	static const int maxParam = sizeof...(Args);
#ifndef USE_EXTERNAL_PARAMS_ONLY
	void* mParams[maxParam + 1];
#endif
	CdeclInvoker2<Ret, Args...> invoker;
public:	
	CdeclFunction2(FuncType function) : invoker(this) {
		mFx = *((void**)&function);		
	}
	
	CdeclFunction2(void* function) : invoker(this) {
		mFx = function;
	}

	virtual ~CdeclFunction2() {
	}
#ifndef USE_EXTERNAL_PARAMS_ONLY
	template <class ...FixedParams>
	void bind(FixedParams... fixedParams) {
		void* params[] = { fixedParams... };
		size_t size = sizeof(params);
		mFixedParamCount = (int)(size/sizeof(params[0]));
		memcpy_s(&mParams, size, params, size);
	}

	void call(void* pReturnVal, void* params[]) override {
		if (mFixedParamCount == 0) {
			invoker.call(pReturnVal, params);
		}
		else if (mFixedParamCount == maxParam) {
			invoker.call(pReturnVal, mParams);
		}
		else {
			size_t size = (maxParam - mFixedParamCount) * sizeof(void*);
			memcpy_s(&mParams[mFixedParamCount], size, params, size);
			invoker.call(pReturnVal, mParams);
		}
	}
#else
	void call(void* pReturnVal, void* params[]) override {
		invoker.call(pReturnVal, params);	
	}
#endif

	DFunction2* clone() override {
		ClassType* newInstance = new ClassType(mFx);
#ifndef USE_EXTERNAL_PARAMS_ONLY
		newInstance->mFixedParamCount = mFixedParamCount;
		std::memcpy(newInstance->mParams, mParams, sizeof(mParams));
#endif
		return newInstance;
	}
	friend class CdeclInvoker2 < Ret, Args ... >;
};

/*current cdecl function support call function with maximum 8 parameters*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);	
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*);
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4, Arg5>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*);
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3, Arg4>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*);
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void CdeclInvoker2<void, Arg1, Arg2, Arg3>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*, void*);
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void CdeclInvoker2<void, Arg1, Arg2>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*, void*);
	((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Arg>
inline void CdeclInvoker2<void, Arg>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)(void*);
	((FunctionType)(mOwner->mFx))(params[0]);
}


inline void CdeclInvoker2<void>::call(void* pReturnVal, void* params[]) {
	typedef void(_cdecl *FunctionType)();
	((FunctionType)(mOwner->mFx))();
}


/* cdecl functions with return type general*/
template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4, Arg5>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3, Arg4>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Ret, class Arg1, class Arg2, class Arg3>
inline void CdeclInvoker2<Ret, Arg1, Arg2, Arg3>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Ret, class Arg1, class Arg2>
inline void CdeclInvoker2<Ret, Arg1, Arg2>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*, void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Ret, class Arg>
inline void CdeclInvoker2<Ret, Arg>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)(void*);
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))(params[0]);
}

template <class Ret>
inline void CdeclInvoker2<Ret>::call(void* pReturnVal, void* params[]) {
	typedef Ret(_cdecl *FunctionType)();
	*((Ret*)pReturnVal) = ((FunctionType)(mOwner->mFx))();
}