#pragma once
#include "Context.h"
#include <list>
#include <memory>

namespace ffscript {
	class StaticContext :
		public Context
	{
	protected:
		std::list<CommandPointer> _globalCommands;
		std::list<CommandPointer> _destructorCommands;

		void runCommands(const std::list<CommandPointer>& commands);

	public:
		StaticContext(unsigned char* threadData, int bufferSize);
		StaticContext(int bufferSize);
		virtual ~StaticContext();
		void addCommand(CommandPointer command);
		void addDestructorCommand(CommandPointer command);
		virtual void run();
		virtual void runDestructorCommands();
	};

	typedef std::shared_ptr<StaticContext> StaticContextRef;
}