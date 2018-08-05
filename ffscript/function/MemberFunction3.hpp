#pragma once
#include "DynamicFunction2.h"
#include "MemberTypeInfo.hpp"

namespace MemberFunction3 {
	using namespace FT;

	template <class Class, class...Types>
	struct MInvokeVoid;

	template <class Class, class Ret, class...Types>
	struct MInvoke;

#define DECLARE_CLASS_MINVOKER_T(Class, Ret, ...)\
	struct MInvoke<Class, Ret, __VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	typedef typename real_type<Ret>::T RRT;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RATs  = typename real_type<ATs<N>>::T;\
	public:\
		typedef Ret(Class::*Fp)(__VA_ARGS__);\
		Class* _obj;\
		Fp _fx;\
	public:\
		MInvoke(Class* obj, Fp fx) : _obj(obj), _fx(fx) {}\
		void operator()(void* pRet, char* args);\
	}

#define DECLARE_CLASS_MINVOKER_VOID(Class,...) \
	struct MInvokeVoid<Class, __VA_ARGS__> {\
	private:\
	typedef MemberTypeInfo<0, sizeof(void*), __VA_ARGS__> Helper;\
	template <std::size_t N>\
	using ATs  = typename std::tuple_element<N, std::tuple<__VA_ARGS__>>::type;\
	template <std::size_t N>\
	using RATs  = typename real_type<ATs<N>>::T;\
	public:\
		typedef void(Class::*Fp)(__VA_ARGS__);\
		Class* _obj;\
		Fp _fx;\
	public:\
		MInvokeVoid(Class* obj, Fp fx) : _obj(obj), _fx(fx) {}\
		void operator()(void* pRet, char* args);\
	}

	template <class Class>
	DECLARE_CLASS_MINVOKER_VOID(Class);

	template <class Class, class T>
	DECLARE_CLASS_MINVOKER_VOID(Class, T);

	template <class Class, class T1, class T2>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2);

	template <class Class, class T1, class T2, class T3>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2, T3);

	template <class Class, class T1, class T2, class T3, class T4>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2, T3, T4);

	template <class Class, class T1, class T2, class T3, class T4, class T5>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2, T3, T4, T5);

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2, T3, T4, T5, T6);

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2, T3, T4, T5, T6, T7);

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	DECLARE_CLASS_MINVOKER_VOID(Class, T1, T2, T3, T4, T5, T6, T7, T8);

	template <class Class, class Ret>
	DECLARE_CLASS_MINVOKER_T(Class, Ret);

	template <class Class, class Ret, class T>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T);

	template <class Class, class Ret, class T1, class T2>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2);

	template <class Class, class Ret, class T1, class T2, class T3>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2, T3);

	template <class Class, class Ret, class T1, class T2, class T3, class T4>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2, T3, T4);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2, T3, T4, T5);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6, T7);

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	DECLARE_CLASS_MINVOKER_T(Class, Ret, T1, T2, T3, T4, T5, T6, T7, T8);

	template <class Class>
	void MInvokeVoid<Class>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)();
		(_obj->*(Fx)_fx)();
	}

	template <class Class, class T>
	void MInvokeVoid<Class, T>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]));
	}

	template <class Class, class T1, class T2>
	void MInvokeVoid<Class, T1, T2>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]));
	}

	template <class Class, class T1, class T2, class T3>
	void MInvokeVoid<Class, T1, T2, T3>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4>
	void MInvokeVoid<Class, T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5>
	void MInvokeVoid<Class, T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6>
	void MInvokeVoid<Class, T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void MInvokeVoid<Class, T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]));
	}

	template <class Class, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void MInvokeVoid<Class, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		typedef void(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>, RATs<7>);
		(_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]), *((RATs<7>*)&args[Helper::offset<7>()]));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///
	/// implementation of invoke type t
	///
	////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Class, class Ret>
	void MInvoke<Class, Ret>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)();
		*((RRT*)pRet) = (_obj->*(Fx)_fx)();
	}

	template <class Class, class Ret, class T>
	void MInvoke<Class, Ret, T>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]));
	}

	template <class Class, class Ret, class T1, class T2>
	void MInvoke<Class, Ret, T1, T2>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3>
	void MInvoke<Class, Ret, T1, T2, T3>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]),
			*((RATs<2>*)&args[Helper::offset<2>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4>
	void MInvoke<Class, Ret, T1, T2, T3, T4>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5>
	void MInvoke<Class, Ret, T1, T2, T3, T4, T5>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
	void MInvoke<Class, Ret, T1, T2, T3, T4, T5, T6>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void MInvoke<Class, Ret, T1, T2, T3, T4, T5, T6, T7>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]));
	}

	template <class Class, class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void MInvoke<Class, Ret, T1, T2, T3, T4, T5, T6, T7, T8>::operator()(void* pRet, char* args) {
		typedef RRT(Class::*Fx)(RATs<0>, RATs<1>, RATs<2>, RATs<3>, RATs<4>, RATs<5>, RATs<6>, RATs<7>);
		*((RRT*)pRet) = (_obj->*(Fx)_fx)(*((RATs<0>*)&args[0]), *((RATs<1>*)&args[Helper::offset<1>()]), *((RATs<2>*)&args[Helper::offset<2>()]), *((RATs<3>*)&args[Helper::offset<3>()]),
			*((RATs<4>*)&args[Helper::offset<4>()]), *((RATs<5>*)&args[Helper::offset<5>()]), *((RATs<6>*)&args[Helper::offset<6>()]), *((RATs<7>*)&args[Helper::offset<7>()]));
	}
}

namespace FT {
	using namespace MemberFunction3;

	template <class Class, class Ret, class...Types>
	class MFunction3 : public DFunction2 {
	public:
		typedef Ret(Class::*Fx)(Types...);
	private:
		typedef typename std::conditional<std::is_void<Ret>::value, MInvokeVoid<Class, Types...>, MInvoke<Class, Ret, Types...>>::type MyInvoker;
		MyInvoker _invoker;
	public:
		MFunction3(Class* obj, Fx fx) : _invoker(obj, fx) {}

		void call(void* pReturnVal, void* params[]) {
			_invoker(pReturnVal, (char*)params);
		}
		DFunction2* clone() {
			auto funcObj = new MFunction3<Class, Ret, Types...>(_invoker._obj, _invoker._fx);
			return funcObj;
		}
	};
}