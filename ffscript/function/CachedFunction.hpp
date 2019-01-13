/******************************************************************
* File:        CachedFunction.hpp
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

#include "FunctionDelegate3.hpp"
#include "CachedDelegate.h"
#include "FTCachedDelegateInternal.hpp"

namespace CachedFunctionInvoker {
	using namespace FT;
	using namespace FTCached;

	template <class Ret, class...Types>
	class CachedFunctionInvokerRet : public FTCachedInvoker<Types...> {
		typedef Invoke<Ret, Types...> RealInvokerType;
		typedef typename RealInvokerType::RRT RRT;
		RRT _ret;
	public:
		typedef Ret(*Fx)(Types...);
		RealInvokerType _realInvoker;
	public:
		CachedFunctionInvokerRet(void** ref, Fx fx) : _realInvoker(fx) {
			*ref = &_ret;
		}

		void invoke() {
			_realInvoker(&_ret, this->_argumentData);
		}
	};

	template <class...Types>
	class CachedFunctionInvokerVoid : public FTCachedInvoker<Types...> {
	public:
		typedef void(*Fx)(Types...);
		InvokeVoid<Types...> _realInvoker;
	public:
		CachedFunctionInvokerVoid(void** ref, Fx fx) : _realInvoker(fx) {
			*ref = nullptr;
		}

		void invoke() {
			_realInvoker(nullptr, this->_argumentData);
		}
	};
}

namespace FT {
	using namespace CachedFunctionInvoker;

	template <class Ret, class...Types>
	class CachedFunctionDelegate : public CachedDelegate {
	private:
		typedef typename std::conditional<std::is_void<Ret>::value, CachedFunctionInvokerVoid<Types...>, CachedFunctionInvokerRet<Ret, Types...>>::type MyInvoker;
		MyInvoker _invoker;
	public:
		typedef Ret(Fx)(Types...);
	public:
		CachedFunctionDelegate(Fx mfx) : _invoker(&_ret, mfx) {}

		void setArgs(Types...types) {
			_invoker.setArgs(types...);
		}
		void call() override {
			return _invoker.invoke();
		}
	};
}