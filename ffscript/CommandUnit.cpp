#include "stdafx.h"
#include "CommandUnit.h"

namespace ffscript {
	CommandUnit::CommandUnit() : _mask(UMASK_NONE)
	{
	}

	CommandUnit::~CommandUnit()
	{
	}

	const MemoryBlockRef& CommandUnit::getUserData() const {
		return _userData;
	}

	void CommandUnit::setUserData(const MemoryBlockRef& userData) {
		_userData = userData;
	}

	MaskType CommandUnit::getMask()const {
		return _mask;
	}

	void CommandUnit::setMask(MaskType mask) {
		_mask = mask;
	}

}