/******************************************************************
* File:        expresion_defs.h
* Description: declare macro and constants used for compiling 
*              expression.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#ifndef _EXPRESSION_DEFS_
#define _EXPRESSION_DEFS_

typedef unsigned int UNIT_TYPE;

#define ISOPERATOR(pObject) (pObject->getType() & EXP_UNIT_GROUP_OPERATOR)
#define ISFUNCTION(pObject) (pObject->getType() & EXP_UNIT_GROUP_FUNCTION)
#define ISOPERAND(pObject) (pObject->getType() & EXP_UNIT_GROUP_OPERAND)
#define ISUSERFUNC(pObject) (pObject->getType() & EXP_UNIT_GROUP_USERFUNC)

#ifndef MAX
#define MAX(a,b) (a > b ? a : b)
#endif

typedef enum tagEExpressionResult
{
	EE_SUCCESS = 0,
	EE_TOKEN_INVALID,
	EE_TOKEN_OUTOFLENGTH,
	EE_TOKEN_UNEXPECTED,
	EE_INCOMPLETED_EXPRESSION,	
	EE_TOKEN_TYPE_UNEXPECTED,
	EE_TYPE_CONVERSION_ERROR,
	EE_FUNCTION_NOT_FOUND,
	EE_TYPE_AMBIOUS_CALL,
	EE_TYPE_UNKNOWN,
	EE_FAIL,
	EE_TYPE_END,
} EExpressionResult;

#endif //_EXPRESSION_DEFS_