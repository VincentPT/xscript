/******************************************************************
* File:        BasicOperators.hpp
* Description: define function template for basic operators such as
*              operator+, operator-,...
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"

namespace ffscript {
	namespace basic {
		namespace operators {
			template <class RT, class T1, class T2>
			RT add(T1 a, T2 b) {
				return a + b;
			}

			template <class RT, class T1, class T2>
			RT sub(T1 a, T2 b) {
				return a - b;
			}

			template <class RT, class T1, class T2>
			RT mul(T1 a, T2 b) {
				return a * b;
			}

			template <class RT, class T1, class T2>
			RT div(T1 a, T2 b) {
				return a / b;
			}

			template <class RT, class T1, class T2>
			RT mod(T1 a, T2 b) {
				return a % b;
			}

			template <class RT, class T>
			RT neg(T a) {
				return -a;
			}

			template <class RT, class T1, class T2>
			RT assign(T1 & a, T2 b) {
				return a = (T1)b;
			}

#pragma region bitwise operators
			template <class RT, class T1, class T2>
			RT and (T1 a, T2 b) { return a & b; }

			template <class RT, class T1, class T2>
			RT or (T1 a, T2 b) { return a | b; }

			template <class RT, class T1, class T2>
			RT xor (T1 a, T2 b) { return a ^ b; }

			template <class RT, class T>
			RT not(T a) { return ~a; }

			template <class RT, class T1, class T2>
			RT shiftLeft(T1 a, T2 b) { return a << b; }

			template <class RT, class T1, class T2>
			RT shiftRight(T1 a, T2 b) { return a >> b; }
#pragma endregion

			template <class T>
			T post_inc(T& a) { return a++; }

			template <class T>
			T pre_inc(T& a) { return ++a; }

			template <class T>
			T post_dec(T& a) { return a--; }

			template <class T>
			T pre_dec(T& a) { return --a; }

#pragma region compound operators
			template <class T1, class T2>
			void mul_comp(T1& a, T2 b) { a *= (T1)b; }

			template <class T1, class T2>
			void div_comp(T1& a, T2 b) { a /= (T1)b; }

			template <class T1, class T2>
			void add_comp(T1& a, T2 b) {
				a += (T1)b;
			}

			template <class T1, class T2>
			void sub_comp(T1& a, T2 b) { a -= (T1)b; }

			template <class T1, class T2>
			void mod_comp(T1& a, T2 b) { a %= (T1)b; }

			template <class T1, class T2>
			void and_comp(T1& a, T2 b) { a &= b; }

			template <class T1, class T2>
			void or_comp(T1& a, T2 b) { a |= b; }

			template <class T1, class T2>
			void xor_comp(T1& a, T2 b) { a ^= b; }

			template <class T1, class T2>
			void shiftLeft_comp(T1& a, T2 b) { a <<= b; }

			template <class T1, class T2>
			void shiftRight_comp(T1& a, T2 b) { a >>= b; }
#pragma endregion

#pragma region compare functions
			template <class T1, class T2>
			bool less(T1 a, T2 b) { return a < b; }

			template <class T1, class T2>
			bool less_or_equal(T1 a, T2 b) { return a <= b; }

			template <class T1, class T2>
			bool great(T1 a, T2 b) {
				return (a > b);
			}

			template <class T1, class T2>
			bool great_or_equal(T1 a, T2 b) { return a >= b; }

			template <class T1, class T2>
			bool equal(T1 a, T2 b) { return a == b; }

			template <class T1, class T2>
			bool not_equal(T1 a, T2 b) { return a != b; }
#pragma endregion

#pragma region logic functions
			template <class T1, class T2>
			bool logic_and(T1 a, T2 b) { return (a && b); }

			template <class T1, class T2>
			bool logic_or(T1 a, T2 b) { return (a || b); }

			template <class T>
			bool logic_not(T a) { return !a; }
#pragma endregion
		}
	}
}