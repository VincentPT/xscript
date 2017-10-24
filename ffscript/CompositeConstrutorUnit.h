#pragma once
#include "ExpresionParser.h"


namespace ffscript {
	class CompositeConstrutorUnit :
		public Function
	{
		ExecutableUnitRef _constructorUnit;
		std::vector<ExecutableUnitRef> _constructorParams;
		std::vector<ScriptTypeRef> _argumentTypes;
		ParamCastingList _castingList;
	public:
		CompositeConstrutorUnit(std::vector<ScriptTypeRef> argumentTypes);
		virtual ~CompositeConstrutorUnit();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
		virtual int getChildCount();
	};
}