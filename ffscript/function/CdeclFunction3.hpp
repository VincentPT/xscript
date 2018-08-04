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
	class Invoke<Ret, __VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	typedef typename real_type<Ret>::T RRT;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RRATs  = typename real_type<ATs<N>>::T;\
	public:\
		typedef Ret(*Fp)(__VA_ARGS__);\
		Fp _fx;\
	public:\
		Invoke(Fp fx) : _fx(fx) {}\
		void operator()(void* pRet, char* args);\
	}

#define DECLARE_CLASS_INVOKER_VOID(...) \
	class InvokeVoid<__VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	typedef correct_types<__VA_ARGS__> RATs;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RRATs  = typename real_type<ATs<N>>::T;\
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
		typedef void(*Fx)(RRATs<0>);
		((Fx)_fx)(*((T*)&args[0]));
	}

	template <class T1, class T2>
	void InvokeVoid<T1, T2>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]));
	}

	template <class T1, class T2, class T3>
	void InvokeVoid<T1, T2, T3>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]));
	}

	template <class T1, class T2, class T3, class T4>
	void InvokeVoid<T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5>
	void InvokeVoid<T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6>
	void InvokeVoid<T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>, RRATs<5>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]), *((RRATs<5>*)&args[Helper::offset<5>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void InvokeVoid<T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>, RRATs<5>, RRATs<6>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]), *((RRATs<5>*)&args[Helper::offset<5>()]), *((RRATs<6>*)&args[Helper::offset<6>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void InvokeVoid<T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		typedef void(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>, RRATs<5>, RRATs<6>, RRATs<7>);
		((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]), *((RRATs<5>*)&args[Helper::offset<5>()]), *((RRATs<6>*)&args[Helper::offset<6>()]), *((RRATs<7>*)&args[Helper::offset<7>()]));
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
		typedef RRT(*Fx)(RRATs<0>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]));
	}

	template <class Ret, class T1, class T2>
	void Invoke<Ret, T1, T2>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]));
	}

	template <class Ret, class T1, class T2, class T3>
	void Invoke<Ret, T1, T2, T3>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]),
			*((RRATs<2>*)&args[Helper::offset<2>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4>
	void Invoke<Ret, T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5>
	void Invoke<Ret, T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>, RRATs<5>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]), *((RRATs<5>*)&args[Helper::offset<5>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>, RRATs<5>, RRATs<6>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]), *((RRATs<5>*)&args[Helper::offset<5>()]), *((RRATs<6>*)&args[Helper::offset<6>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		typedef RRT(*Fx)(RRATs<0>, RRATs<1>, RRATs<2>, RRATs<3>, RRATs<4>, RRATs<5>, RRATs<6>, RRATs<7>);
		*((RRT*)pRet) = ((Fx)_fx)(*((RRATs<0>*)&args[0]), *((RRATs<1>*)&args[Helper::offset<1>()]), *((RRATs<2>*)&args[Helper::offset<2>()]), *((RRATs<3>*)&args[Helper::offset<3>()]),
			*((RRATs<4>*)&args[Helper::offset<4>()]), *((RRATs<5>*)&args[Helper::offset<5>()]), *((RRATs<6>*)&args[Helper::offset<6>()]), *((RRATs<7>*)&args[Helper::offset<7>()]));
	}
}

namespace FT {
	using namespace Cdel3;

	template <class Ret, class...Types>
	class CdelFunction3 : public DFunction2 {
	public:
		typedef Ret(*Fx)(Types...);
	private:
		typedef typename std::conditional<sizeof...(Types) != 0, InvokeVoid<Types...>, InvokeVoidN0>::type MyInvokerVoid;
		typedef typename std::conditional<std::is_void<Ret>::value, MyInvokerVoid, Invoke<Ret, Types...>>::type MyInvoker;
		MyInvoker _invoker;
	public:
		CdelFunction3(Fx fx) : _invoker(fx) {}

		void call(void* pReturnVal, void* params[]) {
			_invoker(pReturnVal, (char*)params);
		}
		DFunction2* clone() {
			auto funcObj = new CdelFunction3<Ret, Types...>(_invoker._fx);
			return funcObj;
		}
	};
}