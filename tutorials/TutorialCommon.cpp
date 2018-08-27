// CoActionRecursive.cpp : Defines the entry point for the console application.
//

#include "TutorialCommon.h"
#include <chrono>

int scriptMainFunctionId = -1;

CLamdaProg* complieProgram(FImportLibrary importLib, const char* file, int globalStackSize) {
	CompilerSuite compiler;

	// initialize compiler, import system library and set the stack size of global context
	compiler.initialize(globalStackSize);
	auto& rootScope = compiler.getGlobalScope();
	auto scriptCompiler = rootScope->getCompiler();

	// import custom libraries
	// String library
	includeRawStringToCompiler(scriptCompiler);

	// import application specific library
	importLib(scriptCompiler);
	
	// tell compiler that from now, any the function that will be registered
	// is a user function, that mean it will be clean each time compileProgram is executed
	scriptCompiler->beginUserLib();

	auto script = readCodeFromUtf8File(file);
	compiler.setPreprocessor(std::make_shared<DefaultPreprocessor>());
	auto program = compiler.compileProgram(script.c_str(), script.c_str() + script.size());

	if (program == nullptr) {
		int line, column;
		compiler.getLastCompliedPosition(line, column);

		string errorMsg("error at line = ");
		errorMsg.append(std::to_string(line + 1));
		errorMsg.append(", column = ");
		errorMsg.append(std::to_string(column + 1));
		errorMsg.append("\n");
		errorMsg.append(scriptCompiler->getLastError());

		cout << "Compile program " << file << " error" << endl;
		cout << errorMsg << endl;
	}
	else {
		scriptMainFunctionId = scriptCompiler->findFunction("main", "");
		if (scriptMainFunctionId < 0) {
			cout << "Warning!!! Main function of script is not implemented" << endl;
		}
		else {
			cout << "+----------------------------------------------------------+" << endl;
			cout << "|            Compile program successfully!                 |" << endl;
			cout << "+----------------------------------------------------------+" << endl << endl;
		}
		return rootScope->detachScriptProgram(program);
	}
	return nullptr;
}

void* runProgram(CLamdaProg* scriptProgram) {
	if (scriptMainFunctionId < 0) return nullptr;
	void* returnFromMain = nullptr;

	using namespace std::chrono;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	try {
		ScriptRunner scriptRunner(scriptProgram->getProgram(), scriptMainFunctionId);
		scriptRunner.runFunction(nullptr);

		returnFromMain = scriptRunner.getTaskResult();
	}
	catch (std::exception& e) {
		cout << "An exception has been occured while running the script: " << e.what() << endl;
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	cout << endl;
	cout << "+----------------------------------------------------------+" << endl;
	cout << "|              time consume: " << time_span.count() <<"s                   |" << endl;
	cout << "+----------------------------------------------------------+" << endl;

	return returnFromMain;
}