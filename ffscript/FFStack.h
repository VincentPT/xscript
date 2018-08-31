#pragma once


namespace ffscript {

	typedef void(*RaiseStackOverflowFunc)(void);

	template <class T, int _stack_size>
	class FFStack {
		T _data[_stack_size];
		T* _p;
		T* _end;
		RaiseStackOverflowFunc _errorFunc;
	public:
		FFStack(RaiseStackOverflowFunc errorFunc) :
			_errorFunc(errorFunc)			
		{
			_p = _data - 1;
			_end = _p + _stack_size;
		}

		inline void push_front(const T& val) {
			if (_p >= _end) {
				_errorFunc();
			}
			*++_p = val;
		}

		inline void pop_front() {
			_p --;
		}

		inline T& front() {
			return *_p;
		}

		inline const T& front() const {
			return *_p;
		}

		inline int getSize() const {
			return (int)(_p - _data) + 1;
		}

		inline const T* begin() const {
			return _data;
		}

		inline const T* end() const {
			return _p + 1;
		}
	};
}