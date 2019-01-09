/******************************************************************
* File:        ScopeRuntimeData.cpp
* Description: implement ScopeRuntimeData and its derived classes.
*              A scope runtime data object contains information
*              of current scope in a context. It created at run time
*              when the command pointer enter an scope and destroyed
*              when the command pointer leave current scope.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "ScopeRuntimeData.h"
#include <stdlib.h>

namespace ffscript {

	static const unsigned char mask_base = 0xF0;

	ScopeRuntimeData::ScopeRuntimeData() : _executedConstructor (nullptr)
	{
	}

	ScopeRuntimeData::~ScopeRuntimeData() {}

	ScopeRuntimeData* ScopeRuntimeData::createRuntimeData(int scopeContructorCount) {
		if (scopeContructorCount == 0) {
			return nullptr;
		}
		if (scopeContructorCount <= 8) {
			return new ScopeRuntimeDataFixSize();
		}

		return new ScopeRuntimeDataDynamicSize( (scopeContructorCount >> 3) + (scopeContructorCount & 0x07) != 0 ); // scopeContructorCount /8 + scopeContructorCount % 8
	}

	unsigned char ScopeRuntimeData::isContructorExecuted(int index) {
		int byteIndex = index >> 3; // index / 8
		index &= 0x07;   // index % 8

		unsigned char mask = mask_base >> index;
		unsigned char val = _executedConstructor[byteIndex];

		return (val & mask);
	}

	void ScopeRuntimeData::markContructorExecuted(int index) {
		int byteIndex = index >> 3; // index / 8
		index &= 0x07;   // index % 8

		unsigned char mask = mask_base >> index;
		unsigned char& val = _executedConstructor[byteIndex];

		val |= mask;
	}

	void ScopeRuntimeData::markContructorNotExecuted(int index) {
		int byteIndex = index >> 3; // index / 8
		index &= 0x07;   // index % 8

		unsigned char mask = mask_base >> index;
		unsigned char& val = _executedConstructor[byteIndex];

		val &= (~mask);
	}

	/////////////////////////////////////////////////////////////////////
	ScopeRuntimeDataFixSize::ScopeRuntimeDataFixSize() :_data(0) {
		_executedConstructor = &_data;
	}
	ScopeRuntimeDataFixSize::~ScopeRuntimeDataFixSize() {}
	
	/////////////////////////////////////////////////////////////////////
	ScopeRuntimeDataDynamicSize::ScopeRuntimeDataDynamicSize(int size) {
		_data = (unsigned char*) malloc(size);
		_executedConstructor = _data;
	}

	ScopeRuntimeDataDynamicSize::~ScopeRuntimeDataDynamicSize() {
		free(_data);
	}
}