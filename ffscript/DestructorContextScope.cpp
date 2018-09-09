/******************************************************************
* File:        DestructorContextScope.cpp
* Description: implement DestructorContextScope class. A destructor
*              context scope is a special of context scope that store
*              commands generated to destruct an object in preivous
*              expression.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "DestructorContextScope.h"

namespace ffscript {
	DestructorContextScope::DestructorContextScope(ScriptScope* parent, FunctionScope* functionScope) :
		ContextScope(parent, functionScope),
		_destructorUnit(nullptr)
	{
	}


	DestructorContextScope::~DestructorContextScope() {
	}

	void DestructorContextScope::setDestructorParentUnit(ExecutableUnit* parentUnit) {
		_destructorUnit = parentUnit;
	}

	ExecutableUnit* DestructorContextScope::getDestructorParentUnit() const {
		return _destructorUnit;
	}
}