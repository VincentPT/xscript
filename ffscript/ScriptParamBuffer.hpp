/******************************************************************
* File:        ScriptParamBuffer.hpp
* Description: define ScriptParamBuffer template class. This class
*              is designed to serialize script function's arguments
*              then pass it to the script function before execute
*              the function.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <vector>

namespace ffscript {
	class ScriptParamBuffer {
		std::vector<size_t> _paramContainer;
	public:
		const void* getBuffer() const {
			return _paramContainer.data();
		}
		ScriptParamBuffer() {}

		template <typename T>
		ScriptParamBuffer(const T& param) {
			addParam(param);
		}

		template <typename T>
		void addParam(const T& param) {
			int elem = (sizeof(param) - 1)/sizeof(size_t) + 1;
			void* begin = _paramContainer.data() + _paramContainer.size();
			size_t* elmsData = (size_t*)&param;
			size_t* elmsEnd = elmsData + elem;
			while (elmsData < elmsEnd)
			{
				_paramContainer.push_back(*elmsData++);				
			}
		}
	};
}