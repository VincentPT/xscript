/******************************************************************
* File:        StaticContext.h
* Description: declare StaticContext class. A type of context, but
*              the memory of its is global shared memory and can be
*              accessed any where, any time in the program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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