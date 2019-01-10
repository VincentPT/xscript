/******************************************************************
* File:        MethodContainerUT.cpp
* Description: Test cases invoking member functions using MFunction.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"
#include "../ffscript/ffscript.h"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "MethodsForTest.h"

using namespace std;
using namespace ffscript;

namespace ffscriptUT
{
	FF_TEST_METHOD(MethodContainer, methodArgumentBufferTestMethodContainerVoid1)
	{
		MFunctionT<MethodContainer, void> memberFunction(this, &MethodContainer::memberFoo);
		memberFunction.call(nullptr, nullptr);
	}
}
