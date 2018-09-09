/******************************************************************
* File:        TutorialCommon.h
* Description: Contains common headers of C Lambda compiler library
*              and c++ standards libraries.
*              This file also declares common utilities function used
*              for tutorials of usage C Lambda scripting language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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

CLamdaProg* compileProgram(FImportLibrary importLib, const char* file, int globalStackSize = 1024);
// run script program which has main() function in global context
void* runProgram(CLamdaProg*);