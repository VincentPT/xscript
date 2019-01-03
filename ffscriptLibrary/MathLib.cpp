/******************************************************************
* File:        MathLib.cpp
* Description: implement an interface to import math functions into the
*              script compiler.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "MathLib.h"

#include "ScriptCompiler.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunctionFactory.hpp"
#include "DynamicFunctionFactory.h"
#include "BasicType.h"
#include "BasicFunction.h"

#include <math.h>

namespace ffscript {
#define REGIST_MATH_FUNCTION1(helper, nativeFunc, scriptFunc, returnType, ...) \
	helper.registFunction(\
		scriptFunc, #__VA_ARGS__,\
		createUserFunctionFactory<returnType,__VA_ARGS__>(helper.getSriptCompiler(), #returnType, nativeFunc)\
	)

#define REGIST_MATH_FUNCTION2(helper, func, returnType, ...) REGIST_MATH_FUNCTION1(helper, func, #func, returnType, __VA_ARGS__)

	void includeMathToCompiler(ScriptCompiler* scriptCompiler) {
		FunctionRegisterHelper helper(scriptCompiler);

		auto& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();

		// Trigonometric functions
		REGIST_MATH_FUNCTION2(helper, sin, double, double);
		REGIST_MATH_FUNCTION2(helper, sin, float, float);
		REGIST_MATH_FUNCTION2(helper, cos, double, double);
		REGIST_MATH_FUNCTION2(helper, cos, float, float);
		REGIST_MATH_FUNCTION2(helper, tan, float, float);
		REGIST_MATH_FUNCTION2(helper, tan, double, double);

		REGIST_MATH_FUNCTION2(helper, asin, double, double);
		REGIST_MATH_FUNCTION2(helper, asin, float, float);
		REGIST_MATH_FUNCTION2(helper, acos, double, double);
		REGIST_MATH_FUNCTION2(helper, acos, float, float);
		REGIST_MATH_FUNCTION2(helper, atan, float, float);
		REGIST_MATH_FUNCTION2(helper, atan, double, double);

		REGIST_MATH_FUNCTION2(helper, atan2, double, double, double);
		REGIST_MATH_FUNCTION2(helper, atan2, float, float, float);

		// Hyperbolic functions
		REGIST_MATH_FUNCTION2(helper, sinh, double, double);
		REGIST_MATH_FUNCTION2(helper, sinh, float, float);
		REGIST_MATH_FUNCTION2(helper, cosh, double, double);
		REGIST_MATH_FUNCTION2(helper, cosh, float, float);
		REGIST_MATH_FUNCTION2(helper, tanh, double, double);
		REGIST_MATH_FUNCTION2(helper, tanh, float, float);

		REGIST_MATH_FUNCTION2(helper, asinh, double, double);
		REGIST_MATH_FUNCTION2(helper, asinh, float, float);
		REGIST_MATH_FUNCTION2(helper, acosh, double, double);
		REGIST_MATH_FUNCTION2(helper, acosh, float, float);
		REGIST_MATH_FUNCTION2(helper, atanh, float, float);
		REGIST_MATH_FUNCTION2(helper, atanh, double, double);

		//Exponential and logarithmic functions
		REGIST_MATH_FUNCTION2(helper, exp, double, double);
		REGIST_MATH_FUNCTION2(helper, exp, float, float);
		REGIST_MATH_FUNCTION2(helper, log, double, double);
		REGIST_MATH_FUNCTION2(helper, log, float, float);
		REGIST_MATH_FUNCTION2(helper, log10, double, double);
		REGIST_MATH_FUNCTION2(helper, log10, float, float);
		REGIST_MATH_FUNCTION2(helper, exp2, double, double);
		REGIST_MATH_FUNCTION2(helper, exp2, float, float);
		REGIST_MATH_FUNCTION2(helper, log2, double, double);
		REGIST_MATH_FUNCTION2(helper, log2, float, float);
		REGIST_MATH_FUNCTION2(helper, logb, double, double);
		REGIST_MATH_FUNCTION2(helper, logb, float, float);

		//Power functions
		REGIST_MATH_FUNCTION2(helper, pow, double, double, double);
		REGIST_MATH_FUNCTION2(helper, pow, float, float, float);
		REGIST_MATH_FUNCTION2(helper, sqrt, double, double);
		REGIST_MATH_FUNCTION2(helper, sqrt, float, float);
		REGIST_MATH_FUNCTION2(helper, cbrt, double, double);
		REGIST_MATH_FUNCTION2(helper, cbrt, float, float);
		REGIST_MATH_FUNCTION2(helper, hypot, double, double, double);
		REGIST_MATH_FUNCTION2(helper, hypot, float, float, float);

		//Rounding and remainder functions
		REGIST_MATH_FUNCTION2(helper, ceil, double, double);
		REGIST_MATH_FUNCTION2(helper, ceil, float, float);
		REGIST_MATH_FUNCTION2(helper, floor, double, double);
		REGIST_MATH_FUNCTION2(helper, floor, float, float);
		REGIST_MATH_FUNCTION2(helper, round, double, double);
		REGIST_MATH_FUNCTION2(helper, round, float, float);

		//Other functions
		REGIST_MATH_FUNCTION2(helper, abs, double, double);
		REGIST_MATH_FUNCTION2(helper, abs, float, float);
		REGIST_MATH_FUNCTION2(helper, abs, int, int);
		helper.registFunction("abs", "long",
			createUserFunctionFactory<long long, long long>(helper.getSriptCompiler(), "long", abs));
	}
}