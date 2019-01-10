/******************************************************************
* File:        FunctionsForTest.cpp
* Description: Contains function for testing.
* Author:      Vincent Pham
*
* Copyright (c) 2019 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include <stdio.h>
#include "../ffscriptUT/fftest.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include "FunctionsForTest.h"

namespace ffscriptUT
{
	void funcFoo() {
		char sbuff[128];
		SPRINTF_S(sbuff, __FUNCTION__);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a) {

		char sbuff[128];
		SPRINTF_S(sbuff, "a=%f", a);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b) {

		char sbuff[128];
		SPRINTF_S(sbuff, "a=%f, b=%f", a, b);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b, int c) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d)", __FUNCTION__, a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c)", __FUNCTION__, a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf)", __FUNCTION__, a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p)", __FUNCTION__, a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d)", __FUNCTION__, a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
	}

	void funcFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d, %d)", __FUNCTION__, a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);
	}

	float funcFoof() {
		char sbuff[128];
		SPRINTF_S(sbuff, __FUNCTION__);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a) {

		char sbuff[128];
		SPRINTF_S(sbuff, "a=%f", a);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b) {

		char sbuff[128];
		SPRINTF_S(sbuff, "a=%f, b=%f", a, b);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b, int c) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d)", __FUNCTION__, a, b, c);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b, int c, char d) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c)", __FUNCTION__, a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b, int c, char d, const double& e) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf)", __FUNCTION__, a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b, int c, char d, const double& e, void* f) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p)", __FUNCTION__, a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d)", __FUNCTION__, a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	float funcFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h) {

		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%f, %f, %d, %c, %lf, %p, %d, %d)", __FUNCTION__, a, b, c, d, e, f, g, h);
		PRINT_TEST_MESSAGE(sbuff);

		return 1.0f;
	}

	char funcFooGeneral() {
		PRINT_TEST_MESSAGE(__FUNCTION__);
		return 1;
	}

	short funcFooGeneral(int a) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d)", __FUNCTION__, a);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	float funcFooGeneral(int a, char b) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d, %c)", __FUNCTION__, a, b);
		PRINT_TEST_MESSAGE(sbuff);
		return 1.0f;
	}

	int funcFooGeneral(int a, char b, const double& c) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d, %c, %lf)", __FUNCTION__, a, b, c);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long funcFooGeneral(int a, char b, const double& c, const float& d) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f)", __FUNCTION__, a, b, c, d);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	long long funcFooGeneral(int a, char b, const double& c, const float& d, unsigned int e) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f, %u)", __FUNCTION__, a, b, c, d, e);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000;
	}

	double funcFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f, %u, %d)", __FUNCTION__, a, b, c, d, e, f);
		PRINT_TEST_MESSAGE(sbuff);
		return 1000000000000000.0;
	}

	unsigned short funcFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g) {
		char sbuff[128];
		SPRINTF_S(sbuff, "%s(%d, %c, %lf, %f, %u, %d, %ld)", __FUNCTION__, a, b, c, d, e, f, g);
		PRINT_TEST_MESSAGE(sbuff);
		return 1;
	}

	double sum(int& p1, double p2) {
		return p1 + p2;
	}

	void doNothing() {
	}

	int returnConstant() {
		return 123;
	}
}
