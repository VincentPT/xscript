#pragma once

#include <string>

template <typename CharType>
inline const CharType* trimLeft(const CharType* s, const CharType* end) {
	const CharType* c = s;
	while (c < end && (*c == ' ' || *c =='\t') || *c == '\r' || *c == '\n')
	{
		c++;
	}
	return c;
}

int parseType(std::string& type);
bool checkPointerType(const std::string& type);
bool checkArrayType(const std::string& type);