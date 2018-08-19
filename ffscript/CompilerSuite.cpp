#include "stdafx.h"
#include "CompilerSuite.h"
#include "ExpresionParser.h"

namespace ffscript{
	CompilerSuite::CompilerSuite()
	{
		_pCompiler = (ScriptCompilerRef)(new ScriptCompiler());
	}

	CompilerSuite::~CompilerSuite()
	{
	}

	void CompilerSuite::initialize(int globalMemSize) {
		_globalScopeRef = (GlobalScopeRef)(new GlobalScope(globalMemSize, _pCompiler.get()));

		FunctionRegisterHelper funcLibHelper(_pCompiler.get());
		
		auto& typeManager = _pCompiler->getTypeManager();

		typeManager->registerBasicTypes(_pCompiler.get());
		typeManager->registerBasicTypeCastFunctions(_pCompiler.get(), funcLibHelper);
		typeManager->registerConstants(_pCompiler.get());

		importBasicfunction(funcLibHelper);

		_pCompiler->beginUserLib();
	}

	Program* CompilerSuite::compileProgram(const wchar_t* codeStart, const wchar_t* codeEnd) {
		_pCompiler->clearUserLib();

		Program* program = new Program();
		_pCompiler->bindProgram(program);

		if (_preprocessor) {
			auto newCode = _preprocessor->preprocess(codeStart, codeEnd);

			if (_globalScopeRef->parse(newCode->c_str(), newCode->c_str() + newCode->size()) == nullptr) {
				return nullptr;
			}
		}
		else if (_globalScopeRef->parse(codeStart, codeEnd) == nullptr) {
			return nullptr;
		}

		if (_globalScopeRef->correctAndOptimize(program) != 0) {
			return nullptr;
		}

		if (_globalScopeRef->extractCode(program) == false) {
			delete program;
			return nullptr;
		}

		return program;
	}

	ExpUnitExecutor* CompilerSuite::compileExpression(const wchar_t* expression) {
		ExpressionParser parser(_pCompiler.get());
		_pCompiler->pushScope(_globalScopeRef.get());

		_globalScopeRef->setBeginCompileChar(expression);

		list<ExpUnitRef> units;
		EExpressionResult eResult = parser.stringToExpList(expression, units);
		_globalScopeRef->setLastCompilerChar(parser.getLastCompileChar());

		if (eResult != E_SUCCESS) {
			return nullptr;
		}

		list<ExpressionRef> expList;
		bool res = parser.compile(units, expList);
		if (res == false) return nullptr;

		Expression* expressionPtr = expList.front().get();
		eResult = parser.link(expressionPtr);
		if (eResult != E_SUCCESS) return nullptr;

		//all variable in the scope will be place at right offset by bellow command
		//if this function is not execute before extract the code then all variable
		//will be placed at offset 0
		_globalScopeRef->updateVariableOffset();

		ExpUnitExecutor* pExcutor = new ExpUnitExecutor(_globalScopeRef.get());
		pExcutor->extractCode(_pCompiler.get(), expressionPtr);
		return pExcutor;
	}

	const GlobalScopeRef& CompilerSuite::getGlobalScope() const {
		return _globalScopeRef;
	}

	const TypeManagerRef& CompilerSuite::getTypeManager() const {
		return _pCompiler->getTypeManager();
	}

	ScriptCompilerRef& CompilerSuite::getCompiler() {
		return _pCompiler;
	}

	void CompilerSuite::setPreprocessor(const PreprocessorRef& preprocessor) {
		_preprocessor = preprocessor;
	}

	const PreprocessorRef CompilerSuite::getPreprocessor() const {
		return _preprocessor;
	}

	void CompilerSuite::getLastCompliedPosition(int& line, int& column) {
		if (_preprocessor && _globalScopeRef) {
			auto beginCompileChar = _globalScopeRef->getBeginCompileChar();
			auto lastCompileChar = _globalScopeRef->getLastCompileChar();

			_preprocessor->getOriginalPosition((int)(lastCompileChar - beginCompileChar), line, column);
		}
	}
}