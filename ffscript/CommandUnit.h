#pragma once
#include <memory>
#include "ffscript.h"
#include "expresion_defs.h"
#include "MemoryBlock.h" 

namespace ffscript {

	class FFSCRIPT_API CommandUnit
	{
		MaskType _mask;
		MemoryBlockRef _userData;
	public:
		CommandUnit();
		virtual ~CommandUnit();

		virtual const std::string& toString()const = 0;
		virtual UNIT_TYPE getType()const = 0;
		const MemoryBlockRef& getUserData() const;
		void setUserData(const MemoryBlockRef&);
		virtual MaskType getMask()const;
		virtual void setMask(MaskType);
	};

	typedef std::shared_ptr<CommandUnit> CommandUnitRef;
}
