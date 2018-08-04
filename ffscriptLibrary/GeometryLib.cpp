#include "stdafx.h"
#include "GeometryLib.h"

#include "function\CdeclFunction2.hpp"
#include "function\MemberFunction2.hpp"
#include "BasicFunctionFactory.hpp"
#include "DynamicFunctionFactory.h"
#include "BasicType.h"
#include "ScriptCompiler.h"
#include "FunctionRegisterHelper.h"
#include "BasicFunction.h"

namespace ffscript {
	Point operator-(const Point& P) {
		Point X = { -P.x, -P.y };
		return X;
	}

	Point operator+(const Point& P, const Point& Q) {
		Point X = { P.x + Q.x, P.y + Q.y };
		return X;
	}

	const Point& operator+=(Point& P, const Point& Q) {
		P.x += Q.x;
		P.y += Q.y;

		return P;
	}

	Point operator-(const Point& P, const Point& Q) {
		Point X = { P.x - Q.x, P.y - Q.y };
		return X;
	}

	const Point& operator-=(Point& P, const Point& Q) {
		P.x -= Q.x;
		P.y -= Q.y;

		return P;
	}

	Point operator*(const Point& P, float k) {
		Point X = { P.x * k, P.y * k };
		return X;
	}

	const Point& operator*=(Point& P, float k) {
		P.x *= k;
		P.y *= k;

		return P;
	}

	Point operator/(const Point& P, float k) {
		Point X = { P.x / k, P.y / k };
		return X;
	}

	const Point& operator/=(Point& P, float k) {
		P.x /= k;
		P.y /= k;

		return P;
	}

	float operator*(const Point& P, const Point& Q) {
		return P.x * Q.y - P.y * Q.x;
	}

	void includeGeoLibToCompiler(ScriptCompiler* scriptCompiler) {
		FunctionRegisterHelper helper(scriptCompiler);

		auto& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();

		ScriptType typeFloat(basicTypes.TYPE_FLOAT, scriptCompiler->getType(basicTypes.TYPE_FLOAT));

		// register struct Point must be same as RawPoint
		StructClass* pointStruct = new StructClass(scriptCompiler, "Point");
		pointStruct->addMember(typeFloat, "x");
		pointStruct->addMember(typeFloat, "y");
		auto iTypePoint = scriptCompiler->registStruct(pointStruct);
		ScriptType typePoint(iTypePoint, scriptCompiler->getType(iTypePoint).c_str());

		// register struct Ray must be same as RawRay
		StructClass* rayStruct = new StructClass(scriptCompiler, "Ray");
		rayStruct->addMember(typePoint, "start");
		rayStruct->addMember(typePoint, "dir");
		auto iTypeRay = scriptCompiler->registStruct(rayStruct);

		helper.registPredefinedOperators("+", "Point&,Point&", "Point", createFunctionCdecl<Point, const Point&, const Point&>(operator+));
		helper.registPredefinedOperators("-", "Point&,Point&", "Point", createFunctionCdecl<Point, const Point&, const Point&>(operator-));
		helper.registPredefinedOperators("*", "Point&,float", "Point",createFunctionCdecl<Point, const Point&, float>(operator*));
		helper.registPredefinedOperators("/", "Point&,float", "Point", createFunctionCdecl<Point, const Point&, float>(operator/));
		
		helper.registPredefinedOperators("+=", "Point&,Point&", "Point&", createFunctionCdecl<const Point&, Point&, const Point&>(operator+=));
		helper.registPredefinedOperators("-=", "Point&,Point&", "Point&", createFunctionCdecl<const Point&, Point&, const Point&>(operator-=));
		helper.registPredefinedOperators("*=", "Point&,float", "&Point", createFunctionCdecl<const Point&, Point&, float>(operator*=));
		helper.registPredefinedOperators("/=", "Point&,float", "&Point", createFunctionCdecl<const Point&, Point&, float>(operator/=));

		// dot product
		helper.registPredefinedOperators("*", "Point&,Point&", "float", createFunctionCdecl<float, const Point&, const Point&>(operator*));
		// reverser direction
		helper.registPredefinedOperators("-", "Point&", "Point", createFunctionCdecl<Point, const Point&>(operator-));
	}
}