#pragma once
#include "DynamicFunction.h"
#include "functioninternal.hpp"
#include <cstring>

template <class ...Args> class CdeclInvoker;

template <class Ret> class CdeclInvoker<Ret> {
	Ret _retStorage;
public:
	void* _fx;
	CdeclInvoker(void* fx, void** data) : _fx(fx) { *data = &_retStorage;}
	inline void call() {
		typedef Ret(_cdecl *FunctionType)();
		_retStorage = ((FunctionType)_fx)();
	}
	inline bool pushParam(void*) { return false; }
	inline void* popParam() { return nullptr; }
};

template <class Ret, class T1>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1)
	inline void call() {
		typedef Ret(_cdecl *FunctionType)(void*);
		_retStorage = ((FunctionType)_fx)(_params[0]);
	}
END_INVOKER1;


template <class Ret, class T1, class T2>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2)
	inline void call() {
		typedef Ret(_cdecl *FunctionType)(void*, void*);
		_retStorage = ((FunctionType)_fx)(_params[0], _params[1]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2, T3)
	inline void call() {
		typedef Ret(_cdecl *FunctionType)(void*, void*, void*);
		_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2, T3, T4)
	inline void call() {
		typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*);
		_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2, T3, T4, T5)
	inline void call() {
		typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*);
		_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4]);
	}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6]);
}
END_INVOKER1;

template <class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER1(CdeclInvoker, Ret, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef Ret(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	_retStorage = ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7]);
}
END_INVOKER1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <> class CdeclInvoker<void> {
public:
	void* _fx;
	CdeclInvoker(void* fx, void** data) : _fx(fx) { *data = nullptr; }
	inline void call() {
		typedef void(_cdecl *FunctionType)();
		((FunctionType)_fx)();
	}
	inline bool pushParam(void*) { return false; }
	inline void* popParam() { return nullptr; }
};

template <class T1>
BEGIN_INVOKER2(CdeclInvoker, T1)
	inline void call() {
		typedef void(_cdecl *FunctionType)(void*);
		((FunctionType)_fx)(_params[0]);
	}
};


template <class T1, class T2>
BEGIN_INVOKER2(CdeclInvoker, T1, T2)
	inline void call() {
		typedef void(_cdecl *FunctionType)(void*, void*);
		((FunctionType)_fx)(_params[0], _params[1]);
	}
};

template <class T1, class T2, class T3>
BEGIN_INVOKER2(CdeclInvoker, T1, T2, T3)
	inline void call() {
		typedef void(_cdecl *FunctionType)(void*, void*, void*);
		((FunctionType)_fx)(_params[0], _params[1], _params[2]);
	}
};

template <class T1, class T2, class T3, class T4>
BEGIN_INVOKER2(CdeclInvoker, T1, T2, T3, T4)
	inline void call() {
		typedef void(_cdecl *FunctionType)(void*, void*, void*, void*);
		 ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3]);
	}
};

template <class T1, class T2, class T3, class T4, class T5>
BEGIN_INVOKER2(CdeclInvoker, T1, T2, T3, T4, T5)
	inline void call() {
		typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*);
		 ((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4]);
	}
};

template <class T1, class T2, class T3, class T4, class T5, class T6>
BEGIN_INVOKER2(CdeclInvoker, T1, T2, T3, T4, T5, T6)
inline void call() {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5]);
}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
BEGIN_INVOKER2(CdeclInvoker, T1, T2, T3, T4, T5, T6, T7)
inline void call() {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6]);
}
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
BEGIN_INVOKER2(CdeclInvoker, T1, T2, T3, T4, T5, T6, T7, T8)
inline void call() {
	typedef void(_cdecl *FunctionType)(void*, void*, void*, void*, void*, void*, void*, void*);
	((FunctionType)_fx)(_params[0], _params[1], _params[2], _params[3], _params[4], _params[5], _params[6], _params[7]);
}
};

template <class Ret, class ...Args>
class CdeclFunction :
	public DFunction
{
public:
	typedef Ret(_cdecl *FuncType)(Args...);
protected:
	CdeclInvoker<Ret, Args...> invoker;
public:
	CdeclFunction(FuncType function) : CdeclFunction(*((void**)&function)) {}
	CdeclFunction(void* function) : invoker(function, &_ret) {}
	virtual ~CdeclFunction() {}

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
		CdeclFunction* newInstance = new CdeclFunction(invoker._fx);
		newInstance->invoker = invoker;
		return newInstance;
	}
};

template <class Ret, class ...Args>
inline Ret __cdecl cdeclForward(Ret __cdecl function(Args...),const Args&... args) {
	return function(args...);
}