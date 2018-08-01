#pragma once
#include <type_traits>

template<class T, size_t alignment>
constexpr int getAlignSize() {
	return ((sizeof(T) - 1) / alignment + 1) * alignment;
}

template <int offset, size_t alignment, class...>
struct MemberTypeInfo;

template <int offset, size_t alignment>
struct MemberTypeInfo<offset, alignment> {
protected:
	static constexpr int _size = 0;
public:
	template<int index>
	static constexpr int getOffset() {
		static_assert(0 != index, "index out of bound");
		return offset;
	}

	template<int index>
	static constexpr int getSize() {
		static_assert(0 != index, "index out of bound");
		return 0;
	}

	static constexpr int totalSize() {
		return 0;
	}
};

template <int offset, size_t alignment, class _First, class... _Rest>
struct MemberTypeInfo<offset, alignment, _First, _Rest...> /*: private MemberTypeInfo<offset + getAlignSize<_First, alignment>(), alignment, _Rest...>*/ {
protected:
	static constexpr int _mySize = getAlignSize<_First, alignment>();
	static constexpr int _count = 1 + sizeof...(_Rest);

	typedef MemberTypeInfo<offset + _mySize, alignment, _Rest...> _MyBase;
public:
	template<int index>
	static constexpr int getOffset() {
		static_assert(_count > index/* && index >= 0*/, "index out of bound");
		return index <= 0 ? offset : _MyBase::getOffset<index - 1>();
	}

	template<int index>
	static constexpr int getSize() {
		static_assert(_count > index/* && index >= 0*/, "index out of bound");
		return index <= 0 ? _mySize : _MyBase::getSize<index - 1>();
	}

	static constexpr int totalSize() {
		return _mySize + _MyBase::totalSize();
	}
};

