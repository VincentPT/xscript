/******************************************************************
* File:        CachedMethod.hpp
* Description: template class and its member functions that used to
*              invoke _cdecl functions. This function object stores
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
#include "MemberFunction3.hpp"
#include "CachedDelegate.h"
#include <exception>

namespace CachedMethodInvoker {
	using namespace FT;

	struct SetArgRef {
		template <class T>
		static void set(char* base, T val) {
			typedef typename std::remove_reference<T>::type val_type;
			*(val_type**)base = &val;
		}
	};

	struct SetArgVal {
		template <class T>
		static void set(char* base, T val) {
			*(T*)base = val;
		}
	};

	template <size_t alignment>
	struct ArgumentTuple {
		template <class...Types>
		static void setArg(char* base, Types...) {}

		template <class First>
		static void setArg(char* base, First first) {
			typedef typename std::conditional<std::is_reference<First>::value, SetArgRef, SetArgVal>::type SetArg;
			SetArg::template set<First>(base, first);
		}

		template <class First, class...Rest>
		static void setArg(char* base, First first, Rest...rest) {
			typedef typename std::conditional<std::is_reference<First>::value, SetArgRef, SetArgVal>::type SetArg;
			SetArg::template set<First>(base, first);

			constexpr int size = FT::getAlignSize<First, alignment>();
			ArgumentTuple<alignment>::setArg<Rest...>(base + size, rest...);
			
		}
	};

	template <class Class, class Ret, class...Types>
	class CachedMethodInvoker {
		typedef MFunction3<Class, Ret, Types...> RealDelegateType;
		static constexpr size_t aligment = RealDelegateType::getAlignment();
	protected:
		char _argumentData[RealDelegateType::getArgumentStorageSize() + sizeof(void*)];
	public:
		void setArgs(Types... types) {
			ArgumentTuple<aligment>::setArg<Types...>(_argumentData, types...);
		}

		//virtual void invoke() = 0;
	};

	template <class Class, class Ret, class...Types>
	class CachedMethodInvokerRet : public CachedMethodInvoker<Class, Ret, Types...> {
		typedef MInvoke<Class, Ret, Types...> RealInvokerType;
		typedef typename RealInvokerType::RRT RRT;
		RRT _ret;
	public:
		typedef Ret(Class::*MFx)(Types...);
		typedef Ret(Class::*MFxConst)(Types...) const;
		RealInvokerType _realInvoker;
	public:
		CachedMethodInvokerRet(void** ref, Class* obj, MFx fx) : _realInvoker(obj, fx) {
			*ref = &_ret;
		}
		CachedMethodInvokerRet(void** ref, Class* obj, MFxConst fx) : _realInvoker(obj, (MFx)fx) {
			*ref = &_ret;
		}

		void invoke() {
			_realInvoker(&_ret, this->_argumentData);
		}
	};

	template <class Class, class Ret, class...Types>
	class CachedMethodInvokerVoid : public CachedMethodInvoker<Class, Ret, Types...> {
	public:
		typedef Ret(Class::*MFx)(Types...);
		typedef Ret(Class::*MFxConst)(Types...) const;
		MInvokeVoid<Class, Types...> _realInvoker;
	public:
		CachedMethodInvokerVoid(void** ref, Class* obj, MFx fx) : _realInvoker(obj, fx) {
			*ref = nullptr;
		}
		CachedMethodInvokerVoid(void** ref, Class* obj, MFxConst fx) : _realInvoker(obj, (MFx)fx) {
			*ref = nullptr;
		}

		void invoke() {
			_realInvoker(nullptr, this->_argumentData);
		}
	};
}

namespace FT {
	using namespace CachedMethodInvoker;

	template <class Class, class Ret, class...Types>
	class CachedMethodDelegate : public CachedDelegate {
	private:
		typedef typename std::conditional<std::is_void<Ret>::value, CachedMethodInvokerVoid<Class, Ret, Types...>, CachedMethodInvokerRet<Class, Ret, Types...>>::type MyInvoker;
		MyInvoker _invoker;
	public:
		typedef Ret(Class::*MFx)(Types...);
		typedef Ret(Class::*MFxConst)(Types...) const;
	public:
		CachedMethodDelegate(Class* obj, MFx mfx) : _invoker(&_ret, obj, mfx) {}
		CachedMethodDelegate(Class* obj, MFxConst mfx) : _invoker(&_ret, obj, (MFx)mfx) {}

		void setArgs(Types...types) {
			_invoker.setArgs(types...);
		}
		void call() override {
			return _invoker.invoke();
		}
	};
}