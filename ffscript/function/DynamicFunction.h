#pragma once

class DFunction
{
protected:
	int mParamCount;
	void* _ret;
public:
	DFunction();
	virtual ~DFunction();
	virtual void call() = 0;	
	virtual bool pushParam(void* param) = 0;
	virtual void* popParam() = 0;
	virtual DFunction* clone() = 0;
	
	void* getReturnValAsVoidPointer() const;
	__int64 getReturnValAsInt64() const;
	__int32 getReturnValAsInt32() const;
	__int16 getReturnValAsInt16() const;
	__int8 getReturnValAsInt8() const;
	float getReturnValAsFloat() const;
	double getReturnValAsDouble() const;
};

