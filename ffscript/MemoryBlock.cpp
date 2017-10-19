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