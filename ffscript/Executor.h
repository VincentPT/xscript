#pragma once
#include <list>
#include <memory>
#include "expressionunit.h"
#include "function\DynamicFunction.h"
#include "Context.h"
#include "MemoryBlock.h"

namespace ffscript {
	class Executor	{
	
	protected:
	
		typedef shared_ptr<InstructionCommand> CommandRef;
		std::list<CommandRef> _commandContainer;
		std::list<MemoryBlockRef> _memoryBlocks;
		CommandList _commandList;
	public:
		Executor();
		virtual ~Executor();

		virtual CommandList* getCode();
		void addCommand(InstructionCommand*);
		void runCode();
	};

	typedef std::shared_ptr<Executor> ExecutorRef;
}

