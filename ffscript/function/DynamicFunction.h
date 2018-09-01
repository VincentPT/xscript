/******************************************************************
* File:        DynamicFunction.h
* Description: declare abstract class of function object that invoke
*              a target function. This function object design to store
*              all necessary arguments of its target function when
*              it is created. The caller just to call 'call' only
*              to invoke the target function.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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

