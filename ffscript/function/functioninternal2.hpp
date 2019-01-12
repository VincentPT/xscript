/******************************************************************
* File:        functioninternal2.hpp
* Description: define some common template classes, functions and
*              macros used by derived classes of DFunction2.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once

#include <type_traits>

namespace FT {
	template<typename T>
	struct make_pointer {
		typedef typename std::remove_reference<T>::type* ptr_type;
	};

	template<typename... Types>
	struct correct_types;

	template<>
	struct correct_types<> {};

	template<typename T1, typename... Types>
	struct correct_types<T1, Types...> {
		typedef correct_types<Types...> sub;
		typedef typename make_pointer<T1>::ptr_type ptr_type;
		typedef typename std::conditional<std::is_reference<T1>::value, ptr_type, T1>::type T;
	};

	template<typename T>
	struct real_type {
		typedef typename make_pointer<T>::ptr_type ptr_type;
		typedef typename std::conditional<std::is_reference<T>::value, ptr_type, T>::type _T;
	};
}

#define ARG_OFFSET(idx) Helper::template offset<idx>()