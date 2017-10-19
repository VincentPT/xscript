#include "stdafx.h"
#include "CppUnitTest.h"
#include <functional>
#include "function\MemberFunction.hpp"
#include "TemplateForTest.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ffscriptUT
{
	TEST_CLASS(MemberFunctionUT)
	{
	public:

		float memberVariable;

		MemberFunctionUT() :memberVariable(1.234567f){

		}

		void memberFoo() {
			char sbuff[128];
			sprintf_s(sbuff, __FUNCTION__);
			PRINT_TEST_MESSAGE(sbuff);
		}
	
		void memberFoo(const float& a) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, "a=%f", a);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, "a=%f, b=%f", a, b);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b, int c) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d)", a, b, c);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b, int c, char d) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c)", a, b, c, d);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b, int c, char d, const double& e) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf)", a, b, c, d, e);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p)", a, b, c, d, e, f);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d)", a, b, c, d, e, f, g);
			PRINT_TEST_MESSAGE(sbuff);
		}

		void memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
			PRINT_TEST_MESSAGE(sbuff);
		}

		float memberFoof() {
			char sbuff[128];
			sprintf_s(sbuff, __FUNCTION__);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, "a=%f", a);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, "a=%f, b=%f", a, b);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b, int c) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d)", a, b, c);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b, int c, char d) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c)", a, b, c, d);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b, int c, char d, const double& e) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf)", a, b, c, d, e);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p)", a, b, c, d, e, f);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d)", a, b, c, d, e, f, g);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		float memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%f, %f, %d, %c, %lf, %p, %d, %d)", a, b, c, d, e, f, g, h);
			PRINT_TEST_MESSAGE(sbuff);

			return memberVariable;
		}

		char MemberFooGeneral() {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			PRINT_TEST_MESSAGE(__FUNCTION__);
			return 1;
		}

		short MemberFooGeneral(int a) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d)", a);
			PRINT_TEST_MESSAGE(sbuff);
			return 1;
		}

		float MemberFooGeneral(int a, char b) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d, %c)", a, b);
			PRINT_TEST_MESSAGE(sbuff);
			return 1.0f;
		}

		int MemberFooGeneral(int a, char b, const double& c) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf)", a, b, c);
			PRINT_TEST_MESSAGE(sbuff);
			return 1;
		}

		long MemberFooGeneral(int a, char b, const double& c, const float& d) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f)", a, b, c, d);
			PRINT_TEST_MESSAGE(sbuff);
			return 1;
		}

		long long MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u)", a, b, c, d, e);
			PRINT_TEST_MESSAGE(sbuff);
			return 1000000000000000;
		}

		double MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u, %d)", a, b, c, d, e, f);
			PRINT_TEST_MESSAGE(sbuff);
			return 1000000000000000.0;
		}

		unsigned short MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f", memberVariable);
			PRINT_TEST_MESSAGE(sbuff);
			sprintf_s(sbuff, __FUNCTION__"(%d, %c, %lf, %f, %u, %d, %d)", a, b, c, d, e, f, g);
			PRINT_TEST_MESSAGE(sbuff);
			return 1;
		}

		TEST_METHOD(testMemberFunctionVoid1)
		{
			MFunction<void, MemberFunctionUT> memberFunction(this, &MemberFunctionUT::memberFoo);
			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid2)
		{
			MFunction<void, MemberFunctionUT, const float&> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;		

			pushParams(&memberFunction, &forRefValue1);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid3)
		{
			MFunction<void, MemberFunctionUT, const float&, const float&> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid4)
		{
			MFunction<void, MemberFunctionUT, const float&, const float&, int> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid5)
		{
			MFunction<void, MemberFunctionUT, const float&, const float&, int, char> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid6)
		{
			MFunction<void, MemberFunctionUT, const float&,
				const float&, int, char, const double&> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid7)
		{
			MFunction<void, MemberFunctionUT, const float&,
				const float&, int, char, const double&, void*> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid8)
		{
			MFunction<void, MemberFunctionUT, const float&,
				const float&, int, char, const double&, void*, short> memberFunction(this, &MemberFunctionUT::memberFoo);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionVoid9)
		{
			MFunction<void, MemberFunctionUT, const float&,
				const float&, int, char, const double&, void*, short, int> memberFunction(this, &MemberFunctionUT::memberFoo);
			
			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);

			memberFunction.call();
		}

		TEST_METHOD(testMemberFunctionFloat1)
		{
			MFunction<float, MemberFunctionUT> memberFunction(this, &MemberFunctionUT::memberFoof);

			float expectedVal = 1.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat2)
		{
			MFunction<float, MemberFunctionUT, const float&> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;

			pushParams(&memberFunction, &forRefValue1);

			float expectedVal = 2.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat3)
		{
			MFunction<float, MemberFunctionUT, const float&, const float&> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2);

			float expectedVal = 3.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat4)
		{
			MFunction<float, MemberFunctionUT, const float&, const float&, int> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3);

			float expectedVal = 4.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat5)
		{
			MFunction<float, MemberFunctionUT, const float&, const float&, int, char> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4);

			float expectedVal = 5.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat6)
		{
			MFunction<float, MemberFunctionUT, const float&,
				const float&, int, char, const double&> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3);

			float expectedVal = 6.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat7)
		{
			MFunction<float, MemberFunctionUT, const float&,
				const float&, int, char, const double&, void*> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6);

			float expectedVal = 7.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat8)
		{
			MFunction<float, MemberFunctionUT, const float&,
				const float&, int, char, const double&, void*, short> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7);

			float expectedVal = 8.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		TEST_METHOD(testMemberFunctionFloat9)
		{
			MFunction<float, MemberFunctionUT, const float&,
				const float&, int, char, const double&, void*, short, int> memberFunction(this, &MemberFunctionUT::memberFoof);

			float forRefValue1 = 1.0f;
			float forRefValue2 = 2.0f;
			int p3 = 3;
			char p4 = '4';
			double forRefValue3 = 5.0f;
			void* p6 = (void*)6;
			short p7 = 7;
			int p8 = 8;

			pushParams(&memberFunction, &forRefValue1, &forRefValue2, p3, p4, &forRefValue3, p6, p7, p8);

			float expectedVal = 9.0f;
			memberVariable = expectedVal;

			memberFunction.call();

			float actualVal = memberFunction.getReturnValAsFloat();

			Assert::AreEqual(expectedVal, actualVal);
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral1)
		{
			MFunction<char, MemberFunctionUT> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->call();

			Assert::AreEqual((char)1, (char)dynamicCaller->getReturnValAsInt8());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral2)
		{
			MFunction<short, MemberFunctionUT, int> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->call();

			Assert::AreEqual((short)1, (short)dynamicCaller->getReturnValAsInt16());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral3)
		{
			MFunction<float, MemberFunctionUT, int, char> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->call();

			Assert::AreEqual(1.0f, dynamicCaller->getReturnValAsFloat());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral4)
		{
			MFunction<int, MemberFunctionUT, int, char, const double&> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			double p3 = 3.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->call();

			Assert::AreEqual(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral5)
		{
			MFunction<long, MemberFunctionUT, int, char, const double&, const float&> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->call();

			Assert::AreEqual(1, dynamicCaller->getReturnValAsInt32());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral6)
		{
			MFunction<long long, MemberFunctionUT, int, char, const double&, const float&, unsigned int> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
			dynamicCaller->pushParam((void*)1);
			dynamicCaller->pushParam((void*)'2');
			dynamicCaller->pushParam((void*)&p3);
			dynamicCaller->pushParam((void*)&p4);
			dynamicCaller->pushParam((void*)5);
			dynamicCaller->call();

			Assert::IsTrue(1000000000000000 == dynamicCaller->getReturnValAsInt64());
		}

		/*test two params funtion void(const float&, const float&, int, char, const double&, void*, short, int>) - static call*/
		TEST_METHOD(testMemberFunctionGeneral7)
		{
			MFunction<double, MemberFunctionUT, int, char, const double&, const float&, unsigned int, short> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
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
		TEST_METHOD(testMemberFunctionGeneral8)
		{
			MFunction<unsigned short, MemberFunctionUT, int, char, const double&, const float&, unsigned int, short, long> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
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
		TEST_METHOD(testMemberFunctionGeneral9)
		{
			MFunction<unsigned short, MemberFunctionUT, int, char, const double&, const float&, unsigned int, short, long> memberFunction(this, &MemberFunctionUT::MemberFooGeneral);

			double p3 = 3.0f;
			float p4 = 4.0f;

			DFunction* dynamicCaller = &memberFunction;
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

		TEST_METHOD(testMemberFunctionCdecl1)
		{
			Function<double, double> function(::sin);
			double p = 1.0f;
			double expect = ::sin(p);

			function.pushParam(&p);
			function.call();

			double val1 = function.getReturnValAsDouble();
			Assert::AreEqual(expect, val1);
		}

		static double cdelfoo(const double& d) {
			return d;
		}

		static double _stdcall stdfoo(double d) {
			return d;
		}

		static double _stdcall stdfoo(double a, const double& b) {
			return a + b;
		}

		TEST_METHOD(testMemberFunctionCdecl2)
		{
			Function<double, const double&> function(cdelfoo);
			double p = 1.0f;
			double expect = cdelfoo(p);

			function.pushParam(&p);
			function.call();

			double val1 = function.getReturnValAsDouble();
			Assert::AreEqual(expect, val1);
		}

		TEST_METHOD(testMemberFunctionStd1)
		{
			SFunction<double, double> function(stdfoo);
			double p = 1.0f;
			double expect = stdfoo(p);

			function.pushParam(&p);
			function.call();

			double val1 = function.getReturnValAsDouble();
			Assert::AreEqual(expect, val1);
		}

		TEST_METHOD(testMemberFunctionStd2)
		{
			SFunction<double, double, const double&> function(stdfoo);
			double p = 1.0f;
			double q = 1.0f;
			double expect = stdfoo(p, q);

			function.pushParam(&p);
			function.pushParam(&q);
			function.call();

			double val1 = function.getReturnValAsDouble();
			Assert::AreEqual(expect, val1);
		}

		void memberFoo(float a) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f, a=%f", memberVariable, a);
			PRINT_TEST_MESSAGE(sbuff);
		}

		double memberFoo(float a, double b) {
			char sbuff[128];
			sprintf_s(sbuff, "memberVariable=%f, a=%f", memberVariable, a);
			PRINT_TEST_MESSAGE(sbuff);

			return b + a + memberVariable;
		}

		TEST_METHOD(testMemberFunctionMember1)
		{
			MFunctionW <void, MemberFunctionUT, float> function(this, &MemberFunctionUT::memberFoo);
			double p = 1.0f;
			function.pushParam(&p);
			function.call();
		}

		TEST_METHOD(testMemberFunctionMember2)
		{
			MFunctionW <double, MemberFunctionUT, float, double> function(this, &MemberFunctionUT::memberFoo);
			float p = 1.0f;
			double q = 1.0f;
			double expect = memberFoo(p, q);

			function.pushParam(&p);
			function.pushParam(&q);
			function.call();

			double val1 = function.getReturnValAsDouble();
			Assert::AreEqual(expect, val1);
		}

		TEST_METHOD(testMemberFunctionMember3)
		{
			MFunctionW <double, MemberFunctionUT, float, double> function(this, &MemberFunctionUT::memberFoo);
			float p = 1.0f;
			double q = 1.0f;
			double expect = memberFoo(p, q);

			function.pushParam(&p);
			function.pushParam(&q);
			function.call();

			DFunction* newInstance = function.clone();
			newInstance->call();
			double val2 = newInstance->getReturnValAsDouble();

			double val1 = function.getReturnValAsDouble();
			Assert::AreEqual(expect, val1);
			Assert::AreEqual(expect, val2);
		}
	};
}
