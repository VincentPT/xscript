/******************************************************************
* File:        MethodsForTest.cpp
* Description: Implement class containing methods for testing.
* Author:      Vincent Pham
*
* Copyright (c) 2019 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "MethodsForTest.h"

using namespace std;

namespace ffscriptUT
{
	MethodContainer::MethodContainer() :memberVariable(1.234567f) {

	}

	void MethodContainer::memberFoo() {
		char sbuff[128];
		SPRINTF_S(sbuff, __FUNCTION__);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "a=%f", a);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "a=%f, b=%f", a, b);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b, int c) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d)", __FUNCTION__, a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c)", __FUNCTION__, a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf)", __FUNCTION__, a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p)", __FUNCTION__, a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d)", __FUNCTION__, a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void MethodContainer::memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d, %d)", __FUNCTION__, a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);
	}

	float MethodContainer::memberFoof() {
		char sbuff[128];
		SPRINTF_S(sbuff, __FUNCTION__);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "a=%f", a);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "a=%f, b=%f", a, b);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b, int c) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d)", __FUNCTION__, a, b, c);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c)", __FUNCTION__, a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf)", __FUNCTION__, a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p)", __FUNCTION__, a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d)", __FUNCTION__, a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	float MethodContainer::memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d, %d)", __FUNCTION__, a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return memberVariable;
	}

	char MethodContainer::MemberFooGeneral() {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		PRINT_TEST_MESSAGE(__FUNCTION__);
		return 1;
	}

	short MethodContainer::MemberFooGeneral(int a) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d)", __FUNCTION__, a);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	float MethodContainer::MemberFooGeneral(int a, char b) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d, %c)", __FUNCTION__, a, b);
		PRINT_TEST_MESSAGE(sbuff);
		return 1.0f;
	}

	int MethodContainer::MemberFooGeneral(int a, char b, const double& c) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d, %c, %lf)", __FUNCTION__, a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long MethodContainer::MemberFooGeneral(int a, char b, const double& c, const float& d) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f)", __FUNCTION__, a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long long MethodContainer::MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f, %u)", __FUNCTION__, a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000;
	}

	double MethodContainer::MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f, %u, %d)", __FUNCTION__, a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000.0;
	}

	unsigned short MethodContainer::MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f", memberVariable);
		PRINT_TEST_MESSAGE(sbuff);
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f, %u, %d, %ld)", __FUNCTION__, a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	void MethodContainer::memberFoo(float a) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f, a=%f", memberVariable, a);
		PRINT_TEST_MESSAGE(sbuff);
	}

	double MethodContainer::memberFoo(float a, double b) {
		char sbuff[128];
		SPRINTF_S(sbuff, "memberVariable=%f, a=%f", memberVariable, a);
		PRINT_TEST_MESSAGE(sbuff);

		return b + a + memberVariable;
	}
}
