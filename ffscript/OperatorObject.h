#pragma once
#include "MemoryBlock.h"

class DFunction2;

namespace ffscript {
	class OperatorObject
	{
	public:
		OperatorObject();
		virtual ~OperatorObject();
	};

	//////////////////////////////////////////////////////////////////
	class NativeOperatorObject : public OperatorObject {
		std::shared_ptr<DFunction2> _funcHolder;
	public:
		NativeOperatorObject(std::shared_ptr<DFunction2> function);
		virtual ~NativeOperatorObject();

		const std::shared_ptr<DFunction2>& getFunction() const;
	};

	//////////////////////////////////////////////////////////////////
	class ScriptOperatorObject : public OperatorObject {
	public:
		ScriptOperatorObject();
		virtual ~ScriptOperatorObject();
	};
}
