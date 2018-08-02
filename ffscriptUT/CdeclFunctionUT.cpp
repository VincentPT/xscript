#include "stdafx.h"
#include "CppUnitTest.h"
#include <functional>
#include "function\CdeclFunction.hpp"
#include "function\CdeclFunction2.hpp"
#include "function\CdeclFunction3.hpp"
#include "TemplateForTest.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

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

	TEST_CLASS(CdeclFunctionUT)
	{
	public:		
		TEST_METHOD(testCdeclunctionVoid1)
		{
			CdeclFunction<void> cdeclFunction(&cdeclFoo);
			cdeclFunction.call();
		}

		TEST_METHOD(testCdeclunctionVoid2)
		{
			CdeclFunction<void, const float&> cdeclFunction(&cdeclFoo);

			float forRefValue1 = 1.0f;		

			pushParams(&cdeclFunction, &forRefValue1);

			cdeclFunction.call();
		}

		TEST_METHOD(testCdeclunctionVoid3)
		{
			CdeclFunction<void, const float&, const float&> cdeclFunction(&cdeclFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			pushParams(&cdeclFunction, &forRefValue1, &forRefValue2);

			cdeclFunction.call();
		}

		TEST_METHOD(testCdeclunctionVoid4)
		{
			CdeclFunction<void, const float&, const float&, int> cdeclFunction(&cdeclFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3);

			cdeclFunction.call();
		}

		TEST_METHOD(testCdeclunctionVoid5)
		{
			CdeclFunction<void, const float&, const float&, int, char> cdeclFunction(&cdeclFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4);

			cdeclFunction.call();
		}

		TEST_METHOD(testCdeclunctionVoid6)
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

		TEST_METHOD(testCdeclunctionVoid7)
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

		TEST_METHOD(testCdeclunctionVoid8)
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

		TEST_METHOD(testCdeclunctionVoid9)
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

		TEST_METHOD(testCdeclunctionFloat1)
		{
			CdeclFunction<float> cdeclFunction(&cdeclFoof);
			cdeclFunction.call();

			float actualVal = cdeclFunction.getReturnValAsFloat();

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat2)
		{
			CdeclFunction<float, const float&> cdeclFunction(&cdeclFoof);

			float forRefValue1 = 1.0f;

			pushParams(&cdeclFunction, &forRefValue1);

			cdeclFunction.call();

			float actualVal = cdeclFunction.getReturnValAsFloat();

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat3)
		{
			CdeclFunction<float, const float&, const float&> cdeclFunction(&cdeclFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			pushParams(&cdeclFunction, &forRefValue1, &forRefValue2);

			float expectedVal = 3.0f;

			cdeclFunction.call();

			float actualVal = cdeclFunction.getReturnValAsFloat();

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat4)
		{
			CdeclFunction<float, const float&, const float&, int> cdeclFunction(&cdeclFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3);
			cdeclFunction.call();

			float actualVal = cdeclFunction.getReturnValAsFloat();

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat5)
		{
			CdeclFunction<float, const float&, const float&, int, char> cdeclFunction(&cdeclFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			pushParams(&cdeclFunction, &forRefValue1, &forRefValue2, p3, p4);

			cdeclFunction.call();

			float actualVal = cdeclFunction.getReturnValAsFloat();

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat6)
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

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat7)
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

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat8)
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

			Assert::AreEqual(1.0f, actualVal);
		}

		TEST_METHOD(testCdeclunctionFloat9)
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

			Assert::AreEqual(1.0f, actualVal);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral1)
		{
			CdeclFunction<char> cdeclFunction(&cdeclFooGeneral);

			DFunction* dynamicCaller = &cdeclFunction;
			dynamicCaller->call();

			Assert::AreEqual((char)1, (char)dynamicCaller->getReturnValAsInt8());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral2)
		{
			CdeclFunction<short, int> cdeclFunction(&cdeclFooGeneral);

			DFunction* dynamicCaller = &cdeclFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->call();

			Assert::AreEqual((short)1, (short)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral3)
		{
			CdeclFunction<float, int, char> cdeclFunction(&cdeclFooGeneral);

			DFunction* dynamicCaller = &cdeclFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->call();

			Assert::AreEqual(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral4)
		{
			CdeclFunction<int, int, char, const double&> cdeclFunction(&cdeclFooGeneral);

			double p3 = 3.0f;

			DFunction* dynamicCaller = &cdeclFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->call();

			Assert::AreEqual(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral5)
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

			Assert::AreEqual(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral6)
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

			Assert::IsTrue(1000000000000000 == dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral7)
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

			Assert::AreEqual(1000000000000000.0, dynamicCaller->getReturnValAsDouble());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral8)
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

			Assert::AreEqual(1, (int)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionGeneral9)
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

			Assert::AreEqual(1, (int)dynamicCaller->getReturnValAsInt16());

			DFunction* pNewInsance = dynamicCaller->clone();
			pNewInsance->call();

			Assert::AreEqual(1, (int)pNewInsance->getReturnValAsInt16());
		}

		/*static double sin(const double& v) {
			return ::sin(v);
		}*/

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testCdeclunctionExternal1)
		{
			/*CdeclFunction<double, double> cdeclFunction(cdeclForward<double, double>();

			double p = 3.0f;

			double expect = ::sin(p);

			cdeclFunction.pushParam(&p);

			cdeclFunction.call();

			double val1 = cdeclFunction.getReturnValAsDouble();
			const void* val2 = cdeclFunction.getReturnValAsReference();

			Assert::AreEqual(expect, val1);
			Assert::AreEqual(expect, *((double*)val2));*/

			double p = 3.0f;
			double val1 = cdeclForward<double, double>(&sin, p);
			double expect = ::sin(p);
			Assert::AreEqual(expect, val1);
		}
		

		TEST_METHOD(testCdeclunction2_1)
		{
			float p1 = 1.0f;
			float p2 = 2.0f;
			int p3 = 3;
			CdeclFunction2<float, const float&, const float&, int> cdelFunction(cdeclFoof);
			DFunction2* nativeFunction2 = &cdelFunction;

			float returnVal;
			void* params[] = {&p1, &p2, (void*)(size_t)p3};
			nativeFunction2->call(&returnVal, params);
			Assert::AreEqual(cdeclFoof(p1, p2, p3), returnVal);
		}
#ifndef USE_EXTERNAL_PARAMS_ONLY
		TEST_METHOD(testCdeclunction2_2)
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
			Assert::AreEqual(cdeclFoof(fixedParam, p2, p3), returnVal);
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

		TEST_METHOD(testCdeclunction3_0)
		{
			typedef MemberTypeInfo<0, sizeof(void*), int, SampleStruct, float> AMemberTypeInfo;
			int offset = 0;
			Assert::AreEqual(offset, AMemberTypeInfo::getOffset<0>());

			offset += sizeof(void*);
			Assert::AreEqual(offset, AMemberTypeInfo::getOffset<1>());

			offset += 2 * sizeof(void*);
			Assert::AreEqual(offset, AMemberTypeInfo::getOffset<2>());

			offset += sizeof(void*);

			Assert::AreEqual((int)sizeof(void*), AMemberTypeInfo::getSize<0>());
			Assert::AreEqual((int)(2 * sizeof(void*)), AMemberTypeInfo::getSize<1>());
			Assert::AreEqual((int)sizeof(void*), AMemberTypeInfo::getSize<2>());
			Assert::AreEqual(32, AMemberTypeInfo::totalSize());
		}

		TEST_METHOD(testCdeclunction3_1)
		{
			int p1 = 123;
			float p2 = 456.0f;
			CCdelFunction3<double, int, float> cdelFunction(sum);
			DFunction2* nativeFunction2 = &cdelFunction;

			char paramData[sizeof(void*) * 2];
			// argument 1
			*((int*)&paramData[0]) = p1;
			// argument 2
			*((float*)&paramData[sizeof(void*)]) = p2;

			double returnVal;

			nativeFunction2->call(&returnVal, (void**)&paramData[0]);
			Assert::AreEqual(sum(p1 , p2), returnVal);
		}

		TEST_METHOD(testCdeclunction3_2)
		{
			SampleStruct p1 = { 456, 789.0f };
			int p2 = 123;
			CCdelFunction3<void, SampleStruct, int> cdelFunction(sum2);
			DFunction2* nativeFunction2 = &cdelFunction;

			char paramData[sizeof(void*) * 3];
			// argument 1
			*((SampleStruct*)&paramData[0]) = p1;
			// argument 2
			*((int*)&paramData[2 * sizeof(void*)]) = p2;
			nativeFunction2->call(nullptr, (void**)&paramData[0]);
		}

		TEST_METHOD(testCdeclunction3_3)
		{
			SampleStruct p1 = { 456, 789.0f };
			int p2 = 123;
			SampleStruct p3;

			typedef CCdelFunction3<void, SampleStruct, int, SampleStruct*> AFunc;

			AFunc cdelFunction((AFunc::Fx)sum21);

			DFunction2* nativeFunction2 = &cdelFunction;

			char paramData[sizeof(void*) * 4];
			// argument 1
			*((SampleStruct*)&paramData[0]) = p1;
			// argument 2
			*((int*)&paramData[2 * sizeof(void*)]) = p2;
			// argument 3
			*((SampleStruct**)&paramData[3 * sizeof(void*)]) = &p3;

			nativeFunction2->call(nullptr, (void**)&paramData[0]);

			SampleStruct p31;
			sum21(p1, p2, p31);

			Assert::AreEqual(p31.b, p3.b);
		}
	};
}
