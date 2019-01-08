/******************************************************************
* File:        MemberTypeInfo.hpp
* Description: define some common template classes that used for
*              new generation of function object classes(
*              CdeclFunction3, MemberFunction3)
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "functioninternal2.hpp"

namespace FT {
	template<class T, size_t alignment>
	constexpr int getAlignSize() {
		return ((sizeof(T) - 1) / alignment + 1) * alignment;
	}

	template <int _offset, size_t alignment, class...>
	struct MemberTypeInfo;

	template <int _offset, size_t alignment>
	struct MemberTypeInfo<_offset, alignment> {
	protected:
		static constexpr int _size = 0;
	public:
		template<int index>
		static constexpr int offset() {
			static_assert(0 != index, "index out of bound");
			return _offset;
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

	struct DummyMemberTypeInfo {
	public:
		template<int index>
		static constexpr int offset() {
			return 0;
		}

		template<int index>
		static constexpr int getSize() {
			return 0;
		}
	};

	template <int _offset, size_t alignment, class _First, class... _Rest>
	struct MemberTypeInfo<_offset, alignment, _First, _Rest...> {
	protected:
		typedef typename real_type<_First>::_T first_t;
		static constexpr int _mySize = getAlignSize<first_t, alignment>();
		static constexpr int _count = 1 + sizeof...(_Rest);

		typedef MemberTypeInfo<_offset + _mySize, alignment, _Rest...> SubMemberTypeInfo;
	public:
		template<int index>
		static constexpr int offset() {
			static_assert(_count > index && index >= 0, "index out of bound");
			typedef typename std::conditional<index == 0, DummyMemberTypeInfo, SubMemberTypeInfo>::type ComputeTypeT;
			return index == 0 ? _offset : ComputeTypeT::template offset<index - 1>();
		}
		
		template<int index>
		static constexpr int getSize() {
			static_assert(_count > index && index >= 0, "index out of bound");
			typedef typename std::conditional<index == 0, DummyMemberTypeInfo, SubMemberTypeInfo>::type ComputeTypeT;
			return index <= 0 ? _mySize : ComputeTypeT::template getSize<index - 1>();
		}

		static constexpr int totalSize() {
			return _mySize + SubMemberTypeInfo::totalSize();
		}
	};
}