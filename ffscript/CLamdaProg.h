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
