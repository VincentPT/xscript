/******************************************************************
* File:        MemberFunction2.hpp
* Description: template class and its member functions that used to
*              invoke member functions. This function object does
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

template <class Ret, class T, class ...Args>
class MFunction2;

template <class Ret, class T, class ...Args>
class MemberFuncInvoker2 {
public:
	MFunction2<Ret, T, Args...>* mOwner;

	MemberFuncInvoker2(MFunction2<Ret, T, Args...>* pOwner) {
		mOwner = pOwner;
	}
	inline void call(void* pReturnVal, void* params[]);
};

//tempalte functions for return type is void
template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2, Arg3, Arg4);

template <class ObjectT, class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2, Arg3);

template <class ObjectT, class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1, Arg2);

template <class ObjectT, class Arg1>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, ObjectT, Arg1);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, void, Arg);

//tempalte functions for return type is general type
template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2, Arg3, Arg4);

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2, Arg3);

template <class Ret, class ObjectT, class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1, Arg2);

template <class Ret, class ObjectT, class Arg1>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT, Arg1);

template <class Ret, class ObjectT>
DECLARE_CLASS_INVOKER_TEMPLATE2(MemberFuncInvoker2, MFunction2, Ret, ObjectT);

template <class Ret, class T, class ...Args>
class MFunction2 :
	public DFunction2
{
public:
	typedef Ret(T::*FuncType)(Args...);
	typedef MFunction2<Ret, T, Args...> ClassType;

protected:
	static const int maxParam = sizeof...(Args);
#ifndef USE_EXTERNAL_PARAMS_ONLY
	void* mParams[maxParam + 1];
#else
	T* _pObject;
#endif
	MemberFuncInvoker2<Ret, T, Args...> invoker;
public:

	MFunction2(T* pObject, FuncType function) : MFunction2(pObject, *((void**)&function) ){}
	
	MFunction2(T* pObject, void* function) : invoker(this) {
#ifndef USE_EXTERNAL_PARAMS_ONLY
		mParams[0] = pObject;
#else
		_pObject = pObject;
#endif
		mFx = function;
	}

	virtual ~MFunction2() {
	}
#ifndef USE_EXTERNAL_PARAMS_ONLY
	template <class ...FixedParams>
	void bind(FixedParams... fixedParams) {
		void* params[] = { fixedParams... };
		size_t size = sizeof(params);
		mFixedParamCount = size/sizeof(params[0]);
		memcpy_s(&mParams[1], size, params, size);
	}

	void call(void* pReturnVal, void* params[]) override {
		if (mFixedParamCount == 0) {
			invoker.call(pReturnVal, params);
		}
		else if(mFixedParamCount == maxParam) {
			invoker.call(pReturnVal, &mParams[1]);
		}
		else {
			size_t size = (maxParam - mFixedParamCount) * sizeof(void*);
			memcpy_s(&mParams[mFixedParamCount + 1], size, params, size);
			invoker.call(pReturnVal, &mParams[1]);
		}
	}
	virtual DFunction2* clone() override {
		ClassType* newInstance = new ClassType((T*)mParams[0], mFx);
		newInstance->mFixedParamCount = mFixedParamCount;
		std::memcpy(newInstance->mParams, mParams, sizeof(mParams));
		return newInstance;
	}
#else
	void call(void* pReturnVal, void* params[]) override {
		invoker.call(pReturnVal, params);		
	}
	virtual DFunction2* clone() override {
		ClassType* newInstance = new ClassType(_pObject, mFx);
		return newInstance;
	}
#endif
	static void* getMethodAddress(FuncType function) {
		return *((void**)(&function));
	}

	friend class MemberFuncInvoker2 < Ret, T, Args ... >;
};
#ifdef USE_EXTERNAL_PARAMS_ONLY
#define OWNER_OBJECT(funcObj) funcObj->_pObject
#else
#define OWNER_OBJECT(funcObj) (ObjectT*)(funcObj->mParams[0])
#endif

/*current std function support call function with maximum 8 parameters*/
template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1], params[2], params[3], params[4]);
}

template <class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2, Arg3, Arg4>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1], params[2], params[3]);
}

template <class ObjectT, class Arg1, class Arg2, class Arg3>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2, Arg3>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1], params[2]);
}

template <class ObjectT, class Arg1, class Arg2>
inline void MemberFuncInvoker2<void, ObjectT, Arg1, Arg2>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0], params[1]);
}

template <class ObjectT, class Arg1>
inline void MemberFuncInvoker2<void, ObjectT, Arg1>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)(void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)(params[0]);
}

template <class ObjectT>
inline void MemberFuncInvoker2<void, ObjectT>::call(void* pReturnVal, void* params[]) {
	typedef void(ObjectT::*MemberFunc)();
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	(obect->*func)();
}

/* std functions with return type general*/
template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2, Arg3, Arg4, Arg5>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1], params[2], params[3], params[4]);
}

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3, class Arg4>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2, Arg3, Arg4>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1], params[2], params[3]);
}

template <class Ret, class ObjectT, class Arg1, class Arg2, class Arg3>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2, Arg3>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1], params[2]);
}

template <class Ret, class ObjectT, class Arg1, class Arg2>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1, Arg2>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*, void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0], params[1]);
}

template <class Ret, class ObjectT, class Arg1>
inline void MemberFuncInvoker2<Ret, ObjectT, Arg1>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)(void*);
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)(params[0]);
}

template <class Ret, class ObjectT>
inline void MemberFuncInvoker2<Ret, ObjectT>::call(void* pReturnVal, void* params[]) {
	typedef Ret(ObjectT::*MemberFunc)();
	ObjectT* obect = OWNER_OBJECT(mOwner);
	MemberFunc func = *((MemberFunc*)&(mOwner->mFx));
	*((Ret*)pReturnVal) = (obect->*func)();
}

/* MemberFunction2 cannot dynamic invoke bellow functions type. So, use MemberFuncWrapper instead.
1. <return type> __cdecl T::function(..., double, ...)
2. <return type> __cdecl T::function(..., float, ...)
*/
template <class Ret, class T, class ...Args>
class MFunction2W : public MFunction2< Ret, MFunction2W<Ret, T, Args...>, const Args& ...> {
public:
	typedef Ret(T::*FuncType)(Args...);
private:
	FuncType _fx;
	T* _obj;
	inline Ret forward(const Args&... args) {
		return (_obj->*_fx) (args...);
	}
public:
	MFunction2W(T* obj, FuncType fx) :
		_fx(fx),
		_obj(obj),
		MFunction2< Ret, MFunction2W<Ret, T, Args...>, const Args& ...>(this, &MFunction2W::forward) {
	}

	virtual DFunction2* clone() override {
		MFunction2W* newInstance = new MFunction2W(_obj, _fx);
#ifndef  USE_EXTERNAL_PARAMS_ONLY
		newInstance->mFixedParamCount = mFixedParamCount;
		std::memcpy(newInstance->mParams, mParams, sizeof(mParams));
#endif // ! USE_EXTERNAL_PARAMS_ONLY
		return newInstance;
	}
};