/******************************************************************
* File:        Preprocessor.h
* Description: declare Preprocessor abstract class. A preprocessor
*              class is a script preprocessor that used to store
*              code information and filter code such as user's
*              comments before the code is compiled.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <string>
#include <memory>

class Preprocessor
{
public:
	Preprocessor();
	virtual ~Preprocessor();

	virtual std::shared_ptr<std::wstring> preprocess(const wchar_t* begin, const wchar_t* end) = 0;
	virtual void getOriginalPosition(int charIndex, int& line, int& column) const = 0;
};

typedef std::shared_ptr<Preprocessor> PreprocessorRef;
