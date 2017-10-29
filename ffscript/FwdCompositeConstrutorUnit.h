#pragma once
#include "ExpresionParser.h"


namespace ffscript {
	class FwdCompositeConstrutorUnit :
		public Function
	{
		FunctionRef _constructorUnit;
		std::vector<ScriptTypeRef> _argumentTypes;
		ParamCastingList _castingList;
	public:
		FwdCompositeConstrutorUnit(const FunctionRef& constructorUnit, const std::vector<ScriptTypeRef>& argumentTypes, const ParamCastingList& castingList);
		virtual ~FwdCompositeConstrutorUnit();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
		virtual int getChildCount();
		FunctionRef& getConstructorUnit();
	};
}