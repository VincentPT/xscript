/******************************************************************
* File:        ContructorsDestructor.cpp
* Description: Contains code for compiling, running C Lambda
*              scripting language. This application demonstrate
*              registering a constructors and destructors in C++
*              side and using them in script side.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "../TutorialCommon.h"

class ConsoleCompilationLogger : public CompilationLogger {
	void log(MessageType type, const wchar_t* message) {
		if (type == MessageType::MESSAGE_ERROR) {
			wcout << L"[Error] " << message << endl;
		}
		else if (type == MessageType::MESSAGE_WARNING) {
			wcout << L"[Warning] " << message << endl;
		}
		else if (type == MessageType::MESSAGE_INFO) {
			wcout << L"[Info] " << message << endl;
		}
		else {
			wcout << message << endl;
		}
	}
};

ConsoleCompilationLogger compilationLogger;

struct CustomArray {
	int n;
	float* data;
};

// println functions
void println(const std::string& s) {
	cout << s << endl;
}

void print(const std::string& s) {
	cout << s;
}

void print(const CustomArray& arr) {
	if (arr.n == 0) {
		cout << "{}";
		return;
	}
	cout << "{";
	float* p = arr.data;
	float* pEnd = p + arr.n - 1;
	while(p < pEnd) {
		cout << *p << ",";
		p++;
	}
	cout << *p << "}";
}

// default constructor
void customArrayCtor(CustomArray* obj) {
	obj->n = 0;
	obj->data = nullptr;

	cout << "default constructor is executed" << endl;
}

// copy constructor
void customArrayCtor(CustomArray* objA, const CustomArray& objB) {
	if (objB.data == nullptr) {
		customArrayCtor(objA);
	}
	else {
		objA->n = objB.n;
		auto size = objB.n * sizeof(float);
		objA->data = (float*)malloc(size);
		memcpy(objA->data, objB.data, size);
	}

	cout << "copy constructor is executed" << endl;
}

// destructor
void customArrayDtor(CustomArray* obj) {
	if (obj->data) {
		free(obj->data);
	}
	cout << "destructor is executed" << endl;
}

// add member function
void add(CustomArray& obj, float val) {
	obj.n++;
	auto size = obj.n * sizeof(float);
	float* newData = (float*) malloc(size);
	memcpy(newData, obj.data, size - sizeof(float));

	*(newData + obj.n - 1) = val;

	if (obj.data) free(obj.data);
	obj.data = newData;
}

void importApplicationLibrary(ScriptCompiler* scriptCompiler) {
	FunctionRegisterHelper fb(scriptCompiler);
	scriptCompiler->setLogger(&compilationLogger);

	// register a new type
	int typeId = scriptCompiler->registType("CustomArray");
	if (IS_UNKNOWN_TYPE(typeId)) {
		return;
	}
	scriptCompiler->setTypeSize(typeId, sizeof(CustomArray));

	// register print functions
	registerFunction
		<void, const std::string&> // native function prototype
		(fb, // register helper object
			println, // native function
			"println", //script function name
			"void", // return type of the script function
			"string&" // parameter type of the function
			);

	registerFunction
		<void, const std::string&> // native function prototype
		(fb, // register helper object
			print, // native function
			"print", //script function name
			"void", // return type of the script function
			"string&" // parameter type of the function
			);

	registerFunction
		<void, const CustomArray&> // native function prototype
		(fb, // register helper object
			print, // native function
			"print", //script function name
			"void", // return type of the script function
			"CustomArray&" // parameter type of the function
			);

	// register add member function
	registerFunction
		<void, CustomArray&, float> // native function prototype
		(fb, // register helper object
			add, // native function
			"add", //script function name
			"void", // return type of the script function
			"CustomArray&, float" // parameter type of the function
			);
	
	// register constructors and destructor
	// default constructor
	int defaultContructorId = registerFunction
		<void, CustomArray*> // native function prototype
		(fb, // register helper object
			customArrayCtor, // native function
			"customArrayCtor", //script function name
			"void", // return type of the script function
			"ref CustomArray" // parameter type of the function
			);
	if (defaultContructorId < 0) {
		return;
	}

	// copy constructor
	int copyContructorId = registerFunction
		<void, CustomArray*, const CustomArray&> // native function prototype
		(fb, // register helper object
			customArrayCtor, // native function
			"customArrayCtor", //script function name
			"void", // return type of the script function
			"ref CustomArray, CustomArray&" // parameter type of the function
			);
	if (copyContructorId < 0) {
		return;
	}

	// destructor
	int destructorId = registerFunction
		<void, CustomArray*> // native function prototype
		(fb, // register helper object
			customArrayDtor, // native function
			"customArrayDtor", //script function name
			"void", // return type of the script function
			"ref CustomArray" // parameter type of the function
			);
	if (destructorId < 0) {
		return;
	}

	scriptCompiler->registConstructor(typeId, defaultContructorId);
	scriptCompiler->registConstructor(typeId, copyContructorId);
	scriptCompiler->registDestructor(typeId, destructorId);
}

int main(int argc, char* argv[])
{
	auto program = compileProgram(importApplicationLibrary, "ContructorsDestructor.c955");
	if (program) {
		// run the code that place in global scope
		program->runGlobalCode();

		// run main function of the script
		runProgram(program);

		// clean up global memory generated by runGlobalCode method
		program->cleanupGlobalMemory();

		delete program;
	}

	return 0;
}

