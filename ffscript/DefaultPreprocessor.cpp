#include "stdafx.h"
#include "DefaultPreprocessor.h"
#include <list>
#include <algorithm>

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
		_linesMap.clear();
		return strRef;
	}

	list<int> orignalLines;
	list<LineMapInfo> lines;

	strRef = make_shared<wstring>();

	auto c = begin;
	auto subStart = c;
	auto lineStart = c;

	int endCharIdx;

	LineMapInfo lineMapInfo;

	int totalSkipChar = 0;

	while (c < end)
	{
		if (*c == '/' && (c + 1) < end && *(c + 1) == '/') {
			strRef->append(subStart, c - subStart);

			auto d = c;
			for (c += 2; c < end && *c != '\n'; c++);

			totalSkipChar += (int)(c - d);

			if (*c == '\n') {
				subStart = c;
			}

			if(c == end) {
				break;
			}
		}
		if (*c == '\n') {
			endCharIdx = (int)(c - begin + 1);
			lineMapInfo.originalLine = (int)orignalLines.size();
			lineMapInfo.endCharIdx = endCharIdx - totalSkipChar;

			orignalLines.push_back(endCharIdx);
			lines.push_back(lineMapInfo);

			lineStart = c + 1;
		}

		c++;
	}

	if (end > lineStart) {
		endCharIdx = (int)(end - begin);
		lineMapInfo.originalLine = (int)orignalLines.size();
		lineMapInfo.endCharIdx = endCharIdx - totalSkipChar;

		orignalLines.push_back(endCharIdx);
		lines.push_back(lineMapInfo);
	}

	if (_originalLinesMap.size() != orignalLines.size()) {
		_originalLinesMap.resize(orignalLines.size());
	}
	_originalLinesMap.assign(orignalLines.begin(), orignalLines.end());

	if (_linesMap.size() != lines.size()) {
		_linesMap.resize(lines.size());
	}
	_linesMap.assign(lines.begin(), lines.end());

	strRef->append(subStart, c - subStart);
	return strRef;
}


void DefaultPreprocessor::getOriginalPosition(int charIndex, int& line, int& column) const {
	line = -1;
	column = -1;
	if (charIndex < 0) return;

	auto it = lower_bound(_linesMap.begin(), _linesMap.end(), charIndex, [](const LineMapInfo& lineMapInfo, int externalVal) {
		return (lineMapInfo.endCharIdx <= externalVal);
	});

	if (it == _linesMap.end()) {
		return;
	}

	line = it->originalLine;
	if (it == _linesMap.begin()) {
		column = charIndex;
	}
	else {
		it--;
		column = charIndex - it->endCharIdx;
	}
}