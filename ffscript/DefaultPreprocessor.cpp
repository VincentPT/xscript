#include "stdafx.h"
#include "DefaultPreprocessor.h"

using namespace std;

DefaultPreprocessor::DefaultPreprocessor()
{
}


DefaultPreprocessor::~DefaultPreprocessor()
{
}

shared_ptr<wstring> DefaultPreprocessor::preprocess(const wchar_t* begin, const wchar_t* end) {
	std::shared_ptr<std::wstring> strRef;
	if (begin == nullptr || end == nullptr) {
		return strRef;
	}
	strRef = make_shared<wstring>();

	auto c = begin;
	auto subStart = c;
	while (c < end)
	{
		if (*c == '/' && (c + 1) < end && *(c + 1) == '/') {
			strRef->append(subStart, c - subStart);
			for (c += 2; c < end && *c != '\n'; c++);

			if (*c == '\n') {
				subStart = c;
			}
		}
		c++;
	}
	strRef->append(subStart, c - subStart);
	return strRef;
}
