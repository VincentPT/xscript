/******************************************************************
* File:        functioninternal2.hpp
* Description: define some common template classes, functions and
*              macros used by derived classes of DFunction2.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once

#include <type_traits>

#define DECLARE_CLASS_INVOKER_TEMPLATE2(className, classOwner, ...) \
class className<__VA_ARGS__> { \
public: \
	classOwner<__VA_ARGS__>* mOwner; \
	className(classOwner<__VA_ARGS__>* pOwner) { \
		mOwner = pOwner; \
	} \
	inline void call(void* pReturnVal, void* params[]); \
}
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
		typedef typename std::conditional<std::is_reference<T>::value, ptr_type, T>::type T;
	};
}