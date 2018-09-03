/******************************************************************
* File:        CommandUnitBuilder.cpp
* Description: implement command unit builder class. A base class of
*              all command builder that used to build the real
*              commands(compiled code) of the script.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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