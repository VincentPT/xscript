#include "stdafx.h"
#include "CompositeConstrutorUnit.h"

namespace ffscript {
	CompositeConstrutorUnit::CompositeConstrutorUnit(const list<pair<Variable*, ExecutableUnitRef>>& assigments) :
		Function("__ctor_composite", EXP_UNIT_ID_CONSTRUCTOR_COMPOSITE, FUNCTION_PRIORITY_USER_FUNCTION, "void")
	{
		_assigments.insert(assigments.end(), assigments.begin(), assigments.end());
	}


	CompositeConstrutorUnit::~CompositeConstrutorUnit()
	{
	}

	int CompositeConstrutorUnit::pushParam(ExecutableUnitRef pExeUnit) {
		// not support push param method
		// throw exception here
		return -1;
	}

	ExecutableUnitRef CompositeConstrutorUnit::popParam() {
		ExecutableUnitRef unitRef = _assigments.back().second;
		_assigments.pop_back();
		return unitRef;
	}

	const ExecutableUnitRef& CompositeConstrutorUnit::getChild(int index) const {
		auto param = _assigments.begin();
		auto end = _assigments.end();
		for (int i = 0; param != end; i++, param++) {
			if (i == index) {
				return param->second;
			}
		}

		return _assigments.back().second;
	}

	ExecutableUnitRef& CompositeConstrutorUnit::getChild(int index) {
		auto param = _assigments.begin();
		auto end = _assigments.end();
		for (int i = 0; param != end; i++, param++) {
			if (i == index) {
				return param->second;
			}
		}

		return _assigments.back().second;
	}

	int CompositeConstrutorUnit::getChildCount() {
		return (int)_assigments.size();
	}

	list<pair<Variable*, ExecutableUnitRef>>& CompositeConstrutorUnit::getAssigments() {
		return _assigments;
	}
}