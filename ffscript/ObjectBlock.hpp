/******************************************************************
* File:        ObjectBlock.hpp
* Description: define ObjectBlock template classes. A memory block class
*              store data in argumment template type and use type's
*              destructor to implicit destroy the data in ObjectBlock's
*              destructor.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

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