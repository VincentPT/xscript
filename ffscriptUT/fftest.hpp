#pragma once
#include <gtest/gtest.h>
#define FF_TEST_CLASS(className) class className : public ::testing::Test
#define FF_TEST_METHOD(className, testName) TEST_F(className, testName)
#define FF_TEST_FUNCTION(testClassName, testName) TEST(testClassName, testName)

//#define GET_MACRO_2(_1,_2,NAME,...) NAME
//#define GET_MACRO_3(_1,_2,_3,NAME,...) NAME
//
//#define FF_EXPECT_TRUE_1(condition) EXPECT_TRUE(condition)
//#define FF_EXPECT_TRUE_2(condition, msg) EXPECT_TRUE(condition) << msg
//#define FF_EXPECT_TRUE(...) GET_MACRO_2(__VA_ARGS__, FF_EXPECT_TRUE_2, FF_EXPECT_TRUE_1)(__VA_ARGS__)
//
//#define FF_EXPECT_FALSE_1(condition) EXPECT_TRUE(condition)
//#define FF_EXPECT_FALSE_2(condition, msg) EXPECT_TRUE(condition) << msg
//#define FF_EXPECT_FALSE(...) GET_MACRO_2(__VA_ARGS__, FF_EXPECT_FALSE_2, FF_EXPECT_FALSE_1)(__VA_ARGS__)
//
//#define FF_EXPECT_NE_2(ne, val) EXPECT_NE(ne, val)
//#define FF_EXPECT_NE_3(ne, val, msg) EXPECT_NE(ne, val) << msg
//#define FF_EXPECT_NE(...) GET_MACRO_3(__VA_ARGS__, FF_EXPECT_NE_3, FF_EXPECT_NE_2)(__VA_ARGS__)
//
//#define FF_EXPECT_EQ_2(e, val) EXPECT_EQ(e, val)
//#define FF_EXPECT_EQ_3(e, val, msg) EXPECT_EQ(e, val) << msg
//#define FF_EXPECT_EQ(...) GET_MACRO_3(__VA_ARGS__, FF_EXPECT_EQ_3, FF_EXPECT_EQ_2)(__VA_ARGS__)

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

inline void FF_EXPECT_TRUE(bool condition, const wchar_t* msg) {
	EXPECT_TRUE(condition) << msg;
}

inline void FF_EXPECT_TRUE(bool condition) {
	EXPECT_TRUE(condition);
}

inline void FF_EXPECT_FALSE(bool condition, const wchar_t* msg) {
	EXPECT_FALSE(condition) << msg;
}

inline void FF_EXPECT_FALSE(bool condition) {
	EXPECT_FALSE(condition);
}

template <class T1, class T2>
inline void FF_EXPECT_NE(const T1& not_expected, const T2& val) {
	EXPECT_NE(not_expected, val);
}

template <class T1, class T2>
inline void FF_EXPECT_NE(const T1& not_expected, const T2& val, const wchar_t* msg) {
	EXPECT_NE(not_expected, val) << msg;
}

template <class T1, class T2>
inline void FF_EXPECT_EQ(const T1& expected, const T2& val) {
	EXPECT_EQ(expected, val);
}

template <class T1, class T2>
inline void FF_EXPECT_EQ(const T1& expected, const T2& val, const wchar_t* msg) {
	EXPECT_EQ(expected, val) << msg;
}

template <class T1, class T2>
inline void FF_EXPECT_STREQ(const T1& expected, const T2& val) {
	EXPECT_STREQ(expected, val);
}

template <class T1, class T2>
inline void FF_EXPECT_STREQ(const T1& expected, const T2& val, const wchar_t* msg) {
	EXPECT_STREQ(expected, val) << msg;
}