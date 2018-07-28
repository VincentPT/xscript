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