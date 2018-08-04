#pragma once
#include "DynamicFunction2.h"
#include "MemberTypeInfo.hpp"

namespace Cdel3 {
	template <class...Types>
	struct InvokeVoid;

	template <class Ret, class...Types>
	struct Invoke;

#define DECLARE_CLASS_INVOKER_T(Ret, ...)\
	class Invoke<Ret, __VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	typedef correct_types<__VA_ARGS__> RATs;\
	typedef typename correct_types<Ret>::T RRT;\
	public:\
		typedef Ret(*Fp)(__VA_ARGS__);\
		Fp _fx;\
	public:\
		Invoke(Fp fx) : _fx(fx) {}\
		void operator()(Ret* pRet, char* args);\
	}

#define DECLARE_CLASS_INVOKER_VOID(...) \
	class InvokeVoid<__VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	typedef correct_types<__VA_ARGS__> RATs;\
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
		_fx(*((T*)&args[0]));
	}

	template <class T1, class T2>
	void InvokeVoid<T1, T2>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]));
	}

	template <class T1, class T2, class T3>
	void InvokeVoid<T1, T2, T3>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]));
	}

	template <class T1, class T2, class T3, class T4>
	void InvokeVoid<T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5>
	void InvokeVoid<T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6>
	void InvokeVoid<T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]), *((T6*)&args[Helper::getOffset<5>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void InvokeVoid<T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]), *((T6*)&args[Helper::getOffset<5>()]), *((T7*)&args[Helper::getOffset<6>()]));
	}

	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void InvokeVoid<T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		_fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]), *((T6*)&args[Helper::getOffset<5>()]), *((T7*)&args[Helper::getOffset<6>()]), *((T8*)&args[Helper::getOffset<7>()]));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// implementation of invoke type t
	///
	////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Ret>
	void Invoke<Ret>::operator()(Ret* pRet, char* args) {
		*pRet = _fx();
	}

	template <class Ret, class T>
	void Invoke<Ret, T>::operator()(Ret* pRet, char* args) {
		typedef RRT(*Fx)(RATs::T);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs::T*)&args[0]));
	}

	template <class Ret, class T1, class T2>
	void Invoke<Ret, T1, T2>::operator()(Ret* pRet, char* args) {
		typedef RRT(*Fx)(RATs::T, RATs::sub::T);
		*((RRT*)pRet) = ((Fx)_fx)(*((RATs::T*)&args[0]), *((RATs::sub::T*)&args[Helper::getOffset<1>()]));
	}

	template <class Ret, class T1, class T2, class T3>
	void Invoke<Ret, T1, T2, T3>::operator()(Ret* pRet, char* args) {
		typedef RRT(*Fx)(RATs::T, RATs::sub::T, RATs::sub::sub::T);

		*((RRT*)pRet) = ((Fx)_fx)(*((RATs::T*)&args[0]), *((RATs::sub::T*)&args[Helper::getOffset<1>()]),
			*((RATs::sub::sub::T*)&args[Helper::getOffset<2>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4>
	void Invoke<Ret, T1, T2, T3, T4>::operator()(Ret* pRet, char* args) {
		*pRet = _fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5>
	void Invoke<Ret, T1, T2, T3, T4, T5>::operator()(Ret* pRet, char* args) {
		*pRet = _fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6>::operator()(Ret* pRet, char* args) {
		*pRet = _fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]), *((T6*)&args[Helper::getOffset<5>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6, T7>::operator()(Ret* pRet, char* args) {
		*pRet = _fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]), *((T6*)&args[Helper::getOffset<5>()]), *((T7*)&args[Helper::getOffset<6>()]));
	}

	template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void Invoke<Ret, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(Ret* pRet, char* args) {
		*pRet = _fx(*((RATs::T*)&args[0]), *((T2*)&args[Helper::getOffset<1>()]), *((T3*)&args[Helper::getOffset<2>()]), *((T4*)&args[Helper::getOffset<3>()]),
			*((T5*)&args[Helper::getOffset<4>()]), *((T6*)&args[Helper::getOffset<5>()]), *((T7*)&args[Helper::getOffset<6>()]), *((T8*)&args[Helper::getOffset<7>()]));
	}
}

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
		_invoker( (Ret*) pReturnVal, (char*)params);
	}
	DFunction2* clone() {
		auto funcObj = new CdelFunction3<Ret, Types...>(_invoker._fx);
		return funcObj;
	}
};