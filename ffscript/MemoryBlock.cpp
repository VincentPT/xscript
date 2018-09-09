/******************************************************************
* File:        MemoryBlock.cpp
* Description: implement memory block classes. A memory block class
*              define the way to store data and also the way to
*              destroy its data when it is no longer used.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/


#include "stdafx.h"
#include "MemoryBlock.h"

#include <stdlib.h>

namespace ffscript {

	MemoryBlock::MemoryBlock() {}
	MemoryBlock::~MemoryBlock() {}

	BufferBlock::BufferBlock(int size){
		_buffer = (unsigned char*) malloc(size);
	}

	BufferBlock::~BufferBlock(){
		if (_buffer) {
			free(_buffer);
		}
	}

	void* BufferBlock::getDataRef() {
		return _buffer;
	}
}