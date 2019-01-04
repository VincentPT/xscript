#pragma once
#include "DynamicFunction2.h"
#include "functioninternal.hpp"
#include <cstring>

template <class Ret, class ...Args>
class StdFunction2;

template <class Ret, class ...Args>
class STDInvoker {
public:
	StdFunction2<Ret, Args...>* mOwner;

	STDInvoker(StdFunction2<Ret, Args...>* pOwner) {
		mOwner = pOwner;
	}
	inline void call();
};


//tempalte functions for return type is void
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, void);

//tempalte functions for return type is float
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, float);

//tempalte functions for return type is double
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, double);

//tempalte functions for return type is __int64
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, __int64);

//tempalte functions for return type is unsigned __int64
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2, Arg3, Arg4);

template <class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg1, Arg2);

template <class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64, Arg);

template <>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, unsigned __int64);


//tempalte functions for return type is general type
template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2, Arg3, Arg4, Arg5);

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2, Arg3, Arg4);

template <class Ret, class Arg1, class Arg2, class Arg3>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2, Arg3);

template <class Ret, class Arg1, class Arg2>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg1, Arg2);

template <class Ret, class Arg>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret, Arg);

template <class Ret>
DECLARE_CLASS_INVOKER_TEMPLATE(STDInvoker, StdFunction2, Ret);

template <class Ret, class ...Args>
class StdFunction2 :
	public DFunction2
{
public:
	typedef Ret(__stdcall *STDFunc)(Args...);
	typedef StdFunction2<Ret, Args...> ClassType;
protected:
	static const int maxParam = sizeof...(Args);
#ifndef USE_EXTERNAL_PARAMS_ONLY
	void* mParams[maxParam + 1];
#endif
	STDInvoker<Ret, Args...> invoker;
public:
	StdFunction2(STDFunc function) : invoker(this) {
		mFx = *((void**)&function);
	}

	StdFunction2(void* function) : invoker(this) {
		mFx = function;
	}

	virtual ~StdFunction2() {
	}

	Ret operator()(Args...args) {
		return ((STDFunc)mFx)(args...);
	}

	inline void call() override {
		invoker.call();
	}

#ifndef USE_EXTERNAL_PARAMS_ONLY
	bool pushParam(void* param) override {
		if (maxParam <= mFixedParamCount) {
			return false;
		}
		mParams[mFixedParamCount++] = param;
		return true;
	}

	void* popParam() override {
		if (mFixedParamCount == 0) return nullptr;
		return mParams[mFixedParamCount--];
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

	friend class STDInvoker < Ret, Args ... > ;
};

/*current std function support call function with maximum 8 parameters*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void STDInvoker<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void STDInvoker<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void STDInvoker<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void STDInvoker<void, Arg1, Arg2, Arg3, Arg4, Arg5>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void STDInvoker<void, Arg1, Arg2, Arg3, Arg4>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void STDInvoker<void, Arg1, Arg2, Arg3>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void STDInvoker<void, Arg1, Arg2>::call() {
	typedef void(__stdcall *FunctionType)(void*, void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Arg>
inline void STDInvoker<void, Arg>::call() {
	typedef void(__stdcall *FunctionType)(void*);
	void** params = &mOwner->mParams[0];
	((FunctionType)(mOwner->mFx))(params[0]);
}


inline void STDInvoker<void>::call() {
	typedef void(__stdcall *FunctionType)();
	((FunctionType)(mOwner->mFx))();
}

/* std functions with return type is float*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void STDInvoker<float, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void STDInvoker<float, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void STDInvoker<float, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void STDInvoker<float, Arg1, Arg2, Arg3, Arg4, Arg5>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void STDInvoker<float, Arg1, Arg2, Arg3, Arg4>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void STDInvoker<float, Arg1, Arg2, Arg3>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void STDInvoker<float, Arg1, Arg2>::call() {
	typedef float(__stdcall *FunctionType)(void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Arg>
inline void STDInvoker<float, Arg>::call() {
	typedef float(__stdcall *FunctionType)(void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))(params[0]);
}


inline void STDInvoker<float>::call() {
	typedef float(__stdcall *FunctionType)();
	mOwner->mReturnVal.fVal = ((FunctionType)(mOwner->mFx))();
}

/* std functions with return type is double*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void STDInvoker<double, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void STDInvoker<double, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void STDInvoker<double, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void STDInvoker<double, Arg1, Arg2, Arg3, Arg4, Arg5>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void STDInvoker<double, Arg1, Arg2, Arg3, Arg4>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void STDInvoker<double, Arg1, Arg2, Arg3>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void STDInvoker<double, Arg1, Arg2>::call() {
	typedef double(__stdcall *FunctionType)(void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Arg>
inline void STDInvoker<double, Arg>::call() {
	typedef double(__stdcall *FunctionType)(void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))(params[0]);
}


inline void STDInvoker<double>::call() {
	typedef double(__stdcall *FunctionType)();
	mOwner->mReturnVal.dVal = ((FunctionType)(mOwner->mFx))();
}

/* std functions with return type is __int64*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void STDInvoker<__int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void STDInvoker<__int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void STDInvoker<__int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void STDInvoker<__int64, Arg1, Arg2, Arg3, Arg4, Arg5>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void STDInvoker<__int64, Arg1, Arg2, Arg3, Arg4>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void STDInvoker<__int64, Arg1, Arg2, Arg3>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void STDInvoker<__int64, Arg1, Arg2>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Arg>
inline void STDInvoker<__int64, Arg>::call() {
	typedef __int64(__stdcall *FunctionType)(void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0]);
}


inline void STDInvoker<__int64>::call() {
	typedef __int64(__stdcall *FunctionType)();
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))();
}

/* std functions with return type is unsigned __int64*/
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void STDInvoker<unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void STDInvoker<unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void STDInvoker<unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void STDInvoker<unsigned __int64, Arg1, Arg2, Arg3, Arg4, Arg5>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Arg1, class Arg2, class Arg3, class Arg4>
inline void STDInvoker<unsigned __int64, Arg1, Arg2, Arg3, Arg4>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Arg1, class Arg2, class Arg3>
inline void STDInvoker<unsigned __int64, Arg1, Arg2, Arg3>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Arg1, class Arg2>
inline void STDInvoker<unsigned __int64, Arg1, Arg2>::call() {
	typedef __int64(__stdcall *FunctionType)(void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Arg>
inline void STDInvoker<unsigned __int64, Arg>::call() {
	typedef __int64(__stdcall *FunctionType)(void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))(params[0]);
}


inline void STDInvoker<unsigned __int64>::call() {
	typedef __int64(__stdcall *FunctionType)();
	mOwner->mReturnVal.i64Val = ((FunctionType)(mOwner->mFx))();
}


/* std functions with return type general*/
template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline void STDInvoker<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline void STDInvoker<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline void STDInvoker<Ret, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4], params[5]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline void STDInvoker<Ret, Arg1, Arg2, Arg3, Arg4, Arg5>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3], params[4]);
}

template <class Ret, class Arg1, class Arg2, class Arg3, class Arg4>
inline void STDInvoker<Ret, Arg1, Arg2, Arg3, Arg4>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2], params[3]);
}

template <class Ret, class Arg1, class Arg2, class Arg3>
inline void STDInvoker<Ret, Arg1, Arg2, Arg3>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1], params[2]);
}

template <class Ret, class Arg1, class Arg2>
inline void STDInvoker<Ret, Arg1, Arg2>::call() {
	typedef int(__stdcall *FunctionType)(void*, void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0], params[1]);
}

template <class Ret, class Arg>
inline void STDInvoker<Ret, Arg>::call() {
	typedef int(__stdcall *FunctionType)(void*);
	void** params = &mOwner->mParams[0];
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))(params[0]);
}

template <class Ret>
inline void STDInvoker<Ret>::call() {
	typedef int(__stdcall *FunctionType)();
	mOwner->mReturnVal.iVal = ((FunctionType)(mOwner->mFx))();
}