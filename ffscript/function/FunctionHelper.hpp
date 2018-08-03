#pragma once
#include "CdeclFunction2.hpp"
#include "CdeclFunction3.hpp"
#include <type_traits>


namespace FT {
	//////////////////////////////////////////////////////////////
	///
	/// check if there is any referene type in list of types
	///
	//////////////////////////////////////////////////////////////
	template <typename...>
	struct has_reference;

	template <>
	struct has_reference<> {
		static constexpr bool value = false;
	};

	template <typename T1, typename... Ts>
	struct has_reference<T1, Ts...> {
		static constexpr bool value = std::is_reference<T1>::value ? true : has_reference<Ts...>::value;
	};

	//////////////////////////////////////////////////////////////
	///
	/// check if there is any type is same with the first type
	/// in list of types
	///
	//////////////////////////////////////////////////////////////
	template <typename T, typename...>
	struct has_type;

	template <typename T>
	struct has_type<T> {
		static constexpr bool value = false;
	};

	template <typename T, typename T1, typename... Types>
	struct has_type<T, T1, Types...> {
		static constexpr bool value = std::is_same<T, T1>::value ? true : has_type<T, Types...>::value;
	};

	//////////////////////////////////////////////////////////////
	///
	/// check if there is any type that its size is exceed stack
	/// unit size
	///
	//////////////////////////////////////////////////////////////
	template <typename...>
	struct no_exceed_uts;

	template <typename T>
	struct no_exceed_uts<T> {
		static constexpr bool value = sizeof(T) <= sizeof(void*);
	};

	template <typename T1, typename... Ts>
	struct no_exceed_uts<T1, Ts...> {
		static constexpr bool value = sizeof(T1) > sizeof(void*) ? false : no_exceed_uts<Ts...>::value;
	};

	// int& (*)(int, float) => must use CdeclFunction3
	// void (*)(int&, float) => must use CdeclFunction3
	// void (*)(int, char) => should use CdeclFunction2
	// void (*)(int, char*) => should use CdeclFunction2

	template <typename... Types>
	constexpr bool isCdel2Applicable() {
		return no_exceed_uts<Types...>::value == false &&
			has_type<float, Types...>::value == false &&
			has_type<double, Types...>::value == false;
	}

	template <typename Ret, typename... Types>
	DFunction2* createFunction(Ret(*f)(Types...) ) {
		if (isCdel2Applicable<Types...>()) {
			return new CdeclInvoker2<Ret, Types...>(f);
		}

		return new CCdeclInvoker3<Ret, Types...>(f);
	}
}