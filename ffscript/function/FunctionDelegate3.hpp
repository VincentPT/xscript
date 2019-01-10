/******************************************************************
* File:        CdeclFunction3.hpp
* Description: template class and its member functions that used to
*              invoke _cdecl functions. This function object does
*              not store any arguments of its target function when
*              it is created. The caller need to pass arguments
*              through and array of bytes to invoke the target
*              function.
**             CdeclFunction3 class is created to replace FunctionDelegate2.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "DynamicFunction2.h"
#include "MemberTypeInfo.hpp"

namespace Cdel3 {
	using namespace FT;

	template <class...Types>
	struct InvokeVoid;

	template <class Ret, class...Types>
	struct Invoke;

#define DECLARE_CLASS_INVOKER_T(Ret, ...)\
	struct Invoke<Ret, ##__VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), ##__VA_ARGS__> Helper;\
	typedef typename real_type<Ret>::_T RRT;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RATs  = typename real_type<ATs<N>>::_T;\
	public:\
		typedef Ret(*Fp)(__VA_ARGS__);\
		Fp _fx;\
	public:\
		Invoke(Fp fx) : _fx(fx) {}\
		void operator()(void* pRet, char* args);\
	}

#define DECLARE_CLASS_INVOKER_VOID(...) \
	struct InvokeVoid<__VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), ##__VA_ARGS__> Helper;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RATs  = typename real_type<ATs<N>>::_T;\
	public:\
		typedef void(*Fp)(__VA_ARGS__);\
		Fp _fx;\
	public:\
		InvokeVoid(Fp fx) : _fx(fx) {}\
		void operator()(void* pRet, char* args);\
	}

	//template <>
	//DECLARE_CLASS_INVOKER_VOID();

	template <class T>
	DECLARE_CLASS_INVOKER_VOID(T);

	template <class T1, class T2>
	DECLARE_CLASS_INVOKER_VOID(T1, T2);

	template <class T1, class T2, class T3>
	DECLARE_CLASS_INVOKER_VOID(T1, T2, T3);

	template <class T1, class T2, class T3, class T4>
	DECLARE_CLASS_INVOKER_VOID(T1, T2, T3, T4);

	template <class T1, class T2, class T3, class T4, class T5>
	DECLARE_CLASS_INVOKER_VOID(T1, T2, T3, T4, T5);

	template <class T1, class T2, class T3, class T4, class T5, class T6>
	DECLARE_CLASS_INVOKER_VOID(T1, T2, T3, T4, T5, T6);

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	DECLARE_CLASS_INVOKER_VOID(T1, T2, T3, T4, T5, T6, T7);

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	DECLARE_CLASS_INVOKER_VOID(T1, T2, T3, T4, T5, T6, T7, T8);

	template <class Ret>
	DECLARE_CLASS_INVOKER_T(Ret);

	template <class Ret, class T>
	DECLARE_CLASS_INVOKER_T(Ret, T);

	template <class Ret, class T1, class T2>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2);

	template <class Ret, class T1, class T2, class T3>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2, T3);

	template <class Ret, class T1, class T2, class T3, class T4>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2, T3, T4);

	template <class Ret, class T1, class T2, class T3, class T4, class T5>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2, T3, T4, T5);

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2, T3, T4, T5, T6);

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2, T3, T4, T5, T6, T7);

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	DECLARE_CLASS_INVOKER_T(Ret, T1, T2, T3, T4, T5, T6, T7, T8);

	//template <>
	//void InvokeVoid<>::operator()(void* pRet, char* args) {
	//	_fx();
	//}

	struct InvokeVoidN0 {
	public:
		typedef void(*Fp)();
		Fp _fx;
	public:
		InvokeVoidN0(Fp fx) : _fx(fx) {}
		void operator()(void* pRet, char* args) {
			_fx();
		}
	};

	template <class T>
	void InvokeVoid<T>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>);
		((Fx)_fx)(*((RATs<0>*)&args[0]));
	}

	template <class T1, class T2>
	void InvokeVoid<T1, T2>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]));
	}

	template <class T1, class T2, class T3>
	void InvokeVoid<T1, T2, T3>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>, RATs<2>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]));
	}

	template <class T1, class T2, class T3, class T4>
	void InvokeVoid<T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]));
	}

	template <class T1, class T2, class T3, class T4, class T5>
	void InvokeVoid<T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6>
	void InvokeVoid<T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]), *((RATs<5>*)&args[ARG_OFFSET(5)]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void InvokeVoid<T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]), *((RATs<5>*)&args[ARG_OFFSET(5)]), *((RATs<6>*)&args[ARG_OFFSET(6)]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void InvokeVoid<T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>, RATs<7>);
		((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]), *((RATs<5>*)&args[ARG_OFFSET(5)]), *((RATs<6>*)&args[ARG_OFFSET(6)]), *((RATs<7>*)&args[ARG_OFFSET(7)]));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// implementation of invoke type t
	///
	////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Ret>
	void Invoke<Ret>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)();
		*((RRT*)pRet) = ((Fx)_fx)();
	}

	template <class Ret, class T>
	void Invoke<Ret, T>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]));
	}

	template <class Ret, class T1, class T2>
	void Invoke<Ret, T1, T2>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]));
	}

	template <class Ret, class T1, class T2, class T3>
	void Invoke<Ret, T1, T2, T3>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>, RATs<2>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]),
			*((RATs<2>*)&args[ARG_OFFSET(2)]));
	}

	template <class Ret, class T1, class T2, class T3, class T4>
	void Invoke<Ret, T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5>
	void Invoke<Ret, T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]), *((RATs<5>*)&args[ARG_OFFSET(5)]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]), *((RATs<5>*)&args[ARG_OFFSET(5)]), *((RATs<6>*)&args[ARG_OFFSET(6)]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>, RATs<7>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[ARG_OFFSET(1)]), *((RATs<2>*)&args[ARG_OFFSET(2)]), *((RATs<3>*)&args[ARG_OFFSET(3)]),
			*((RATs<4>*)&args[ARG_OFFSET(4)]), *((RATs<5>*)&args[ARG_OFFSET(5)]), *((RATs<6>*)&args[ARG_OFFSET(6)]), *((RATs<7>*)&args[ARG_OFFSET(7)]));
	}
}

namespace FT {
	using namespace Cdel3;

	template <class Ret, class...Types>
	class FunctionDelegate3 : public DFunction2 {
	public:
		typedef Ret(*Fx)(Types...);
	private:
		typedef typename std::conditional<sizeof...(Types) != 0, InvokeVoid<Types...>, InvokeVoidN0>::type MyInvokerVoid;
		typedef typename std::conditional<std::is_void<Ret>::value, MyInvokerVoid, Invoke<Ret, Types...>>::type MyInvoker;
		MyInvoker _invoker;
	public:
		FunctionDelegate3(Fx fx) : _invoker(fx) {}

		void call(void* pReturnVal, void* params[]) {
			_invoker(pReturnVal, (char*)params);
		}
		DFunction2* clone() {
			auto funcObj = new FunctionDelegate3<Ret, Types...>(_invoker._fx);
			return funcObj;
		}
	};
}