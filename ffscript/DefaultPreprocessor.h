#pragma once
#include "Preprocessor.h"
class DefaultPreprocessor :
	public Preprocessor
{
public:
	DefaultPreprocessor();
	virtual ~DefaultPreprocessor();
	std::shared_ptr<std::wstring> preprocess(const wchar_t* begin, const wchar_t* end);
};

