#include "stdafx.h"
#include "ScopeRuntimeData.h"

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