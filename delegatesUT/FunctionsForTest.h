/******************************************************************
* File:        FunctionsForTest.h
* Description: declare functions for testing.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

namespace ffscriptUT
{
	void funcFoo();

	void funcFoo(const float& a);
	void funcFoo(const float& a, const float& b);

	void funcFoo(const float& a, const float& b, int c);

	void funcFoo(const float& a, const float& b, int c, char d);

	void funcFoo(const float& a, const float& b, int c, char d, const double& e);

	void funcFoo(const float& a, const float& b, int c, char d, const double& e, void* f);

	void funcFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g);

	void funcFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h);

	float funcFoof();

	float funcFoof(const float& a);

	float funcFoof(const float& a, const float& b);

	float funcFoof(const float& a, const float& b, int c);

	float funcFoof(const float& a, const float& b, int c, char d);

	float funcFoof(const float& a, const float& b, int c, char d, const double& e);

	float funcFoof(const float& a, const float& b, int c, char d, const double& e, void* f);

	float funcFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g);

	float funcFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h);

	char funcFooGeneral();

	short funcFooGeneral(int a);

	float funcFooGeneral(int a, char b);

	int funcFooGeneral(int a, char b, const double& c);

	long funcFooGeneral(int a, char b, const double& c, const float& d);

	long long funcFooGeneral(int a, char b, const double& c, const float& d, unsigned int e);

	double funcFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f);

	unsigned short funcFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g);

	double sum(int& p1, double p2);

	void doNothing();

	int returnConstant();
}
