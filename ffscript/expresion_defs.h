/******************************************************************
* File:        expresion_defs.h
* Description: declare macro and constants used for compiling 
*              expression.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
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
	E_SUCCESS = 0,
	E_TOKEN_INVALID,
	E_TOKEN_OUTOFLENGTH,
	E_TOKEN_UNEXPECTED,
	E_INCOMPLETED_EXPRESSION,	
	E_TOKEN_TYPE_UNEXPECTED,
	E_TYPE_CONVERSION_ERROR,
	E_FUNCTION_NOT_FOUND,
	E_TYPE_AMBIOUS_CALL,
	E_TYPE_UNKNOWN,
	E_FAIL,
	E_TYPE_END,
} EExpressionResult;

#endif //_EXPRESSION_DEFS_