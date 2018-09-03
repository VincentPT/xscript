/******************************************************************
* File:        DefaultPreprocessor.h
* Description: declare default preprocessor class.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "Preprocessor.h"
#include <vector>

class DefaultPreprocessor :
	public Preprocessor
{
protected:

	struct LineMapInfo {
		int originalLine;
		int endCharIdx;
	};
protected:
	std::vector<LineMapInfo> _linesMap;
	std::vector<int> _originalLinesMap;
public:
	DefaultPreprocessor();
	virtual ~DefaultPreprocessor();
	std::shared_ptr<std::wstring> preprocess(const wchar_t* begin, const wchar_t* end);
	void getOriginalPosition(int charIndex, int& line, int& column) const;
};

