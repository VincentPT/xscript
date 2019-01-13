/******************************************************************
* File:        FTCachedDelegateInternal.hpp
* Description: template class for cached method using delegates in
*              namespace FT.
* Author:      Vincent Pham
*
* Copyright (c) 2019 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "MemberTypeInfo.hpp"

namespace FTCached {
	struct SetArgRef {
		template <class T>
		static void set(char* base, T val) {
			typedef typename std::remove_reference<T>::type val_type;
			*(val_type**)base = &val;
		}
	};

	struct SetArgVal {
		template <class T>
		static void set(char* base, T val) {
			*(T*)base = val;
		}
	};

	template <size_t alignment>
	struct ArgumentTuple {
		template <class...Types>
		static void setArg(char* base, Types...) {}

		template <class First>
		static void setArg(char* base, First first) {
			typedef typename std::conditional<std::is_reference<First>::value, SetArgRef, SetArgVal>::type SetArg;
			SetArg::template set<First>(base, first);
		}

		template <class First, class...Rest>
		static void setArg(char* base, First first, Rest...rest) {
			typedef typename std::conditional<std::is_reference<First>::value, SetArgRef, SetArgVal>::type SetArg;
			SetArg::template set<First>(base, first);

			constexpr int size = FT::getAlignSize<First, alignment>();
			ArgumentTuple<alignment>::setArg<Rest...>(base + size, rest...);
		}

		template <class...Types>
		static constexpr int totalSize() {
			return MemberTypeInfo<0, alignment, Types...>::totalSize();
		}
	};

	template <class...Types>
	class FTCachedInvoker {
		static constexpr size_t ARG_SIZE = ArgumentTuple<ARG_ALIGMENT_SIZE>::template totalSize<Types...>();
	protected:
		char _argumentData[ARG_SIZE + 1];
	public:
		void setArgs(Types... types) {
			ArgumentTuple<ARG_ALIGMENT_SIZE>::setArg<Types...>(_argumentData, types...);
		}
	};
}