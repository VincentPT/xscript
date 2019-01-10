/******************************************************************
* File:        MethodContainerUT.cpp
* Description: Test cases invoking member functions using MFunction.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "../ffscriptUT/fftest.hpp"
#include "../ffscriptUT/TemplateForTest.hpp"
#include <math.h>

using namespace std;

namespace ffscriptUT
{
	FF_TEST_CLASS(MethodContainer)
	{
	protected:
		float memberVariable;
		MethodContainer();
	public:
		void memberFoo();

		void memberFoo(const float& a);

		void memberFoo(const float& a, const float& b);

		void memberFoo(const float& a, const float& b, int c);

		void memberFoo(const float& a, const float& b, int c, char d);

		void memberFoo(const float& a, const float& b, int c, char d, const double& e);

		void memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f);

		void memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g);

		void memberFoo(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h);

		float memberFoof();

		float memberFoof(const float& a);

		float memberFoof(const float& a, const float& b);

		float memberFoof(const float& a, const float& b, int c);

		float memberFoof(const float& a, const float& b, int c, char d);

		float memberFoof(const float& a, const float& b, int c, char d, const double& e);

		float memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f);

		float memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g);

		float memberFoof(const float& a, const float& b, int c, char d, const double& e, void* f, short g, int h);

		char MemberFooGeneral();

		short MemberFooGeneral(int a);

		float MemberFooGeneral(int a, char b);

		int MemberFooGeneral(int a, char b, const double& c);

		long MemberFooGeneral(int a, char b, const double& c, const float& d);

		long long MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e);

		double MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f);

		unsigned short MemberFooGeneral(int a, char b, const double& c, const float& d, unsigned int e, short f, long g);

		void memberFoo(float a);

		double memberFoo(float a, double b);
	};
}
