/******************************************************************
* File:        DestructorContextScope.h
* Description: declare DestructorContextScope class. A destructor
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

#pragma once
#include "ContextScope.h"
#include "Program.h"
#include <string>

namespace ffscript {
	class ExecutableUnit;
	class DestructorContextScope :
		public ContextScope
	{
		ExecutableUnit* _destructorUnit;
	public:
		DestructorContextScope(ScriptScope* parent, FunctionScope* functionScope);
		virtual ~DestructorContextScope();

		void setDestructorParentUnit(ExecutableUnit* parentUnit);
		ExecutableUnit* getDestructorParentUnit() const;
	};
}