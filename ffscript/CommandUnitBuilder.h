/******************************************************************
* File:        CommandUnitBuilder.h
* Description: declare command unit builder class. A base class of
*              all command builder that used to build the real
*              commands(compiled code) of the script.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <memory>
#include <string>
#include "ffscript.h"
#include "expresion_defs.h"
#include "MemoryBlock.h" 

namespace ffscript {

	class FFSCRIPT_API CommandUnitBuilder
	{
		MaskType _mask;
		MemoryBlockRef _userData;
	public:
		CommandUnitBuilder();
		virtual ~CommandUnitBuilder();

		virtual const std::string& toString()const = 0;
		virtual UNIT_TYPE getType()const = 0;
		const MemoryBlockRef& getUserData() const;
		void setUserData(const MemoryBlockRef&);
		virtual MaskType getMask()const;
		virtual void setMask(MaskType);
	};

	typedef std::shared_ptr<CommandUnitBuilder> CommandUnitRef;
}
