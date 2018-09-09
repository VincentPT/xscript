/******************************************************************
* File:        CLamdaProg.h
* Description: declare CLamdaProg class. A class used to store
*              compiled code and other stuffs that allow user
*              running the code with a global context.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <memory>
#include <GlobalScope.h>

namespace ffscript {

	class Program;
	class StaticContext;

	class CLamdaProg
	{
		friend class GlobalScope;

		struct StringCmp {
			bool operator()(const char* s1, const char* s2) const;
		};
	protected:
		Program* _program;
		std::shared_ptr<StaticContext> _context;

		int _globalConstructorCount;
		int _globalDataSize;
		int _globalCodeSize;
	protected:
		std::map<const char*, Variable*, StringCmp> _declaredVariableMap;
		std::list<std::shared_ptr<Variable>> _varibles;

		void addVariable(const std::shared_ptr<Variable>& variable);
	public:
		CLamdaProg(Program*);
		virtual ~CLamdaProg();

		Program* getProgram() const;
		const std::shared_ptr<StaticContext>& getGlobalContext() const;

		void setContext(const std::shared_ptr<StaticContext>&);
		void runGlobalCode();
		void cleanupGlobalMemory();
		const std::list<std::shared_ptr<Variable>>& getVariables() const;
		Variable* findDeclaredVariable(const char* variableName) const;
	};
}
