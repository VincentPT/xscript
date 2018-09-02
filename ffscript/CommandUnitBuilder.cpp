#include "stdafx.h"
#include "CommandUnitBuilder.h"

namespace ffscript {
	CommandUnitBuilder::CommandUnitBuilder() : _mask(UMASK_NONE)
	{
	}

	CommandUnitBuilder::~CommandUnitBuilder()
	{
	}

	const MemoryBlockRef& CommandUnitBuilder::getUserData() const {
		return _userData;
	}

	void CommandUnitBuilder::setUserData(const MemoryBlockRef& userData) {
		_userData = userData;
	}

	MaskType CommandUnitBuilder::getMask()const {
		return _mask;
	}

	void CommandUnitBuilder::setMask(MaskType mask) {
		_mask = mask;
	}

}