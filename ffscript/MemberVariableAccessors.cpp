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