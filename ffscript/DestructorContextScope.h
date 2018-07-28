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