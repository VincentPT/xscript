/******************************************************************
* File:        MemberVariableAccessors.cpp
* Description: implement member variable accessor classes. Each member
*              variable accessor has its own data and behavior to
*              access its data base on parent's address.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "MemberVariableAccessors.h"
#include "Context.h"

namespace ffscript {
	MemberVariableAccessor::MemberVariableAccessor() {}
	MemberVariableAccessor::~MemberVariableAccessor() {}

	/////////////////////////////////////////////////////////////////////////////////////////
	MVOffsetAccessor::MVOffsetAccessor(int offset) : _offset(offset) {}

	void* MVOffsetAccessor::access(void* address) {
		return ((char*)address) + _offset;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	void* MVPointerAccessor::access(void* address) {
		return (void*)*((size_t*)address);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	void* MVContextAccessor::access(void*) {
		Context* context = Context::getCurrent();
		return context->getAbsoluteAddress(context->getCurrentOffset());
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	MVGlobalAccessor::MVGlobalAccessor(void* address) : _address(address) {}
	void* MVGlobalAccessor::access(void*) {
		return _address;
	}
}