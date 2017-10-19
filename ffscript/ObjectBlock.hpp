#pragma once
#include "MemoryBlock.h"

namespace ffscript {
	template <class T>
	class ObjectBlock :
		public MemoryBlock
	{
		T _object;
	public:
		ObjectBlock() {};
		ObjectBlock(const T& object) : _object(object) {}
		virtual ~ObjectBlock() {
		}
		void* getDataRef() {
			return &_object;
		}
	};
}