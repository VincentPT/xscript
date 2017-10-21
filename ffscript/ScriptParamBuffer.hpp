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