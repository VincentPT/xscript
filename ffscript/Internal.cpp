/******************************************************************
* File:        Internal.cpp
* Description: implement some internal functions.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "Internal.h"
#include "ffscript.h"

bool checkPointerType(const std::string& type) {
	if (strncmp(type.c_str(), POINTER_SIGN " ", sizeof(POINTER_SIGN)) == 0) {
		return true;
	}
	return false;
}

bool checkArrayType(const std::string& type) {
	if (strncmp(type.c_str(), ARRAY_SIGN " ", sizeof(ARRAY_SIGN)) == 0) {
		return true;
	}
	return false;
}

int parseType(std::string& type) {
	if (checkPointerType(type)) {
		type.erase(type.begin(), type.begin() + sizeof(POINTER_SIGN));
		return DATA_TYPE_POINTER_MASK;
	}
	if (checkArrayType(type)) {
		type.erase(type.begin(), type.begin() + sizeof(ARRAY_SIGN));
		return DATA_TYPE_ARRAY_MASK;
	}
	return 0;
}