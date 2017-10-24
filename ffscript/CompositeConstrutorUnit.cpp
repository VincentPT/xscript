#include "stdafx.h"
#include "CompositeConstrutorUnit.h"

namespace ffscript {
	CompositeConstrutorUnit::CompositeConstrutorUnit(std::vector<ScriptTypeRef> argumentTypes) :
		_argumentTypes(argumentTypes), Function("_create_object_by_ctor", EXP_UNIT_ID_CREATE_OBJECT_COMPOSITE, FUNCTION_PRIORITY_USER_FUNCTION, "TBD")
	{
	}


	CompositeConstrutorUnit::~CompositeConstrutorUnit()
	{
	}

	int CompositeConstrutorUnit::pushParam(ExecutableUnitRef pExeUnit) {
		if (!_constructorUnit) {
			_constructorUnit = pExeUnit;
			return 1;
		}
		if (pExeUnit->getType() != EXP_UNIT_ID_DYNAMIC_FUNC) {
			return -1;
		}

		auto collector = dynamic_cast<DynamicParamFunction*>(pExeUnit.get());
		auto& params = collector->getParams();

		if (params.size() != _argumentTypes.size()) {
			return -1;
		}
		if (params.size() != _castingList.size()) {
			return -1;
		}
		auto jt = _castingList.begin();
		for (auto it = params.begin(); it != params.end(); it++, jt++) {
			auto& castingInfo = *jt;
			auto& castingFunction = castingInfo.castingFunction;
			if (castingFunction) {
				castingFunction->pushParam(*it);
				_constructorParams.push_back(castingFunction);
			}
			else {
				_constructorParams.push_back(*it);
			}
		}

		return 0;
	}

	ExecutableUnitRef CompositeConstrutorUnit::popParam() {
		if (_constructorParams.size()) {
			auto param = _constructorParams.back();
			auto it = _constructorParams.end();
			it--;
			_constructorParams.erase(it);
			return param;
		}

		if (_constructorUnit) {
			auto tempUnit = _constructorUnit;
			_constructorUnit.reset();
			return tempUnit;
		}

		return ExecutableUnitRef();
	}

	const ExecutableUnitRef& CompositeConstrutorUnit::getChild(int index) const {
		if (index == 0) {
			return _constructorUnit;
		}

		index--;

		return _constructorParams[index];
	}

	ExecutableUnitRef& CompositeConstrutorUnit::getChild(int index) {
		if (index == 0) {
			return _constructorUnit;
		}

		index--;

		return _constructorParams[index];
	}

	int CompositeConstrutorUnit::getChildCount() {
		if (!_constructorUnit) return 0;
		return 1 + (int)_constructorParams.size();
	}
}