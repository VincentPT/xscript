/******************************************************************
* File:        FunctionDelegate.hpp
* Description: template class and its member functions that used to
*              invoke functions. This function object stores
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

template <class ...Args> class FunctionInvoker;

template <class Ret> class FunctionInvoker<Ret> {
	Ret _retStorage;
public:
	void* _fx;
	FunctionInvoker(void* fx, void** data) : _fx(fx) { *data = &_retStorage;}
	inline void call() {
		typedef Ret(*FunctionType)();
		this->_retStorage = ((FunctionType)this->_fx)();
	}
	inline bool pushParam(void*) { return false; }
	inline void* popParam() { return nullptr; }
};

template <class Ret, class T1>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1)
	inline void call() {
		typedef Ret(*FunctionType)(void*);
		this->_retStorage = ((FunctionType)this->_fx)(this->_params[0]);
	}
END_INVOKER1;


template <class Ret, class T1, class T2>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2)
	inline void call() {
		typedef Ret(*FunctionType)(void*, void*);
		this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2, T3)
	inline void call() {
		typedef Ret(*FunctionType)(void*, void*, void*);
		this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2, T3, T4)
	inline void call() {
		typedef Ret(*FunctionType)(void*, void*, void*, void*);
		this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2, T3, T4, T5)
	inline void call() {
		typedef Ret(*FunctionType)(void*, void*, void*, void*, void*);
		this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*, void*);
	this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4], this->_params[5]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4], this->_params[5], this->_params[6]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER1(FunctionInvoker, Ret, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef Ret(*FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	this->_retStorage = ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4], this->_params[5], this->_params[6], this->_params[7]);
}
END_INVOKER1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <> class FunctionInvoker<void> {
public:
	void* _fx;
	FunctionInvoker(void* fx, void** data) : _fx(fx) { *data = nullptr; }
	inline void call() {
		typedef void(*FunctionType)();
		((FunctionType)this->_fx)();
	}
	inline bool pushParam(void*) { return false; }
	inline void* popParam() { return nullptr; }
};

template <class T1>
BEGIN_INVOKER2(FunctionInvoker, T1)
	inline void call() {
		typedef void(*FunctionType)(void*);
		((FunctionType)this->_fx)(this->_params[0]);
	}
};


template <class T1, class T2>
BEGIN_INVOKER2(FunctionInvoker, T1, T2)
	inline void call() {
		typedef void(*FunctionType)(void*, void*);
		((FunctionType)this->_fx)(this->_params[0], this->_params[1]);
	}
};

template <class T1, class T2, class T3>
BEGIN_INVOKER2(FunctionInvoker, T1, T2, T3)
	inline void call() {
		typedef void(*FunctionType)(void*, void*, void*);
		((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2]);
	}
};

template <class T1, class T2, class T3, class T4>
BEGIN_INVOKER2(FunctionInvoker, T1, T2, T3, T4)
	inline void call() {
		typedef void(*FunctionType)(void*, void*, void*, void*);
		 ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3]);
	}
};

template <class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER2(FunctionInvoker, T1, T2, T3, T4, T5)
	inline void call() {
		typedef void(*FunctionType)(void*, void*, void*, void*, void*);
		 ((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4]);
	}
};

template <class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER2(FunctionInvoker, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*, void*);
	((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4], this->_params[5]);
}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER2(FunctionInvoker, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4], this->_params[5], this->_params[6]);
}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER2(FunctionInvoker, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef void(*FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)this->_fx)(this->_params[0], this->_params[1], this->_params[2], this->_params[3], this->_params[4], this->_params[5], this->_params[6], this->_params[7]);
}
};

template <class Ret, class ...Args>
class FunctionDelegate :
	public DFunction
{
public:
	typedef Ret(*FuncType)(Args...);
protected:
	FunctionInvoker<Ret, Args...> invoker;
public:
	FunctionDelegate(FuncType function) : FunctionDelegate(*((void**)&function)) {}
	FunctionDelegate(void* function) : invoker(function, &_ret) {}
	virtual ~FunctionDelegate() {}

	virtual Ret operator()(Args...args) {
		return ((FuncType)invoker._fx)(args...);
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

	DFunction* clone() override {
		FunctionDelegate* newInstance = new FunctionDelegate(invoker._fx);
		newInstance->invoker = invoker;
		return newInstance;
	}
};

template <class Ret, class ...Args>
inline Ret cdeclForward(Ret function(Args...),const Args&... args) {
	return function(args...);
}