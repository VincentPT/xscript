#include "stdafx.h"
#include "FwdCompositeConstrutorUnit.h"

namespace ffscript {
	FwdCompositeConstrutorUnit::FwdCompositeConstrutorUnit(const FunctionRef& constructorUnit, const std::vector<ScriptTypeRef>& argumentTypes, const ParamCastingList& castingList) :
		_constructorUnit(constructorUnit),
		_argumentTypes(argumentTypes),
		_castingList(castingList),
		Function("_create_object_by_ctor", EXP_UNIT_ID_CREATE_OBJECT_COMPOSITE, FUNCTION_PRIORITY_USER_FUNCTION, "TBD")
	{		
	}


	FwdCompositeConstrutorUnit::~FwdCompositeConstrutorUnit()
	{
	}

	int FwdCompositeConstrutorUnit::pushParam(ExecutableUnitRef pExeUnit) {		
		if (pExeUnit->getType() != EXP_UNIT_ID_DYNAMIC_FUNC) {
			// throw exception here
		}

		auto collector = dynamic_cast<DynamicParamFunction*>(pExeUnit.get());
		auto& params = collector->getParams();

		if ((params.size() + 1) != _argumentTypes.size()) {
			// throw exception here
		}
		if ((params.size() + 1) != _castingList.size()) {
			// throw exception here
		}
		auto jt = _castingList.begin();
		auto ait = _argumentTypes.begin();

		// ignore first argument because it was already supplied inside constructorUnit
		jt++;
		ait++;
		for (auto it = params.begin(); it != params.end(); it++, jt++, ait++) {
			auto& castingInfo = *jt;
			auto& castingFunction = castingInfo.castingFunction;
			if (castingFunction) {
				// set parameter for casting function
				castingFunction->pushParam(*it);
				// set return type for casting function
				castingFunction->setReturnType(*(ait->get()));
				_constructorUnit->pushParam(castingFunction);
			}
			else {
				_constructorUnit->pushParam(*it);
			}
		}

		return 0;
	}

	ExecutableUnitRef FwdCompositeConstrutorUnit::popParam() {
		return _constructorUnit->popParam();
	}

	const ExecutableUnitRef& FwdCompositeConstrutorUnit::getChild(int index) const {
		return _constructorUnit->getChild(index);
	}

	ExecutableUnitRef& FwdCompositeConstrutorUnit::getChild(int index) {
		return _constructorUnit->getChild(index);
	}

	int FwdCompositeConstrutorUnit::getChildCount() {
		return _constructorUnit->getChildCount();
	}

	FunctionRef& FwdCompositeConstrutorUnit::getConstructorUnit() {
		return _constructorUnit;
	}
}