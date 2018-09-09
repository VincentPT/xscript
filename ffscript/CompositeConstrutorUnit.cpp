/******************************************************************
* File:        CompositeConstrutorUnit.cpp
* Description: implement CompositeConstrutorUnit class. A command
*              builder for expression unit. It is specialized for
*              composite constructor unit.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CompositeConstrutorUnit.h"

namespace ffscript {

	CompositeConstrutorUnit::CompositeConstrutorUnit() :
		Function("__ctor_composite", EXP_UNIT_ID_CONSTRUCTOR_COMPOSITE, FUNCTION_PRIORITY_USER_FUNCTION, "void") {
	}

	CompositeConstrutorUnit::CompositeConstrutorUnit(const list<pair<Variable*, ExecutableUnitRef>>& assigments) : CompositeConstrutorUnit()
	{
		_assigments = assigments;
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

	void CompositeConstrutorUnit::setAssigments(list<pair<Variable*, ExecutableUnitRef>>& assigments) {
		_assigments.clear();
		_assigments.splice(_assigments.end(), assigments);
	}

	list<pair<Variable*, ExecutableUnitRef>>& CompositeConstrutorUnit::getAssigments() {
		return _assigments;
	}
}