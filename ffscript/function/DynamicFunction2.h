#pragma once

//the library won't offer bind function, but it reduce calling cost
#define USE_EXTERNAL_PARAMS_ONLY

class DFunction2
{
protected:
	void* mFx;
#ifndef USE_EXTERNAL_PARAMS_ONLY
	int mFixedParamCount = 0;
#endif
public:
	DFunction2();
	virtual ~DFunction2();
	virtual void call(void* pReturnVal, void* param[]) = 0;
	virtual DFunction2* clone() = 0;
};

