/******************************************************************
* File:        CdeclFunctionUT.cpp
* Description: Test cases invoking cdecl functions using CdeclFunction,
*              CdeclFunction2, CdeclFunction3 function objects.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include <gtest/gtest.h>

#include <functional>
#include "function\CdeclFunction.hpp"
#include "function\CdeclFunction2.hpp"
#include "function\CdeclFunction3.hpp"
#include "TemplateForTest.hpp"

using namespace std;
using namespace FT;

namespace ffscriptUT
{
	void cdeclFoo() {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a) {

		char sbuff[128];
		sprintf_s(sbuff, "a=%f", a);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b) {

		char sbuff[128];
		sprintf_s(sbuff, "a=%f, b=%f", a, b);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b, int c) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d)", a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c)", a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf)", a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p)", a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void cdeclFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);
	}

	float cdeclFoof() {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a) {

		char sbuff[128];
		sprintf_s(sbuff, "a=%f", a);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b) {

		char sbuff[128];
		sprintf_s(sbuff, "a=%f, b=%f", a, b);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b, int c) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d)", a, b, c);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c)", a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf)", a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p)", a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float cdeclFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	char cdeclFooGeneral() {
		PRINT_TEST_MESSAGE(__FUNCTION__);
		return 1;
	}

	short cdeclFooGeneral(int a) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d)", a);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	float cdeclFooGeneral(int a, char b) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c)", a, b);
		PRINT_TEST_MESSAGE(sbuff);
		return 1.0f;
	}

	int cdeclFooGeneral(int a, char b, const double& c) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf)", a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long cdeclFooGeneral(int a, char b, const double& c, const float& d) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f)", a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long long cdeclFooGeneral(int a, char b, const double& c, const float& d, unsigned int e) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u)", a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000;
	}

	double cdeclFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u, %d)", a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000.0;
	}

	unsigned short cdeclFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g) {
		char sbuff[128];
		sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u, %d, %d)", a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

		
	TEST(CdeclFunction, testCdeclunctionVoid1)
	{
		CdeclFunction<void> cdeclFunction(&cdeclFoo);
		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid2)
	{
		CdeclFunction<void, const float&> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;		

		pushParams(&cdeclFunction, &forRefValue1);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid3)
	{
		CdeclFunction<void, const float&, const float&> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid4)
	{
		CdeclFunction<void, const float&, const float&, int> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid5)
	{
		CdeclFunction<void, const float&, const float&, int, char> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid6)
	{
		CdeclFunction<void, const float&,
			const float&, int, char, const double&> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid7)
	{
		CdeclFunction<void, const float&,
			const float&, int, char, const double&, void*> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid8)
	{
		CdeclFunction<void, const float&,
			const float&, int, char, const double&, void*, short> cdeclFunction(&cdeclFoo);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionVoid9)
	{
		CdeclFunction<void, const float&,
			const float&, int, char, const double&, void*, short, int> cdeclFunction(&cdeclFoo);
			
		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;
		int p8 = 8;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);

		cdeclFunction.call();
	}

	TEST(CdeclFunction, testCdeclunctionFloat1)
	{
		CdeclFunction<float> cdeclFunction(&cdeclFoof);
		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat2)
	{
		CdeclFunction<float, const float&> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;

		pushParams(&cdeclFunction, &forRefValue1);

		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat3)
	{
		CdeclFunction<float, const float&, const float&> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2);

		float expectedVal = 3.0f;

		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat4)
	{
		CdeclFunction<float, const float&, const float&, int> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3);
		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat5)
	{
		CdeclFunction<float, const float&, const float&, int, char> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4);

		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat6)
	{
		CdeclFunction<float, const float&,
			const float&, int, char, const double&> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat7)
	{
		CdeclFunction<float, const float&,
			const float&, int, char, const double&, void*> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat8)
	{
		CdeclFunction<float, const float&,
			const float&, int, char, const double&, void*, short> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	TEST(CdeclFunction, testCdeclunctionFloat9)
	{
		CdeclFunction<float, const float&,
			const float&, int, char, const double&, void*, short, int> cdeclFunction(&cdeclFoof);

		float forRefValue1 = 1.0f;
		float forRefValue2 = 2.0f;
		int p3 = 3;
		char p4 = '4';
		double forRefValue3 = 5.0f;
		void* p6 = (void*)6;
		short p7 = 7;
		int p8 = 8;

		pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);
			
		cdeclFunction.call();

		float actualVal = cdeclFunction.getReturnValAsFloat();

		EXPECT_EQ(1.0f, actualVal);
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral1)
	{
		CdeclFunction<char> cdeclFunction(&cdeclFooGeneral);

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->call();

		EXPECT_EQ((char)1, (char)dynamicCaller->getReturnValAsInt8());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral2)
	{
		CdeclFunction<short, int> cdeclFunction(&cdeclFooGeneral);

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->call();

		EXPECT_EQ((short)1, (short)dynamicCaller->getReturnValAsInt16());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral3)
	{
		CdeclFunction<float, int, char> cdeclFunction(&cdeclFooGeneral);

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->call();

		EXPECT_EQ(1.0f, dynamicCaller->getReturnValAsFloat());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral4)
	{
		CdeclFunction<int, int, char, const double&> cdeclFunction(&cdeclFooGeneral);

		double p3 = 3.0f;

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->call();

		EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral5)
	{
		CdeclFunction<long, int, char, const double&, const float&> cdeclFunction(&cdeclFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->call();

		EXPECT_EQ(1, dynamicCaller->getReturnValAsInt32());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral6)
	{
		CdeclFunction<long long, int, char, const double&, const float&, unsigned int> cdeclFunction(&cdeclFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->call();

		EXPECT_TRUE(1000000000000000 == dynamicCaller->getReturnValAsInt64());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral7)
	{
		CdeclFunction<double, int, char, const double&, const float&, unsigned int, short> cdeclFunction(&cdeclFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->pushParam((void*)6);
		dynamicCaller->call();

		EXPECT_EQ(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral8)
	{
		CdeclFunction<unsigned short, int, char, const double&, const float&, unsigned int, short, long> cdeclFunction(&cdeclFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->pushParam((void*)6);
		dynamicCaller->pushParam((void*)7);
		dynamicCaller->call();

		EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());
	}

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionGeneral9)
	{
		CdeclFunction<unsigned short, int, char, const double&, const float&, unsigned int, short, long> cdeclFunction(&cdeclFooGeneral);

		double p3 = 3.0f;
		float p4 = 4.0f;

		DFunction* dynamicCaller = &cdeclFunction;
		dynamicCaller->pushParam((void*)1);
		dynamicCaller->pushParam((void*)'2');
		dynamicCaller->pushParam((void*)&p3);
		dynamicCaller->pushParam((void*)&p4);
		dynamicCaller->pushParam((void*)5);
		dynamicCaller->pushParam((void*)6);
		dynamicCaller->pushParam((void*)7);
		dynamicCaller->call();

		EXPECT_EQ(1, (int)dynamicCaller->getReturnValAsInt16());

		DFunction* pNewInsance = dynamicCaller->clone();
		pNewInsance->call();

		EXPECT_EQ(1, (int)pNewInsance->getReturnValAsInt16());
	}

	/*static double sin(const double& v) {
		return ::sin(v);
	}*/

	/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
	TEST(CdeclFunction, testCdeclunctionExternal1)
	{
		/*CdeclFunction<double, double> cdeclFunction(cdeclForward<double, double>();

		double p = 3.0f;

		double expect = ::sin(p);

		cdeclFunction.pushParam(&p);

		cdeclFunction.call();

		double val1 = cdeclFunction.getReturnValAsDouble();
		const void* val2 = cdeclFunction.getReturnValAsReference();

		EXPECT_EQ(expect, val1);
		EXPECT_EQ(expect, *((double*)val2));*/

		double p = 3.0f;
		double val1 = cdeclForward<double, double>(&sin, p);
		double expect = ::sin(p);
		EXPECT_EQ(expect, val1);
	}
		

	TEST(CdeclFunction, testCdeclunction2_1)
	{
		float p1 = 1.0f;
		float p2 = 2.0f;
		int p3 = 3;
		CdeclFunction2<float, const float&, const float&, int> cdelFunction(cdeclFoof);
		DFunction2* nativeFunction2 = &cdelFunction;

		float returnVal;
		void* params[] = {&p1, &p2, (void*)(size_t)p3};
		nativeFunction2->call(&returnVal, params);
		EXPECT_EQ(cdeclFoof(p1, p2, p3), returnVal);
	}
#ifndef USE_EXTERNAL_PARAMS_ONLY
	TEST(CdeclFunction, testCdeclunction2_2)
	{
		float fixedParam = 1.0f;
		float p2 = 2.0f;
		int p3 = 3;
		CdeclFunction2<float, const float&, const float&, int> cdelFunction(cdeclFoof);
		cdelFunction.bind<float*>(&fixedParam);

		DFunction2* nativeFunction2 = &cdelFunction;

		float returnVal;
		void* params[] = { &p2, (void*)(size_t)p3 };
		nativeFunction2->call(&returnVal, params);
		EXPECT_EQ(cdeclFoof(fixedParam, p2, p3), returnVal);
	}
#endif
	static double sum(int a, float b) {
		return (double)(a + b);
	}

#pragma pack(push)
#pragma pack(1)
	struct SampleStruct {
		int a;
		double b;
	};
#pragma pack(pop)

	static void sum2(SampleStruct a, int b) {
		a.b += b;
	}

	static void sum21(SampleStruct a, int b, SampleStruct& c) {
		c.b = a.b + b;
	}

	TEST(CdeclFunction, testCdeclunction3_0)
	{
		typedef MemberTypeInfo<0, sizeof(void*), int, SampleStruct, float> AMemberTypeInfo;
		int offset = 0;
		EXPECT_EQ(offset, AMemberTypeInfo::offset<0>());

		offset += sizeof(void*);
		EXPECT_EQ(offset, AMemberTypeInfo::offset<1>());

		constexpr auto alignedSizeOfStruct = sizeof(void*) == 8 ? 16 : 12;
		offset += alignedSizeOfStruct;

		EXPECT_EQ(offset, AMemberTypeInfo::offset<2>());
		offset += sizeof(void*);

		EXPECT_EQ((int)sizeof(void*), AMemberTypeInfo::getSize<0>());
		EXPECT_EQ(alignedSizeOfStruct, AMemberTypeInfo::getSize<1>());
		EXPECT_EQ((int)sizeof(void*), AMemberTypeInfo::getSize<2>());
		EXPECT_EQ(offset, AMemberTypeInfo::totalSize());
	}

	TEST(CdeclFunction, testCdeclunction3_1)
	{
		int p1 = 123;
		float p2 = 456.0f;
		CdelFunction3<double, int, float> cdelFunction(sum);
		DFunction2* nativeFunction2 = &cdelFunction;

		char paramData[sizeof(void*) * 2];
		// argument 1
		*((int*)&paramData[0]) = p1;
		// argument 2
		*((float*)&paramData[sizeof(void*)]) = p2;

		double returnVal;

		nativeFunction2->call(&returnVal, (void**)&paramData[0]);
		EXPECT_EQ(sum(p1 , p2), returnVal);
	}

	TEST(CdeclFunction, testCdeclunction3_2)
	{
		SampleStruct p1 = { 456, 789.0f };
		int p2 = 123;
		CdelFunction3<void, SampleStruct, int> cdelFunction(sum2);
		DFunction2* nativeFunction2 = &cdelFunction;

		constexpr auto alignedSizeOfStruct = sizeof(void*) == 8 ? 16 : 12;

		char paramData[sizeof(void*) + alignedSizeOfStruct];
		// argument 1
		*((SampleStruct*)&paramData[0]) = p1;
		// argument 2
		*((int*)&paramData[alignedSizeOfStruct]) = p2;
		nativeFunction2->call(nullptr, (void**)&paramData[0]);
	}

	TEST(CdeclFunction, testCdeclunction3_3)
	{
		SampleStruct p1 = { 456, 789.0f };
		int p2 = 123;
		SampleStruct p3;
		constexpr auto alignedSizeOfStruct = sizeof(void*) == 8 ? 16 : 12;

		typedef CdelFunction3<void, SampleStruct, int, SampleStruct*> AFunc;

		AFunc cdelFunction((AFunc::Fx)sum21);

		DFunction2* nativeFunction2 = &cdelFunction;

		char paramData[sizeof(void*) * 2 + alignedSizeOfStruct];
		// argument 1
		*((SampleStruct*)&paramData[0]) = p1;
		// argument 2
		*((int*)&paramData[alignedSizeOfStruct]) = p2;
		// argument 3
		*((SampleStruct**)&paramData[alignedSizeOfStruct + sizeof(void*)]) = &p3;

		nativeFunction2->call(nullptr, (void**)&paramData[0]);

		SampleStruct p31;
		sum21(p1, p2, p31);

		EXPECT_EQ(p31.b, p3.b);
	}


	static double foo(int& p1, double p2) {
		return p1 + p2;
	}

	static void doNothing1() {

	}

	static int doNothing2() {
		return 123;
	}

	TEST(CdeclFunction, testCdeclunction3_new_1)
	{
		double p2 = 789;
		int p1 = 123;
		CdelFunction3<double, int&, double> cdelFunction(foo);
		DFunction2* nativeFunction2 = &cdelFunction;

		char paramData[sizeof(void*) + sizeof(double)];
		// argument 1
		*((int**)&paramData[0]) = &p1;
		// argument 2
		*((double*)&paramData[sizeof(void*)]) = p2;

		double ret;
		nativeFunction2->call(&ret, (void**)&paramData[0]);

		EXPECT_EQ(foo(p1, p2), ret);
	}

	TEST(CdeclFunction, testCdeclunction3_new_2)
	{	CdelFunction3<void> cdelFunction(doNothing1);
		DFunction2* nativeFunction2 = &cdelFunction;
		nativeFunction2->call(nullptr, nullptr);
	}

	TEST(CdeclFunction, testCdeclunction3_new_3)
	{
		CdelFunction3<int> cdelFunction(doNothing2);
		DFunction2* nativeFunction2 = &cdelFunction;

		int ret;
		nativeFunction2->call(&ret, nullptr);
		EXPECT_EQ(doNothing2(), ret);
	}
}
