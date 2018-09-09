/******************************************************************
* File:        ContructorsDestructor.cpp
* Description: Contains code for compiling, running C Lambda
*              scripting language. This application demonstrate
*              registering a constructors and destructors in C++
*              side and using them in script side.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "../TutorialCommon.h"
#include <function/CdeclFunction.hpp>

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

ostream& operator<<(ostream& out, const CustomArray& arr) {
	if (arr.n == 0) {
		out << "{}";
		return out;
	}
	out << "{";
	float* p = arr.data;
	float* pEnd = p + arr.n - 1;
	while(p < pEnd) {
		out << *p << ",";
		p++;
	}
	out << *p << "}";

	return out;
}

// default constructor
void __customArrayConstructor(CustomArray* obj) {
	obj->n = 0;
	obj->data = nullptr;

	cout << "default constructor is executed" << endl;
}

// copy constructor
void __customArrayConstructor(CustomArray* objA, const CustomArray& objB) {
	if (objB.data == nullptr) {
		__customArrayConstructor(objA);
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
void __customArrayDestructor(CustomArray* obj) {
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
	// map CustomArray in C++ into CustomArray in the script
	int typeId = fb.registerUserType("CustomArray", sizeof(CustomArray));
	if (IS_UNKNOWN_TYPE(typeId)) {
		return;
	}
	// map ostream* in C++ into ostream in the script
	int streamTypeId = fb.registerUserType("ostream", sizeof(ostream*));
	if (IS_UNKNOWN_TYPE(streamTypeId)) {
		return;
	}

	// map cout constant as _Ptr_cout. So, we can use 'operator<<' of ostream for some types
	setConstantMap(scriptCompiler, "cout", "ostream", (size_t)_Ptr_cout);
	// map endl as break line. So, we can use 'operator<<' of ostream like 'cout << endl'
	setConstantMap(scriptCompiler, "endl", "char", '\n');

	// map operators 'ostream& operator<<(ostream&, T)' in c++ into operator '<<' in the script.
	// So, we can use operator<< of these types
	registerOperator<ostream&, ostream&, const string&>(fb, operator<<, "<<", "ostream", "ostream,string&");
	registerOperator<ostream&, ostream&, char>(fb, operator<<, "<<", "ostream", "ostream,char");
	registerOperator<ostream&, ostream&, const CustomArray&>(fb, operator<<, "<<", "ostream", "ostream,CustomArray&");

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
	if( registerContructor
	   <CustomArray*> // native function prototype
		(fb, // register helper object
			__customArrayConstructor, // native function
			typeId, //contructor corressponding to typeId
			"ref CustomArray" // parameter type of the function
			) < 0 ) {
		return;
	}

	// copy constructor
	if( registerContructor
		<CustomArray*, const CustomArray&> // native function prototype
		(fb, // register helper object
			__customArrayConstructor, // native function
			typeId, //contructor corressponding to typeId
			"ref CustomArray, CustomArray&" // parameter type of the function
			) < 0 ) {
		return;
	}

	// destructor
	if( registerDestructor
		<CustomArray*> // native function prototype
		(fb, // register helper object
			__customArrayDestructor, // native function
			typeId //destructor corressponding to typeId
			) < 0) {
		return;
	}
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

