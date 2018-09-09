/******************************************************************
* File:        FFScriptArray.hpp
* Description: define FFScriptArray template class. A class is
*              designed to use as a dynamic array in the script
*              but it still not be used.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <algorithm>
#include <functional>

namespace ffscript {

	template <class T>
	class FFScriptArray
	//struct FFScriptArray
	{
		T*		_data;
		size_t	_size;
		size_t	_capacity;
		bool	_allocated;
	public:
		FFScriptArray(T* pData, size_t size) : _allocated(false), _data(pData), _size(size), _capacity(0) {
		}

		FFScriptArray(size_t size) : _allocated(true), _size(size), _capacity(size) {
			_data = new T[_size];
		}

		static inline void copy(T* pData, const T* source, size_t size) {
			T* pVal = pData;
			T* pEnd = pData + size;

			while (pVal < pEnd)
			{
				*pVal++ = *source++;
			}
		}

		FFScriptArray(const FFScriptArray& arr) : _allocated(true), _size(arr._size), _capacity(arr._size) {
			_data = new T[_size];
			const T* source = arr._data;
			copy(_data, source, _size);
		}

		void resize(size_t newSize) {
			if (newSize > _capacity) {
				_capacity = newSize;
				T* pNewData = new T[_capacity];
				copy(pNewData, _data, _size);

				if (_allocated) {
					delete[]_data;
				}
				_data = pNewData;
				_allocated = true;
			}

			_size = newSize; 
		}

		void sort() {
			std::sort(_data, _data + _size);
		}

		template <class Pr>
		void sort( Pr f ) {
			std::sort(_data, _data + _size, f);
		}

		T& operator[](int i) {
			return _data[i];
		}

		const T& operator[](int i) const {
			return _data[i];
		}

		~FFScriptArray() {
			if (_allocated) {
				delete[] _data;
			}
		}
	};
}
