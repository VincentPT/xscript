#pragma once
#include "Context.h"
#include <list>
#include <memory>

namespace ffscript {
	class StaticContext :
		public Context
	{
	protected:
		std::list<CommandPointer> _globalComands;
	public:
		StaticContext(unsigned char* threadData, int bufferSize);
		StaticContext(int bufferSize);
		virtual ~StaticContext();
		void addCommand(CommandPointer command);
		virtual void run();
	};

	typedef std::shared_ptr<StaticContext> StaticContextRef;
}