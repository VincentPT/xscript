/******************************************************************
* File:        MemberVariableAccessors.cpp
* Description: implement member variable accessor classes. Each member
*              variable accessor has its own data and behavior to
*              access its data base on parent's address.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
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