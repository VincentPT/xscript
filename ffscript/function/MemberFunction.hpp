/******************************************************************
* File:        MemberFunction.hpp
* Description: template class and its member functions that used to
*              invoke member functions. This function object stores
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


template <class Class, class Ret, class ...Args>
class MethodInvoker;

template <class Class, class ...Args>
class MethodInvokerVoid;

//////////////////////////////////////////////////////////////////////////////////////////////////
#define DECLARE_METHOD_INVOKER_T(Class, Ret, ...)\
	class MethodInvoker<Class, Ret, ##__VA_ARGS__> : public FunctionInvokerBase<__VA_ARGS__> {\
	protected:\
		Ret _retStorage;\
	public:\
		typedef Ret(Class::*MFx)(__VA_ARGS__);\
		typedef Ret(Class::*MFxConst)(__VA_ARGS__) const;\
		MFx _fx;\
		Class* _obj;\
		MethodInvoker(Class* obj, MFx fx, void** data) : _obj(obj), _fx(fx) {\
			*data = &_retStorage;\
		}\
		MethodInvoker(Class* obj, MFxConst fx, void** data) : MethodInvoker(obj, (MFx)fx, data) {\
		}\
		void call();\
	}

//////////////////////////////////////////////////////////////////////////////////////////////////
#define DECLARE_METHOD_INVOKER_VOID(Class,...)\
	class MethodInvokerVoid<Class, ##__VA_ARGS__> : public FunctionInvokerBase<__VA_ARGS__> {\
	public:\
		typedef void(Class::*MFx)(__VA_ARGS__);\
		typedef void(Class::*MFxConst)(__VA_ARGS__) const;\
		MFx _fx;\
		Class* _obj;\
		MethodInvokerVoid(Class* obj, MFx fx, void** data) : _obj(obj), _fx(fx) {\
			*data = nullptr;\
		}\
		MethodInvokerVoid(Class* obj, MFxConst fx, void** data) : MethodInvoker(obj, (MFx)fx, data) {\
		}\
		void call();\
	}

template <class Class>
DECLARE_METHOD_INVOKER_VOID(Class);

template <class Class, class T1>
DECLARE_METHOD_INVOKER_VOID(Class, T1);

template <class Class, class T1, class T2>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2);

template <class Class, class T1, class T2, class T3>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2, T3);

template <class Class, class T1, class T2, class T3, class T4>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2, T3, T4);

template <class Class, class T1, class T2, class T3, class T4, class T5>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2, T3, T4, T5);

template <class Class, class T1, class T2, class T3, class T4, class T5, class T6>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2, T3, T4, T5, T6);

template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2, T3, T4, T5, T6, T7);

template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
DECLARE_METHOD_INVOKER_VOID(Class, T1, T2, T3, T4, T5, T6, T7, T8);

template <class Class, class Ret>
DECLARE_METHOD_INVOKER_T(Class, Ret);

template <class Class, class Ret, class T1>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1);

template <class Class, class Ret, class T1, class T2>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2);

template <class Class, class Ret, class T1, class T2, class T3>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2, T3);

template <class Class, class Ret, class T1, class T2, class T3, class T4>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2, T3, T4);

template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2, T3, T4, T5);

template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6);

template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6, T7);

template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
DECLARE_METHOD_INVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6, T7, T8);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Class>
void MethodInvokerVoid<Class>::call() {
	typedef void(Class::*RFx)();
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)();
}

template<class Class, class T1>
void MethodInvokerVoid<Class, T1>::call() {
	typedef void(Class::*RFx)(void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0]);
}

template<class Class, class T1, class T2>
void MethodInvokerVoid<Class, T1, T2>::call() {
	typedef void(Class::*RFx)(void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1]);
}

template<class Class, class T1, class T2, class T3>
void MethodInvokerVoid<Class, T1, T2, T3>::call() {
	typedef void(Class::*RFx)(void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1], this->_args[2]);
}

template<class Class, class T1, class T2, class T3, class T4>
void MethodInvokerVoid<Class, T1, T2, T3, T4>::call() {
	typedef void(Class::*RFx)(void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3]);
}

template<class Class, class T1, class T2, class T3, class T4, class T5>
void MethodInvokerVoid<Class, T1, T2, T3, T4, T5>::call() {
	typedef void(Class::*RFx)(void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4]);
}


template<class Class, class T1, class T2, class T3, class T4, class T5, class T6>
void MethodInvokerVoid<Class, T1, T2, T3, T4, T5, T6>::call() {
	typedef void(Class::*RFx)(void*, void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5]);
}

template<class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
void MethodInvokerVoid<Class, T1, T2, T3, T4, T5, T6, T7>::call() {
	typedef void(Class::*RFx)(void*, void*, void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6]);
}

template<class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
void MethodInvokerVoid<Class, T1, T2, T3, T4, T5, T6, T7, T8>::call() {
	typedef void(Class::*RFx)(void*, void*, void*, void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	(_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6], this->_args[7]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Class, class Ret>
void MethodInvoker<Class, Ret>::call() {
	typedef Ret(Class::*RFx)();
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)();
}

template<class Class, class Ret, class T1>
void MethodInvoker<Class, Ret, T1>::call() {
	typedef Ret(Class::*RFx)(void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0]);
}

template<class Class, class Ret, class T1, class T2>
void MethodInvoker<Class, Ret, T1, T2>::call() {
	typedef Ret(Class::*RFx)(void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1]);
}

template<class Class, class Ret, class T1, class T2, class T3>
void MethodInvoker<Class, Ret, T1, T2, T3>::call() {
	typedef Ret(Class::*RFx)(void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1], this->_args[2]);
}

template<class Class, class Ret, class T1, class T2, class T3, class T4>
void MethodInvoker<Class, Ret, T1, T2, T3, T4>::call() {
	typedef Ret(Class::*RFx)(void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3]);
}

template<class Class, class Ret, class T1, class T2, class T3, class T4, class T5>
void MethodInvoker<Class, Ret, T1, T2, T3, T4, T5>::call() {
	typedef Ret(Class::*RFx)(void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4]);
}


template<class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
void MethodInvoker<Class, Ret, T1, T2, T3, T4, T5, T6>::call() {
	typedef Ret(Class::*RFx)(void*, void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5]);
}

template<class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
void MethodInvoker<Class, Ret, T1, T2, T3, T4, T5, T6, T7>::call() {
	typedef Ret(Class::*RFx)(void*, void*, void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6]);
}

template<class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
void MethodInvoker<Class, Ret, T1, T2, T3, T4, T5, T6, T7, T8>::call() {
	typedef Ret(Class::*RFx)(void*, void*, void*, void*, void*, void*, void*, void*);
	RFx fx = FunctionConverter<MFx, RFx>::convert(_fx);
	_retStorage = (_obj->*fx)(this->_args[0], this->_args[1], this->_args[2], this->_args[3], this->_args[4], this->_args[5], this->_args[6], this->_args[7]);
}

template <class T, class Ret, class ...Args>
class MFunction :
	public DFunction
{
public:
	typedef Ret(T::*MFx)(Args...);
	typedef Ret(T::*MFxConst)(Args...) const;

protected:
	typedef typename std::conditional<std::is_void<Ret>::value, MethodInvokerVoid<T, Args...>, MethodInvoker<T, Ret, Args...>>::type MyInvoker;
	MyInvoker invoker;
public:

	MFunction(T* pObject, MFx function) : invoker(pObject, function, &_ret){}
	MFunction(T* pObject, MFxConst function) : MFunction(pObject, (MFx)function) {}

	virtual ~MFunction() {
	}

	Ret operator()(Args...args) {
		return ((T*)this->mParams[0])->*(*((FuncType*)&this->mFx)) (args...);
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
		MFunction* newInstance = new MFunction(invoker._obj, invoker._fx);
		newInstance->invoker = invoker;
		return newInstance;
	}
};


/* StdFunction cannot dynamic invoke bellow functions type. So, use SFunction instead.
 1. <return type> __stdcall::function(..., double, ...)
 2. <return type> __stdcall::function(..., float, ...)
*/
//template <class Ret, class ...Args>
//class SFunction : public MFunction< Ret, SFunction<Ret, Args...> , const Args& ...> {
//public:
//	typedef Ret(__stdcall *FuncType)(Args...);
//private:
//	FuncType _fx;
//	inline Ret forward(const Args&... args) {
//		return (_fx)(args...);
//	}
//public:
//	SFunction(FuncType fx) :
//		_fx(fx),
//		MFunction(this, &SFunction::forward) {
//	}
//};

/* CdeclFunction cannot dynamic invoke bellow functions type. So, use Function instead.
1. <return type> __cdecl function(..., double, ...)
2. <return type> __cdecl function(..., float, ...)
*/
//template <class Ret, class ...Args>
//class Function : public MFunction<Function<Ret, Args...>, Ret, const Args& ...> {
//public:
//	typedef Ret(*FuncType)(Args...);
//private:
//	FuncType _fx;
//	inline Ret forward(const Args&... args) {
//		return (_fx)(args...);
//	}
//public:
//	Function(FuncType fx) :
//		_fx(fx),
//		MFunction<Ret, Function<Ret, Args...>, const Args& ...>(this, &Function::forward) {
//	}
//};

/* MemberFunction cannot dynamic invoke bellow functions type. So, use MemberFuncWrapper instead.
1. <return type> __cdecl T::function(..., double, ...)
2. <return type> __cdecl T::function(..., float, ...)
*/
//template <class T, class Ret, class ...Args>
//class MFunctionW : public MFunction<MFunctionW<Ret, T, Args...>, Ret, const Args& ...> {
//public:
//	typedef Ret(T::*FuncType)(Args...);
//private:
//	FuncType _fx;
//	T* _obj;
//	//typedef MFunctionW<Ret, T, Args...> ClassType;
//	inline Ret forward(const Args&... args) {
//		return (_obj->*_fx) (args...);
//	}
//public:
//	MFunctionW(T* obj, FuncType fx) :
//		_fx(fx),
//		_obj(obj),
//		MFunction< Ret, MFunctionW<Ret, T, Args...>, const Args& ...>(this, &MFunctionW::forward) {
//	}
//
//	virtual DFunction* clone() override {
//		MFunctionW* newInstance = new MFunctionW(_obj, _fx);
//		newInstance->invoker = this->invoker;
//		return newInstance;
//	}
//};