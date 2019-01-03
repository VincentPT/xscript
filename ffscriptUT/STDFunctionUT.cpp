/******************************************************************
* File:        STDFunctionUT.cpp
* Description: Test cases invoking __stdcall functions using StdFunction.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include "ExpresionParser.h"
#include "function/STDFunction.hpp"
#include "TemplateForTest.hpp"

#include <functional>
#include <math.h>

using namespace std;

namespace ffscriptUT
{
	void __stdcall stdfoo(char* a, const float& b) {

		string message = a;
		message += to_string(b);

		PRINT_TEST_MESSAGE(message.c_str());		
	}

	void __stdcall stdfoo(const float& a, const float& b) {

		char sbuff[128];
		sprintf_s(sbuff, "a=%f, b=%f", a, b );
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d)", a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c)", a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf)", a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p)", a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, __int64 h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %lld)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d, const double& e, void* f, __int64 g) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %lld)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall stdfoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void __stdcall sayHello() {
		PRINT_TEST_MESSAGE(__FUNCTION__" Hello word!");
	}

	void __stdcall sayHello( char* name ) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__" Hello %s", name);
		PRINT_TEST_MESSAGE(sbuff);
	}

	float __stdcall stdfoof(char* a, const float& b) {

		string message = a;
		message += to_string(b);

		PRINT_TEST_MESSAGE(message.c_str());

		return b;
	}

	float __stdcall stdfoof(const float& a, const float& b) {
		return (a + b);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d)", a, b, c);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c)", a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf)", a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + (float)e);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p)", a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + (float)e);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + (float)e + g);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, __int64 h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %lld)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + (float)e + g + (float)h);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d, const double& e, void* f, __int64 g) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %lld)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + (float)e + (float)g);
	}

	float __stdcall stdfoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + (float)e + (float)g + h);
	}

	double __stdcall stdfood(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return (a + b + c + d + e + g + h);
	}

	__int64 __stdcall stdfooi64(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return ((__int64)a + (__int64)b + c + d + (__int64)e + g + h);
	}

	unsigned __int64 __stdcall stdfooui64(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return ((unsigned __int64)a + (unsigned __int64)b + c + d + (unsigned __int64)e + g + h);
	}

	float __stdcall sayHellof() {
		PRINT_TEST_MESSAGE(__FUNCTION__" Hello word!");
		return 1.0f;
	}

	float __stdcall sayHellof(char* name) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__" Hello %s", name);
		PRINT_TEST_MESSAGE(sbuff);
		return 1.0f;
	}

	char __stdcall stdFooGeneral() {
		PRINT_TEST_MESSAGE(__FUNCTION__);
		return 1;
	}

	short __stdcall stdFooGeneral(int a) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d)", a);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	float __stdcall stdFooGeneral(int a, char b) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c)", a, b);
		PRINT_TEST_MESSAGE(sbuff);
		return 1.0f;
	}

	int __stdcall stdFooGeneral(int a, char b, const double& c) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf)", a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long __stdcall stdFooGeneral(int a, char b, const double& c, const float& d) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f)", a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long long __stdcall stdFooGeneral(int a, char b, const double& c, const float& d, unsigned int e) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u)", a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000;
	}

	double __stdcall stdFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u, %d)", a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000.0;
	}

	unsigned short __stdcall stdFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u, %d, %d)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	namespace STDFunctionUT
	{
		/*test no param funtion*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction1)
		{
			StdFunction<void> stdfunc(sayHello);

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test one param funtion*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction2)
		{
			StdFunction<void, char*> stdfunc(sayHello);

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam("firefly");
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(char*, const float&) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction3)
		{
			StdFunction<void, char*, const float&> stdfunc(stdfoo);
			stdfunc("value of b is ", 1.0f);

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(char*, const float&) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction4)
		{
			StdFunction<void, char*, const float&> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue = 1.0f;			
			dynamicCaller->pushParam("value of b is ");
			dynamicCaller->pushParam(&forRefValue);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction5)
		{
			StdFunction<void, const float&, const float&> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;			
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction6)
		{
			StdFunction<void, const float&, const float&, int> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction7)
		{
			StdFunction<void, const float&, const float&, int, char> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction8)
		{
			StdFunction<void, const float&, const float&, int, char, const double&> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction9)
		{
			StdFunction<void, const float&, const float&, int, char, const double&, void*> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction10)
		{
			StdFunction<void, const float&, const float&, int, char, const double&, void*, short> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, __int64>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction11)
		{
			StdFunction<void, const float&, const float&, int, char, const double&, void*, __int64> stdfunc(stdfoo);
			stdfunc(1.0f, 2.0f, 3, '4', 5.0, (void*)6, 7ll);

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, __int64>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction12)
		{
#ifdef ENVIRONMENT64
			StdFunction<void, const float&, const float&, int, char, const double&, void*, __int64> stdfunc(stdfoo);
#else
			typedef void(__stdcall *STDFunc)(const float&, const float&, int, char, const double&, void*, __int64);
			STDFunc func = stdfoo;
			StdFunction<void, const float&, const float&, int, char, const double&, void*, int, int> stdfunc(
				(StdFunction<void, const float&, const float&, int, char, const double&, void*, int, int>::STDFunc)
				func);
#endif

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;
			__int64 param8th = 7;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)6);
#ifdef ENVIRONMENT64
			dynamicCaller->pushParam((void*)(param8th));
			
#else
			dynamicCaller->pushParam((void*)(int)(param8th & 0xFFFFFFFF));
			dynamicCaller->pushParam((void*)(int)(param8th >> 32));
#endif
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, __int64>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction13)
		{
			StdFunction<void, const float&, const float&, int, char, const double&, void*, short, __int64> stdfunc(stdfoo);
			stdfunc(1.0f, 2.0f, 3, '4', 5.0, (void*)6, 7, 8ll);

			FF_EXPECT_TRUE(true);
		}

#ifdef ENVIRONMENT64
		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction15)
		{
			/*This test case can run on 64bit platform only*/
			StdFunction<void, const float&, const float&, int, char, const double&, void*, short, __int64> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;

			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->pushParam((void*)8);

			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}
#endif
		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction16)
		{
			StdFunction<void, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfoo);
			
			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;

			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)3);
			dynamicCaller->pushParam((void*)'4');
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->pushParam((void*)8);

			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction17)
		{
			StdFunction<void, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfoo);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			double forRefValue3 = 5.0f;
			
			//dynamicCaller->pushParams(8, &forRefValue1, &forRefValue2, 3, '4', &forRefValue3, 6, 7, 8);
			pushParams(dynamicCaller, &forRefValue1, &forRefValue2, (void*)3, (void*)'4', &forRefValue3, (void*)6, (void*)7, (void*)8);
			dynamicCaller->call();

			FF_EXPECT_TRUE(true);
		}

		FF_TEST_FUNCTION(STDFunction, testSTDFunction18)
		{
			StdFunction<float> stdfunc(sayHellof);

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->call();

			FF_EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test one param funtion*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction19)
		{
			StdFunction<float, char*> stdfunc(sayHellof);

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam("firefly");
			dynamicCaller->call();

			FF_EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(char*, const float&) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction20)
		{
			StdFunction<float, char*, const float&> stdfunc(stdfoof);
			float p1 = 1.0f;			

			FF_EXPECT_EQ(1.0f, stdfunc("value of b is ", p1));
		}

		/*test two params funtion void(char*, const float&) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction21)
		{
			StdFunction<float, char*, const float&> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue = 1.0f;
			dynamicCaller->pushParam("value of b is ");
			dynamicCaller->pushParam(&forRefValue);
			dynamicCaller->call();

			FF_EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction22)
		{
			StdFunction<float, const float&, const float&> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction23)
		{
			StdFunction<float, const float&, const float&, int> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction24)
		{
			StdFunction<float, const float&, const float&, int, char> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction25)
		{
			StdFunction<float, const float&, const float&, int, char, const double&> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction26)
		{
			StdFunction<float, const float&, const float&, int, char, const double&, void*> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)p6);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction27)
		{
			StdFunction<float, const float&, const float&, int, char, const double&, void*, short> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)p6);
			dynamicCaller->pushParam((void*)(size_t)p7);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + p7), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, __int64>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction28)
		{
			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			__int64 p7 = 7;

			StdFunction<float, const float&, const float&, int, char, const double&, void*, __int64> stdfunc(stdfoof);
			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + (float)p7), stdfunc(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6, p7));
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, __int64>) - dynamic call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction29)
		{
#ifdef ENVIRONMENT64
			StdFunction<float, const float&, const float&, int, char, const double&, void*, __int64> stdfunc(stdfoof);
#else
			typedef float(__stdcall *STDFunc)(const float&, const float&, int, char, const double&, void*, __int64);
			STDFunc func = stdfoof;
			StdFunction<float, const float&, const float&, int, char, const double&, void*, int, int> stdfunc(
				(StdFunction<float, const float&, const float&, int, char, const double&, void*, int, int>::STDFunc)
				func);
#endif

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			__int64 p7 = 7;
			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)p6);
#ifdef ENVIRONMENT64
			dynamicCaller->pushParam((void*)(p7));

#else
			dynamicCaller->pushParam((void*)(int)(p7 & 0xFFFFFFFF));
			dynamicCaller->pushParam((void*)(int)(p7 >> 32));
#endif
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + (float)p7), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, __int64>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction30)
		{
			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			__int64 p8 = 8;

			StdFunction<float, const float&, const float&, int, char, const double&, void*, short, __int64> stdfunc(stdfoof);
			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + (float)p7 + (float)p8), stdfunc(forRefValue1, forRefValue2, p3, p4, forRefValue3, p6, p7, p8));
		}

#ifdef ENVIRONMENT64
		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction31)
		{
			/*This test case can run on 64bit platform only*/
			StdFunction<float, const float&, const float&, int, char, const double&, void*, short, __int64> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			__int64 p8 = 8;

			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)p6);
			dynamicCaller->pushParam((void*)(size_t)p7);
			dynamicCaller->pushParam((void*)p8);

			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + (float)p7 + (float)p8), dynamicCaller->getReturnValAsFloat());
		}
#endif
		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction32)
		{
			StdFunction<float, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			dynamicCaller->pushParam(&forRefValue1);
			dynamicCaller->pushParam(&forRefValue2);
			dynamicCaller->pushParam((void*)(size_t)p3);
			dynamicCaller->pushParam((void*)(size_t)p4);
			dynamicCaller->pushParam(&forRefValue3);
			dynamicCaller->pushParam((void*)p6);
			dynamicCaller->pushParam((void*)(size_t)p7);
			dynamicCaller->pushParam((void*)(size_t)p8);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + (float)p7 + (float)p8), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction33)
		{
			StdFunction<float, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfoof);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(dynamicCaller, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + (float)forRefValue3 + (float)p7 + (float)p8), dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction34)
		{
			StdFunction<double, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfood);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(dynamicCaller, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);
			dynamicCaller->call();

			FF_EXPECT_EQ((forRefValue1 + forRefValue2 + p3 + p4 + forRefValue3 + p7 + p8), dynamicCaller->getReturnValAsDouble());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction35)
		{
			StdFunction<__int64, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfooi64);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(dynamicCaller, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);
			dynamicCaller->call();

			FF_EXPECT_EQ( (int)( (__int64)forRefValue1 + (__int64)forRefValue2 + p3 + p4 + (__int64)forRefValue3 + p7 + p8), (int)dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction36)
		{
			StdFunction<unsigned __int64, const float&, const float&, int, char, const double&, void*, short, int> stdfunc(stdfooui64);

			DFunction* dynamicCaller = &stdfunc;

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(dynamicCaller, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);
			dynamicCaller->call();

			FF_EXPECT_EQ((int)((unsigned __int64)forRefValue1 + (unsigned __int64)forRefValue2 + p3 + p4 + (unsigned __int64)forRefValue3 + p7 + p8), (int)dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction37)
		{
			StdFunction<char> stdfunc(stdFooGeneral);

			DFunction* dynamicCaller = &stdfunc;			
			dynamicCaller->call();

			FF_EXPECT_EQ((char)1, (char)dynamicCaller->getReturnValAsInt8());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction38)
		{
			StdFunction<short, int> stdfunc(stdFooGeneral);

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->call();

			FF_EXPECT_EQ((short)1, (short)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction39)
		{
			StdFunction<float, int, char> stdfunc(stdFooGeneral);

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->call();

			FF_EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction40)
		{
			StdFunction<int, int, char, const double&> stdfunc(stdFooGeneral);

			double p3 = 3.0f;

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction41)
		{
			StdFunction<long, int, char, const double&, const float&> stdfunc(stdFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction42)
		{
			StdFunction<long long, int, char, const double&, const float&, unsigned int> stdfunc(stdFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->call();

			FF_EXPECT_TRUE(1000000000000000  == dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction43)
		{
			StdFunction<double, int, char, const double&, const float&, unsigned int, short> stdfunc(stdFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->call();

			FF_EXPECT_EQ(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction44)
		{
			StdFunction<unsigned short, int, char, const double&, const float&, unsigned int, short, long> stdfunc(stdFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		FF_TEST_FUNCTION(STDFunction, testSTDFunction45)
		{
			StdFunction<unsigned short, int, char, const double&, const float&, unsigned int, short, long> stdfunc(stdFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &stdfunc;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->pushParam((void*)6);
			dynamicCaller->pushParam((void*)7);
			dynamicCaller->call();

			FF_EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());

			DFunction* pNewInsance = dynamicCaller->clone();
			pNewInsance->call();

			FF_EXPECT_EQ(1, (int)pNewInsance->getReturnValAsInt16());
		}
	};
}