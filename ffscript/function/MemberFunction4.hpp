/******************************************************************
* File:        MemberFunction4.hpp
* Description: template class and its member functions that used to
*              invoke member functions. This function object does
*              not store any arguments of its target function when
*              it is created. The caller need to pass arguments
*              through and array of bytes to invoke the target
*              function.
**             MFunction4 is use to call method like MFunction3
**             but not store context object.
**             Context object will be supplied at calling time.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "DynamicFunction2.h"
#include "MemberTypeInfo.hpp"

namespace MemberFunction4 {
	using namespace FT;

	template <class Class, class...Types>
	struct CtxInvokeVoid;

	template <class Class, class Ret, class...Types>
	struct CtxInvoke;

#define DECLARE_CLASS_CTXINVOKER_T(Class, Ret, ...)\
	struct CtxInvoke<Class, Ret, __VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	typedef typename real_type<Ret>::T RRT;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RATs  = typename real_type<ATs<N>>::T;\
	public:\
		typedef Ret(Class::*Fp)(__VA_ARGS__);\
		Fp _fx;\
	public:\
		CtxInvoke(Fp fx) : _fx(fx) {}\
		void operator()(void* pRet, Class* obj, char* args);\
	}

#define DECLARE_CLASS_CTXINVOKER_VOID(Class,...) \
	struct CtxInvokeVoid<Class, __VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RATs  = typename real_type<ATs<N>>::T;\
	public:\
		typedef void(Class::*Fp)(__VA_ARGS__);\
		Fp _fx;\
	public:\
		CtxInvokeVoid(Fp fx) : _fx(fx) {}\
		void operator()(void* pRet, Class* obj, char* args);\
	}

	template <class Class>
	DECLARE_CLASS_CTXINVOKER_VOID(Class);

	template <class Class, class T>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T);

	template <class Class, class T1, class T2>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2);

	template <class Class, class T1, class T2, class T3>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2, T3);

	template <class Class, class T1, class T2, class T3, class T4>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2, T3, T4);

	template <class Class, class T1, class T2, class T3, class T4, class T5>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2, T3, T4, T5);

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2, T3, T4, T5, T6);

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2, T3, T4, T5, T6, T7);

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	DECLARE_CLASS_CTXINVOKER_VOID(Class, T1, T2, T3, T4, T5, T6, T7, T8);

	template <class Class, class Ret>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret);

	template <class Class, class Ret, class T>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T);

	template <class Class, class Ret, class T1, class T2>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2);

	template <class Class, class Ret, class T1, class T2, class T3>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2, T3);

	template <class Class, class Ret, class T1, class T2, class T3, class T4>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2, T3, T4);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2, T3, T4, T5);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6, T7);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	DECLARE_CLASS_CTXINVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6, T7, T8);

	template <class Class>
	void CtxInvokeVoid<Class>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)();
		(obj->*(Fx)_fx)();
	}

	template <class Class, class T>
	void CtxInvokeVoid<Class, T>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]));
	}

	template <class Class, class T1, class T2>
	void CtxInvokeVoid<Class, T1, T2>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>);
		auto xxx = Helper::offset<1>();
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[xxx]));
	}

	template <class Class, class T1, class T2, class T3>
	void CtxInvokeVoid<Class, T1, T2, T3>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4>
	void CtxInvokeVoid<Class, T1, T2, T3, T4>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5>
	void CtxInvokeVoid<Class, T1, T2, T3, T4, T5>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6>
	void CtxInvokeVoid<Class, T1, T2, T3, T4, T5, T6>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void CtxInvokeVoid<Class, T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void CtxInvokeVoid<Class, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, Class* obj, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>, RATs<7>);
		(obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]), *((RATs<7>*)&args[Helper::offset<7>()]));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// implementation of invoke type t
	///
	////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Class, class Ret>
	void CtxInvoke<Class, Ret>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)();
		*((RRT*)pRet) = (obj->*(Fx)_fx)();
	}

	template <class Class, class Ret, class T>
	void CtxInvoke<Class, Ret, T>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]));
	}

	template <class Class, class Ret, class T1, class T2>
	void CtxInvoke<Class, Ret, T1, T2>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3>
	void CtxInvoke<Class, Ret, T1, T2, T3>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]),
			*((RATs<2>*)&args[Helper::offset<2>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4>
	void CtxInvoke<Class, Ret, T1, T2, T3, T4>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5>
	void CtxInvoke<Class, Ret, T1, T2, T3, T4, T5>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	void CtxInvoke<Class, Ret, T1, T2, T3, T4, T5, T6>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void CtxInvoke<Class, Ret, T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void CtxInvoke<Class, Ret, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, Class* obj, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>, RATs<7>);
		*((RRT*)pRet) = (obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]), *((RATs<7>*)&args[Helper::offset<7>()]));
	}
}

namespace FT {
	using namespace MemberFunction4;

	template <class Class, class Ret, class...Types>
	class MFunction4 : public DFunction2 {
	public:
		typedef Ret(Class::*MFx)(Types...);
		typedef Ret(Class::*MFxConst)(Types...) const;
		typedef Ret(*Fx)(Class*, Types...);
	private:
		typedef typename std::conditional<std::is_void<Ret>::value, CtxInvokeVoid<Class, Types...>, CtxInvoke<Class, Ret, Types...>>::type MyInvoker;
		MyInvoker _invoker;
	public:
		MFunction4(MFx mfx) : _invoker(mfx) {}
		MFunction4(MFxConst mfx) : _invoker((MFx)mfx) {}

		void call(void* pReturnVal, void* params[]) {
			_invoker(pReturnVal, (Class*)params[0], (char*)&params[1]);
		}
		DFunction2* clone() {
			auto funcObj = new MFunction4<Class, Ret, Types...>(_invoker._fx);
			return funcObj;
		}

		static Fx convertToFunction(MFx mfx) {
			return (Fx)*((void**)&mfx);
		}

		static Fx convertToFunction(MFxConst mfx) {
			return (Fx)*((void**)&mfx);
		}
	};
}