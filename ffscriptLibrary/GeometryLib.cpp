/******************************************************************
* File:        GeometryLib.cpp
* Description: Implement some geometry functions and object and
*              implement an interface to import them into the
*              script compiler.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "GeometryLib.h"

#include "function/FunctionDelegate.hpp"
#include "function/CdeclFunction2.hpp"
#include "function/MemberFunction2.hpp"
#include "BasicFunctionFactory.hpp"
#include "DynamicFunctionFactory.h"
#include "BasicType.h"
#include "ScriptCompiler.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"
#include "Geometry.h"
#include "RawStringLib.h"

namespace ffscript {

	typedef GeneralLine<float> GeneralLineF;

	Point operator-(Point P) {
		Point X = { -P.x, -P.y };
		return X;
	}

	Point operator+(Point P, Point Q) {
		Point X = { P.x + Q.x, P.y + Q.y };
		return X;
	}

	const Point& operator+=(Point& P, Point Q) {
		P.x += Q.x;
		P.y += Q.y;

		return P;
	}

	Point operator-(Point P, Point Q) {
		Point X = { P.x - Q.x, P.y - Q.y };
		return X;
	}

	const Point& operator-=(Point& P, Point Q) {
		P.x -= Q.x;
		P.y -= Q.y;

		return P;
	}

	Point operator*(Point P, float k) {
		Point X = { P.x * k, P.y * k };
		return X;
	}

	const Point& operator*=(Point& P, float k) {
		P.x *= k;
		P.y *= k;

		return P;
	}

	Point operator/(Point P, float k) {
		Point X = { P.x / k, P.y / k };
		return X;
	}

	const Point& operator/=(Point& P, float k) {
		P.x /= k;
		P.y /= k;

		return P;
	}

	float operator*(Point P, Point Q) {
		return P.x * Q.y - P.y * Q.x;
	}

	void constructPoint(Point& p, float x, float y) {
		p.x = x;
		p.y = y;
	}

	void constructRay(Ray& r, Point p, Point u) {
		r.start = p;
		r.dir = u;
	}

	RawString pointToString(Point p) {
		std::wstring str(L"{");
		str.append(std::to_wstring(p.x));
		str.append(L", ");
		str.append(std::to_wstring(p.y));
		str.append(1, '}');

		RawString rws;
		constantConstructor(rws, str);

		return rws;
	}

	void includeGeoLibToCompiler(ScriptCompiler* scriptCompiler) {
		FunctionRegisterHelper helper(scriptCompiler);
		int functionId;

		auto& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();

		ScriptType typeFloat(basicTypes.TYPE_FLOAT, scriptCompiler->getType(basicTypes.TYPE_FLOAT));

		// register struct Point must be same as RawPoint
		StructClass* pointStruct = new StructClass(scriptCompiler, "Point");
		pointStruct->addMember(typeFloat, "x");
		pointStruct->addMember(typeFloat, "y");
		auto iTypePoint = scriptCompiler->registStruct(pointStruct);
		ScriptType typePoint(iTypePoint, scriptCompiler->getType(iTypePoint).c_str());

		functionId = helper.registFunction("String", "Point", createUserFunctionFactory(scriptCompiler, "String", pointToString));

		// register struct Ray must be same as RawRay
		StructClass* rayStruct = new StructClass(scriptCompiler, "Ray");
		rayStruct->addMember(typePoint, "start");
		rayStruct->addMember(typePoint, "dir");
		auto iTypeRay = scriptCompiler->registStruct(rayStruct);

		helper.registPredefinedOperators("+", "Point,Point", "Point", createFunctionCdecl<Point, Point, Point>(operator+));
		helper.registPredefinedOperators("-", "Point,Point", "Point", createFunctionCdecl<Point, Point, Point>(operator-));
		helper.registPredefinedOperators("*", "Point,float", "Point",createFunctionCdecl<Point, Point, float>(operator*));
		helper.registPredefinedOperators("/", "Point,float", "Point", createFunctionCdecl<Point, Point, float>(operator/));
		
		helper.registPredefinedOperators("+=", "Point&,Point", "Point&", createFunctionCdecl<const Point&, Point&, Point>(operator+=));
		helper.registPredefinedOperators("-=", "Point&,Point", "Point&", createFunctionCdecl<const Point&, Point&, Point>(operator-=));
		helper.registPredefinedOperators("*=", "Point&,float", "Point&", createFunctionCdecl<const Point&, Point&, float>(operator*=));
		helper.registPredefinedOperators("/=", "Point&,float", "Point&", createFunctionCdecl<const Point&, Point&, float>(operator/=));
		
		//auto functionId = helper.registFunction("Point", "ref Point, float, float", createUserFunctionFactory<void, Point&, float, float>(scriptCompiler, "void", constructPoint));
		//scriptCompiler->registConstructor(iTypePoint, functionId);

		//functionId = helper.registFunction("Ray", "ref Ray, Point, Point", createUserFunctionFactory<void, Ray&, Point, Point>(scriptCompiler, "void", constructRay));
		//scriptCompiler->registConstructor(iTypeRay, functionId);

		// dot product
		helper.registPredefinedOperators("*", "Point,Point", "float", createFunctionCdecl<float, Point, Point>(operator*));
		// reverser direction
		helper.registPredefinedOperators("-", "Point", "Point", createFunctionCdecl<Point, Point>(operator-));

		// general line
		auto generalLineTypeInt = scriptCompiler->registType("GeneralLine");
		scriptCompiler->setTypeSize(generalLineTypeInt, sizeof(GeneralLineF));

		functionId = helper.registFunction("GeneralLine", "ref GeneralLine, Point&, Point&", createUserFunctionFactoryContext<GeneralLineF, void, const Point&, const Point&>(scriptCompiler, "void", &GeneralLineF::build));
		scriptCompiler->registConstructor(generalLineTypeInt, functionId);

		functionId = helper.registFunction("compute", "GeneralLine&, Point&", createUserFunctionFactoryContext<GeneralLineF, float, const Point&>(scriptCompiler, "float", &GeneralLineF::compute));
		functionId = helper.registFunction("distance", "GeneralLine&, Point&", createUserFunctionFactoryContext<GeneralLineF, float, const Point&>(scriptCompiler, "float", &GeneralLineF::directionalDistance));
		
		functionId = helper.registFunction("angle", "Point&, Point&", createUserFunctionFactory<float, const Point&, const Point&>(scriptCompiler, "float", directionalAngle));

		functionId = helper.registFunction("intersect", "Point&,Point&,Point&,Point&,float&,float&", createUserFunctionFactory<bool,const Point&,const Point&,const Point&,const Point&, float*, float*>(scriptCompiler, "bool", Intersect2D_Lines));
		functionId = helper.registFunction("project", "Point&,Point&,Point&", createUserFunctionFactory<float, const Point&, const Point&, const Point&>(scriptCompiler, "float", projectPoint));

		setConstantMap(scriptCompiler, "PI", "float", 3.14159f);
	}
}