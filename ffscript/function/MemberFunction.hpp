#pragma once
#include "DynamicFunction.h"
#include "functioninternal.hpp"
#include <cstring>

template <class ...Args> class ThisInvoker;

template <class Ret, class T>
BEGIN_INVOKER1(ThisInvoker, Ret, T)
inline void call() {
	typedef Ret(T::*MemberFunc)();
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))();
}
END_INVOKER1;

template <class Ret, class T, class T1>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1]);
}
END_INVOKER1;


template <class Ret, class T, class T1, class T2>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2]);
}
END_INVOKER1;

template <class Ret, class T, class T1, class T2, class T3>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2, T3)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3]);
}
END_INVOKER1;

template <class Ret, class T, class T1, class T2, class T3, class T4>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2, T3, T4)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4]);
}
END_INVOKER1;

template <class Ret, class T, class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2, T3, T4, T5)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5]);
}
END_INVOKER1;

template <class Ret, class T, class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5], _params[6]);
}
END_INVOKER1;

template <class Ret, class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7]);
}
END_INVOKER1;

template <class Ret, class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER1(ThisInvoker, Ret, T, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef Ret(T::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	_retStorage = (obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7], _params[8]);
}
END_INVOKER1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class T>
BEGIN_INVOKER2(ThisInvoker, T)
inline void call() {
	typedef void(T::*MemberFunc)();
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))();
}
END_INVOKER2;


template <class T, class T1>
BEGIN_INVOKER2(ThisInvoker, T, T1)
inline void call() {
	typedef void(T::*MemberFunc)(void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1]);
}
END_INVOKER2;


template <class T, class T1, class T2>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2]);
}
END_INVOKER2;

template <class T, class T1, class T2, class T3>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2, T3)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3]);
}
END_INVOKER2;

template <class T, class T1, class T2, class T3, class T4>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2, T3, T4)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4]);
}
END_INVOKER2;

template <class T, class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2, T3, T4, T5)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5]);
}
END_INVOKER2;

template <class T, class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5], _params[6]);
}
END_INVOKER2;

template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7]);
}
END_INVOKER2;

template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER2(ThisInvoker, T, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef void(T::*MemberFunc)(void*, void*, void*, void*, void*, void*, void*, void*);
	T* obj = (T*)_params[0];
	(obj->*(*(MemberFunc*)(&_fx)))(_params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7], _params[8]);
}
END_INVOKER2;

template <class Ret, class T, class ...Args>
class MFunction :
	public DFunction
{
public:
	typedef Ret(T::*FuncType)(Args...);
	typedef Ret(T::*FuncTypeConst)(Args...) const;

protected:
	ThisInvoker<Ret, T, Args...> invoker;
public:

	MFunction(T* pObject, FuncType function) : MFunction(pObject, *((void**)&function) ){}
	MFunction(T* pObject, FuncTypeConst function) : MFunction(pObject, *((void**)&function)) {}
	
	MFunction(T* pObject, void* function) : invoker(function, &_ret) {
		invoker.pushParam(pObject);
	}

	virtual ~MFunction() {
	}

	Ret operator()(Args...args) {
		return ((T*)mParams[0])->*(*((FuncType*)&mFx)) (args...);
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

	virtual DFunction* clone() override {
		MFunction* newInstance = new MFunction((T*)nullptr, (void*)nullptr);
		newInstance->invoker = invoker;
		return newInstance;
	}
};


/* StdFunction cannot dynamic invoke bellow functions type. So, use SFunction instead.
 1. <return type> __stdcall::function(..., double, ...)
 2. <return type> __stdcall::function(..., float, ...)
*/
template <class Ret, class ...Args>
class SFunction : public MFunction< Ret, SFunction<Ret, Args...> , const Args& ...> {
public:
	typedef Ret(__stdcall *FuncType)(Args...);
private:
	FuncType _fx;
	inline Ret forward(const Args&... args) {
		return (_fx)(args...);
	}
public:
	SFunction(FuncType fx) :
		_fx(fx),
		MFunction(this, &SFunction::forward) {
	}
};

/* CdeclFunction cannot dynamic invoke bellow functions type. So, use Function instead.
1. <return type> __cdecl function(..., double, ...)
2. <return type> __cdecl function(..., float, ...)
*/
template <class Ret, class ...Args>
class Function : public MFunction< Ret, Function<Ret, Args...>, const Args& ...> {
public:
	typedef Ret(_cdecl *FuncType)(Args...);
private:
	FuncType _fx;
	inline Ret forward(const Args&... args) {
		return (_fx)(args...);
	}
public:
	Function(FuncType fx) :
		_fx(fx),
		MFunction(this, &Function::forward) {
	}
};

/* MemberFunction cannot dynamic invoke bellow functions type. So, use MemberFuncWrapper instead.
1. <return type> __cdecl T::function(..., double, ...)
2. <return type> __cdecl T::function(..., float, ...)
*/
template <class Ret, class T, class ...Args>
class MFunctionW : public MFunction< Ret, MFunctionW<Ret, T, Args...>, const Args& ...> {
public:
	typedef Ret(T::*FuncType)(Args...);
private:
	FuncType _fx;
	T* _obj;
	//typedef MFunctionW<Ret, T, Args...> ClassType;
	inline Ret forward(const Args&... args) {
		return (_obj->*_fx) (args...);
	}
public:
	MFunctionW(T* obj, FuncType fx) :
		_fx(fx),
		_obj(obj),
		MFunction(this, &MFunctionW::forward) {
	}

	virtual DFunction* clone() override {
		MFunctionW* newInstance = new MFunctionW(_obj, _fx);
		newInstance->invoker = invoker;
		return newInstance;
	}
};