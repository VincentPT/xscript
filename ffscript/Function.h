#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include "expressionunit.h"
#include "IStack.h"

class EXPRESSIONCORE_API COneParamFunc : public CFunc
{
private:
protected:
	CExecutableUnit* m_pParamFunc;
	COneParamFunc(int iPriority, unsigned int returnType);
public:
	virtual ~COneParamFunc();
	virtual CExecutableUnit* PopParam();
	virtual CExecutableUnit* GetFirstParam();
	virtual CExecutableUnit* GetNextParam(CExecutableUnit* param);
};

class EXPRESSIONCORE_API CTwoParamFunc : public CFunc
{
private:
protected:
	CExecutableUnit* m_pFirstParamFunc;
	CExecutableUnit* m_pSecondParamFunc;
	CTwoParamFunc(int iPriority, unsigned int returnType);
public:
	virtual ~CTwoParamFunc();
	virtual CExecutableUnit* PopParam();
	virtual CExecutableUnit* GetFirstParam();
	virtual CExecutableUnit* GetNextParam(CExecutableUnit* param);
};

class EXPRESSIONCORE_API cDynamicParamFunction : public CFunc
{
private:
protected:
	cDynamicParamFunction(int iPriority, unsigned int returnType);
	IStack* m_pParams;
public:
	virtual ~cDynamicParamFunction();
	virtual int PushParam(CExecutableUnit** ppExeUnit);
	virtual CExecutableUnit* PopParam();
	virtual CExecutableUnit* GetFirstParam();
	virtual CExecutableUnit* GetNextParam(CExecutableUnit* param);
	virtual int ValidateParams() = 0;
	virtual bool PushParam(CExecutableUnit* ppExeUnit) = 0;
};

class EXPRESSIONCORE_API cMultiParamFunction : public cDynamicParamFunction
{
private:
protected:
	cMultiParamFunction(int iPriority, unsigned int returnType, int fixedParamCount);
	int m_iFixedParamCount;
public:
	virtual ~cMultiParamFunction();
	//virtual int PushParam(CExecutableUnit** ppExeUnit);
	virtual int ValidateParams();
};

#define IMPLEMENT_ONEPARAM_FUNC_CLASS(className,funcName, funcID,priority,paramTypes,returnType) \
	className::className():COneParamFunc(priority,returnType){\
}\
	className::~className(){\
}\
	UNIT_TYPE className::GetType()const{\
	return funcID;\
}\
	const TCHAR* className::GetFuncName(){\
	return funcName;\
}\
	const TCHAR* className::GetName()const{\
	return  className::GetFuncName();\
}\
	int className::PushParam(CExecutableUnit** ppExeUnit){\
	if( paramTypes & (*ppExeUnit)->GetReturnType()) {\
		if(m_pParamFunc == NULL){\
			m_pParamFunc= *ppExeUnit;\
			return 0;\
		}\
		else{\
			DBG_ERROR(_tprintf(__T("\n[#]funtion does not need more param")));\
		}\
	}\
	else{\
		DBG_ERROR(_tprintf(__T("\n[#]param Type Unxpected:%d"),(*ppExeUnit)->GetReturnType()));\
	}\
	return -1;\
}

#define IMPLEMENT_TWOPARAM_FUNC_CLASS(className,funcName,funcID,priority,param1Types,param2Types,returnType) \
	className::className():CTwoParamFunc(priority,returnType){\
}\
	className::~className(){\
}\
	UNIT_TYPE className::GetType()const{\
	return funcID;\
}\
	const TCHAR* className::GetFuncName(){\
	return funcName;\
}\
	const TCHAR* className::GetName()const{\
	return  className::GetFuncName();\
}\
	int className::PushParam(CExecutableUnit** ppExeUnit){\
	if(m_pSecondParamFunc == NULL && ( (*ppExeUnit)->GetReturnType() & param2Types)){\
	m_pSecondParamFunc = *ppExeUnit;\
	return 1;\
	}\
	if(m_pFirstParamFunc == NULL && ( (*ppExeUnit)->GetReturnType() & param1Types)){\
	m_pFirstParamFunc = *ppExeUnit;\
	return 0;\
	}\
	return -1;\
}

#define IMPLEMENT_MULTIPARAM_FUNC_CLASS(className,funcName,funcID,priority, returnType) \
	className::className():cDynamicParamFunction(priority,returnType){\
}\
	className::~className(){\
}\
	UNIT_TYPE className::GetType()const{\
	return funcID;\
}\
	const TCHAR* className::GetFuncName(){\
	return funcName;\
}\
	const TCHAR* className::GetName()const{\
	return  className::GetFuncName();\
}
#endif //_FUNCTION_H_