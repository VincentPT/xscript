// script headers
#include <CompilerSuite.h>
#include <CLamdaProg.h>
#include <DefaultPreprocessor.h>
#include <ScriptTask.h>
#include <Utils.h>
// string library of script
#include <RawStringLib.h>

// c++ standard headers
#include <iostream>
#include <string>

using namespace ffscript;
typedef void(*FImportLibrary)(ScriptCompiler*);

CLamdaProg* complieProgram(FImportLibrary importLib, const char* file, int globalStackSize = 1024);
// run script program which has main() function in global context
void* runProgram(CLamdaProg*);