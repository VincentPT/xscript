/******************************************************************
* File:        Geometry.h
* Description: C++ template functions of some common geometry
*              functions using in scripting library.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

#define GEOMETRY_EPSILON 0.000001
#define MIN_POINT_DISTANCE 5.0f

template <class T>
bool Intersect2D_Lines(
	const T& P, const T& u,
	const T& Q, const T& v,
	float* t1, float* t2)
{
	double D = (float)( ((double)v.x)*u.y - ((double)u.x)*v.y);

	//lines are parallel
	if (std::abs(D) < GEOMETRY_EPSILON) return false;

	auto A = Q.x - P.x;
	auto B = Q.y - P.y;

	if (t1) {
		*t1 = (float)((v.x * B - v.y * A) / D);
	}
	if (t2) {
		*t2 = (float)((u.x * B - u.y * A) / D);
	}
	return true;
}

template <class T>
inline float lengthSquared(const T& u)
{
	return (float)(u.x*u.x + u.y*u.y);
}

template <class T>
inline double lengthSquaredd(const T& u)
{
	double res = (double)u.x;
	res *= u.x;
	res += ((double)u.y)*u.y;
	return res;
}

template <class T>
double cosAngle(const T& u, const T& v)
{
	auto dotProduct = (double)u.x;
	dotProduct *= v.x;
	dotProduct += ((double)u.y)*v.y;

	auto l1l2 = lengthSquaredd(u);
	l1l2 *= lengthSquaredd(v);
	auto cosValue = dotProduct / sqrt(l1l2);

	if (cosValue > 1.0) {
		cosValue = 1.0;
	}
	else if (cosValue < -1.0) {
		cosValue = -1.0;
	}

	return cosValue;
}

template <class T1, class T2>
T2 cosAngle(const T1& u, const T1& v, const T2& l1, const T2& l2)
{
	// u.x*v.x + u.y*v.)
	auto dotProduct = (T2)u.x;
	dotProduct *= v.x;
	dotProduct += ((T2)u.y)*v.y;
	auto cosValue = dotProduct / (l1*l2);

	if (cosValue > 1.0f) {
		cosValue = 1.0f;
	}
	else if (cosValue < -1.0f) {
		cosValue = -1.0f;
	}

	return cosValue;
}

template <class T>
float distanceFromPoint(const T& P, const T& u, const T& Q) {
	float A, B, C;

	A = (float)-u.y;
	B = (float)u.x;
	C = -A * P.x - B * P.y;

	return std::abs(A * Q.x + B * Q.y + C) / sqrt(A*A + B*B);
}

// compute value of a point against a line equation
// line equation is established by a point and a direction vector
// the normal vector follow right hand rule
template <class T>
auto compute(const T& P, const T& u, const T& Q) {
	auto A = -u.y;
	auto B = u.x;
	auto C = -A * P.x - B * P.y;

	return A * Q.x + B * Q.y + C;
}

template <class T>
struct GeneralLine {
	T A;
	T B;
	T C;
	T vectorLength;

	// build a general line equation from the point it goes through
	// and the direction vector it follows
	// the general line has normal vector follow left hand rule
	template <class Pt>
	void build(const Pt& P, const Pt& u) {
		A = -u.y;
		B = u.x;
		C = -A * P.x - B * P.y;

		vectorLength = sqrt(A * A + B * B);
	}

	template <class Pt>
	auto compute(const Pt& Q) const {
		return A * Q.x + B * Q.y + C;
	}

	// compute directional distance of a point to this line
	template <class Pt>
	auto directionalDistance(const Pt& Q) const {
		return compute(Q)/ vectorLength;
	}
};

template <class T>
auto directionalAngle(const T& u, const T& v)
{
	// angle has value from [0 - pi]
	auto angle = acosf((float)cosAngle(u, v));

	// test direction of v base on u
	// by computing dot product of normal vector n = (u.y, -u.x) and
	// sub vector t = u - v
	// after optimize expansion of the dot product we have following formular
	auto dotProduct = u.x*v.y - u.y*v.x;

	if (dotProduct < 0) {
		angle = -angle;
	}

	return angle;
}

template <class T>
inline bool checkPoint(const T& p, int width, int height) {
	return !(p.x < 0 || p.y < 0 || p.x > width || p.y > height);
}

template <class T, class Iterator>
void projectPoints(const T& p, const T& u, Iterator begin, Iterator end) {
	auto v = { u.y, -u.x };
	float t;
	for (auto iter = begin; iter != end; iter++) {
		auto& x = *iter;
		Intersect2D_Lines(p, u, x, v, &t, nullptr);
		x = p + u * t;
	}
}

template <class T>
float projectPoint(const T& p, const T& u, const T& x) {
	T v = { u.y, -u.x };
	float t;
	Intersect2D_Lines(p, u, x, v, &t, nullptr);
	return t;
}

template <class T>
bool IsMidleSegment1(
	const T& P, const T& A, const T& B
)
{
	T u = P - A;
	T v = P - B;
	float dotProduct = (float)(u.x*v.x + u.y*v.y);

	return dotProduct <= 0;
}

template <class T>
bool IsMidleSegment2(
	const T& P, const T& A, const T& B
)
{
	T u = P - A;
	T v = P - B;
	float dotProduct = (float)(u.x*v.x + u.y*v.y);

	return dotProduct < 0;
}

template <class T>
auto computeRank(
	const T& P, const T& A, const T& B
)
{
	T u = P - A;
	T v = P - B;
	return (u.x*v.x + u.y*v.y);
}

// compute ratio of vector v base on vector u
// vector u must be not a zero vector
template <class T>
auto computeT(
	const T&u, const T&v
)
{
	if (u.x) {
		return v.x / u.x;
	}
	return v.y / u.y;
}

/// offset a vector by a distance in direction of its normal vector which follow right hand rule
template <class T>
void offset(T& P, const T& u, float distance) {
	distance = (float)(distance / sqrt(lengthSquaredd(u)));

	//// normalize vector follow right hand rule has length is distance
	//T v(-u.y * distance, u.x * distance);
	//// caclulate new p
	//P = P + v;

	//optimize above statments
	P.x += -u.y * distance;
	P.y += u.x * distance;
}

template <class T>
char isPointInside(const std::vector<T>& poly, const T& Q) {
	float prevVal = 0;
	char sign = 1;
	int n = (int)poly.size();
	for (int j = 0; j < n; j++) {
		auto& P1 = poly[j];
		auto& P2 = poly[(j + 1) % n];
		auto u = P2 - P1;
		float currVal = compute(P1, u, Q);

		// check if the point lies on current edge
		// then the point is in the polygon
		if (currVal == 0) {
			if (IsMidleSegment1(Q, P1, P2)) {
				sign = 0;
			}
			else {
				sign = -1;
			}
			break;
		}

		// if the previous value and current value is opposite of sign
		// it means the point is out side of the polygon
		if (prevVal * currVal < 0) {
			sign = -1;
			break;
		}
		prevVal = currVal;
	}

	return sign;
}

// check if a point inside a polygon
// lies on an edge is not count
template <class T>
bool isPointInside2(const std::vector<T>& poly, const T& Q) {
	float prevVal = 0;
	int n = (int)poly.size();
	int j;
	for (j = 0; j < n; j++) {
		auto& P1 = poly[j];
		auto& P2 = poly[(j + 1) % n];
		auto u = P2 - P1;
		float currVal = compute(P1, u, Q);

		// check if the point lies on current edge
		if (currVal == 0) {
			break;
		}

		// if the previous value and current value is opposite of sign
		// it means the point is out side of the polygon
		if (prevVal * currVal < 0) {
			break;
		}
		prevVal = currVal;
	}

	return j == n;
}

///////////////////////////////////////////////////////////////////////////////////
/// check if sign of a point over generalines is same
/// if the point lies on a line, the function return line index
/// if sign of the point is different, the function return a nagative number of 
/// index of the line that sign of the point on that line is different than sign of the point on preivous line
/// if sign of the point is same, the function return 0
///////////////////////////////////////////////////////////////////////////////////
template <class T, class Pt>
int checkPointSign(const std::vector<GeneralLine<T>>& cachedPoly, const Pt& Q) {
	float prevVal = 0;
	int n = (int)cachedPoly.size();
	for (int j = 0; j < n; j++) {
		auto& generalLine = cachedPoly[j];
		float currVal = generalLine.directionalDistance(Q);

		// check if the point lies on current edge
		// then the point is in the polygon
		if (std::abs(currVal) <= GEOMETRY_EPSILON) {
			return (j + 1);
		}

		// if the previous value and current value is opposite of sign
		// it means the point is out side of the polygon
		if (prevVal * currVal < 0) {
			return -(j + 1);
			break;
		}
		prevVal = currVal;
	}

	return 0;
}


template <class T>
auto triangleArea(const T& A, const T& B, const T& C) {
	auto u = C - B;
	auto t = projectPoint(B, u, A);
	auto projectedA = B + u * t;

	auto la2 = lengthSquaredd(u);
	auto lh2 = lengthSquaredd(projectedA - A);
	
	return sqrt(la2*lh2) / 2;
}

template <class T>
auto polyArea(const std::vector<T>& poly) {
	int n = (int)poly.size();
	double area = 0;
	auto& A = poly[0];
	for (int j = 1; j < n - 1; j++) {
		auto& B = poly[j];
		auto& C = poly[j + 1];

		area += triangleArea(A, B, C);
	}

	return area;
}

template <class T>
bool hasInside(const std::vector<T>& poly1, const std::vector<T>& poly2) {
	int n = (int)poly2.size();
	for (int i = 0; i < n; i++) {
		auto& P = poly2[i];
		if (isPointInside2(poly1, P)) {
			return true;
		}
	}
	return false;
}

template <class T>
struct PolyIntersectionInfo {
	int atEdgeInPoly1;
	int atEdgeInPoly2;
	T position;
};

// check if two poly is intersect
// return -1 if two poly is not intersect
// return 0 of they are just touch to each other
// return 1 if they have a common zone
template <class T>
int checkIntersect2d(const std::vector<T>& poly1, const std::vector<T>& poly2, PolyIntersectionInfo<T>& intersectAt) {
	int m = (int)poly1.size();
	int n = (int)poly2.size();
	intersectAt.atEdgeInPoly1 = -1;
	intersectAt.atEdgeInPoly2 = -1;
	intersectAt.position = T();

	// count number of inside point
	if (hasInside(poly1, poly2)) {
		return 1;
	}
	if (hasInside(poly2, poly1)) {
		return 1;
	}

	bool isTouched = false;
	// check if any segment of two polygon intersect
	float t1, t2;
	for (int i = 0; i < m; i++) {
		auto& v11 = poly1[i];
		auto& v12 = poly1[(i + 1) % m];
		auto u1 = v12 - v11;


		for (int j = 0; j < n; j++) {
			auto& v21 = poly2[j];
			auto& v22 = poly2[(j + 1) % n];
			auto u2 = v22 - v21;

			// two segment must be intersect at the middle of each other
			if (Intersect2D_Lines(v11, u1, v21, u2, &t1, &t2)) {
				// intersect at middle of each line, don't count for intersection
				// at two construct points of the segment
				if ((t1 > 0.0f && t1 < 1.0f) && (t2 > 0.0f && t2 < 1.0f)) {
					// if two edges are intersect at middle, it means they have common area
					intersectAt.atEdgeInPoly1 = i;
					intersectAt.atEdgeInPoly2 = j;
					return 1;
				}
				else if ((t1 == 0.0f || t1 == 1.0f) && (t2 >= 0.0f && t2 <= 1.0f)) {
					isTouched = true;
				}
				else if ((t2 == 0.0f || t2 == 1.0f) && (t1 >= 0.0f && t1 <= 1.0f)) {
					isTouched = true;
				}
			}
			else {
				// now two lines are parallel or overlap
				if (compute(v11, u1, v21) == 0) {
					// now two lines are overlap
					// compute parameter t of v21, v22 on segment 1
					if (u1.x || u1.y) {
						// check if v21 is in middle of segment 1
						auto t = computeT(u1, v21 - v11);
						if (t >= 0.0f && t <= 1.0f) {
							isTouched = true;
						}

						// check if v22 is in middle of segment 1
						t = computeT(u1, v22 - v11);
						if (t >= 0.0f && t <= 1.0f) {
							isTouched = true;
						}
					}

					// compute parameter t of v11, v12 on segment 2
					if (u2.x || u2.y) {
						// check if v11 is in middle of segment 2
						auto t = computeT(u2, v11 - v21);
						if (t >= 0.0f && t <= 1.0f) {
							isTouched = true;
						}

						// check if v12 is in middle of segment 2
						t = computeT(u2, v12 - v21);
						if (t >= 0.0f && t <= 1.0f) {
							isTouched = true;
						}
					}
				}
			}

			if (isTouched) {
				intersectAt.atEdgeInPoly1 = i;
				intersectAt.atEdgeInPoly2 = j;
			}
		}
	}

	return isTouched ? 0 : -1;
}

inline int next(int i, int nSub1) {
	return i == nSub1 ? 0 : i + 1;
}

inline int back(int i, int nSub1) {
	return i == 0 ? nSub1 : i - 1;
}

inline int next(int i, int dir, int nSub1) {
	return dir > 0 ? next(i, nSub1) : back(i, nSub1);
}

namespace geometry {

	template <class Pt>
	void buildGeneralLines(GeneralLine<float>* cachedPoly, Pt* poly, int nSub1) {
		for (int i = 0; i <= nSub1; i++) {
			auto& P = poly[i];
			auto& Q = poly[next(i, nSub1)];

			cachedPoly[i].build(P, Q - P);
		}
	}
	template<class Pt>
	struct _ConnectionNode {
		const Pt* pCoord;
		_ConnectionNode* node1;
		_ConnectionNode* node2;

		_ConnectionNode(const Pt* p) {
			pCoord = p;
			node1 = nullptr;
			node2 = nullptr;
		}
	};

	template<class Pt>
	struct _Segment {
		_ConnectionNode<Pt>* node1;
		_ConnectionNode<Pt>* node2;
	};

	template<class Pt>
	void inline linkNode(_ConnectionNode<Pt>* from, _ConnectionNode<Pt>* to) {
		if (from->node1 == nullptr) {
			from->node1 = to;
		}
		else if (from->node2 == nullptr) {
			if (from->node1 == to) {
				return;
			}
			from->node2 = to;
		}
		else {
			if (from->node1 == to || from->node2 == to) {
				return;
			}
			// a node has only two connection
			// if the program reach here, it mean there is something wrong in caller side
			throw std::runtime_error("node is already link fully");
		}
	}

	template<class Pt>
	bool inline addNode(_Segment<Pt>& segment, _ConnectionNode<Pt>* node) {
		if (segment.node1 == nullptr) {
			segment.node1 = node;
		}
		else if (segment.node2 == nullptr) {
			auto existNode = segment.node1;
			if (existNode->pCoord == node->pCoord) return false;

			linkNode(node, existNode);
			linkNode(existNode, node);

			segment.node2 = node;
		}
		else {
			if (segment.node1->pCoord == node->pCoord ||
				segment.node2->pCoord == node->pCoord) return false;
			throw std::runtime_error("segment is full");
		}

		return true;
	}

	template <class Pt>
	bool polyIntersect(const std::vector<Pt>& poly1, const std::vector<Pt>& poly2, std::vector<Pt>& polyOut) {
		if (poly1.size() < 3 || poly2.size() < 3) {
			throw std::runtime_error("invalid polygon");
		}
		int n1Sub1 = (int)poly1.size() - 1;
		int n2Sub1 = (int)poly2.size() - 1;

		typedef _ConnectionNode<Pt> ConnectionNode;
		typedef _Segment<Pt> Segment;

		std::vector<Segment> segmentMap(poly1.size() + poly2.size(), { nullptr, nullptr });

		// cache polygon 1 information use for geometry calculation
		std::vector<GeneralLine<float>> cachedPoly1(poly1.size());
		buildGeneralLines(cachedPoly1.data(), poly1.data(), n1Sub1);

		// cache polygon 2 information use for geometry calculation
		std::vector<GeneralLine<float>> cachedPoly2(poly2.size());
		buildGeneralLines(cachedPoly2.data(), poly2.data(), n2Sub1);

		typedef std::shared_ptr<ConnectionNode> ConnectionNodeRef;

		std::list<ConnectionNodeRef> connectionNodes;
		auto buildConnectionFromInsidePoints = [&connectionNodes, &segmentMap](const Pt* poly, int nSub1, const std::vector<GeneralLine<float>>& otherCachedPoly, int baseMapIdx) {
			int res;
			bool addedNode;
			for (int i = 0; i <= nSub1; i++) {
				auto& P = poly[i];
				res = checkPointSign(otherCachedPoly, P);
				if (res == 0) {
					// the current vertex is inside of polygon 2
					auto connectionNode = std::make_shared<ConnectionNode>( &P );

					auto pNewNode = connectionNode.get();
					addedNode = addNode(segmentMap[i + baseMapIdx], pNewNode);
					addedNode = addNode(segmentMap[back(i, nSub1) + baseMapIdx], pNewNode) || addedNode;
					if (addedNode) connectionNodes.push_back(connectionNode);
				}
			}
		};

		buildConnectionFromInsidePoints(poly1.data(), n1Sub1, cachedPoly2, 0);
		if (connectionNodes.size() == poly1.size()) {
			// polygon 1 is inside polygon 2
			polyOut = poly1;
			return true;
		}
		auto step1ConnectionCount = connectionNodes.size();
		buildConnectionFromInsidePoints(poly2.data(), n2Sub1, cachedPoly1, (int)poly1.size());
		if (connectionNodes.size() == step1ConnectionCount + poly2.size()) {
			// polygon 2 is inside polygon 1
			polyOut = poly2;
			return true;
		}

		float t1, t2, l1, l2;
		bool addedNode;
		// find intersection of segments between polygon 1 and polygon 2
		std::list<Pt> intersectionPoints;
		for (int i = 0; i <= n1Sub1; i++) {
			auto& P1 = poly1[i];
			auto& Q1 = poly1[next(i, n1Sub1)];
			auto u1 = Q1 - P1;
			const auto& u1Length = cachedPoly1[i].vectorLength;

			for (int j = 0; j <= n2Sub1; j++) {
				auto& P2 = poly2[j];
				auto& Q2 = poly2[next(j, n2Sub1)];
				auto u2 = Q2 - P2;
				const auto& u2Length = cachedPoly2[j].vectorLength;

				if (Intersect2D_Lines(P1, u1, P2, u2, &t1, &t2)) {
					l1 = t1 * u1Length;
					l2 = t2 * u2Length;

					if (l1 >= -GEOMETRY_EPSILON && l1 < u1Length - GEOMETRY_EPSILON && l2 >= -GEOMETRY_EPSILON && l2 < u2Length - GEOMETRY_EPSILON) {
						// intersect at middle of each segment
						if (l1 > GEOMETRY_EPSILON && l1 < u1Length && l2 > GEOMETRY_EPSILON && l2 < u2Length) {

							auto P = P1 + u1 * t1;
							intersectionPoints.push_back(P);
							auto connectionNode = std::make_shared<ConnectionNode>(&intersectionPoints.back());
							auto pNewNode = connectionNode.get();

							// add node to segment #i of polygon 1
							addedNode = addNode(segmentMap[i], pNewNode);

							// add node to segment #j of polygon 2
							addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;

							if (addedNode) connectionNodes.push_back(connectionNode);
						}
						else if (std::abs(l1) <= GEOMETRY_EPSILON && l2 > GEOMETRY_EPSILON && l2 < u2Length) {

							auto connectionNode = std::make_shared<ConnectionNode>(&P1);
							auto pNewNode = connectionNode.get();

							// add node to segment #i of polygon 1
							addedNode = addNode(segmentMap[i], pNewNode);
							// add node to previous segment of segment #i of polygon 1
							addedNode = addNode(segmentMap[back(i, n1Sub1)], pNewNode) || addedNode;

							// add node to segment of polygon 2
							addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;

							if (addedNode) connectionNodes.push_back(connectionNode);
						}
						else if (std::abs(l2) <= GEOMETRY_EPSILON && l1 > GEOMETRY_EPSILON && l1 < u1Length) {

							auto connectionNode = std::make_shared<ConnectionNode>(&P2 );
							auto pNewNode = connectionNode.get();

							// add node to segment #i of polygon 1
							addedNode = addNode(segmentMap[i], pNewNode);

							// add node to segment of polygon 2
							addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;
							// add node to previous segment of segment #j of polygon 2
							addedNode = addNode(segmentMap[back(j, n2Sub1) + poly1.size()], pNewNode) || addedNode;

							if (addedNode) connectionNodes.push_back(connectionNode);
						}
						else if (std::abs(l1) <= GEOMETRY_EPSILON && std::abs(l2) <= GEOMETRY_EPSILON){

							auto connectionNode = std::make_shared<ConnectionNode>(&P1 );
							auto pNewNode = connectionNode.get();

							// add node to segment #i of polygon 1
							addedNode = addNode(segmentMap[i], pNewNode);
							// add node to previous segment of segment #i of polygon 1
							addedNode = addNode(segmentMap[back(i, n1Sub1)], pNewNode) || addedNode;

							// add node to segment of polygon 2
							addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;
							// add node to previous segment of segment #j of polygon 2
							addedNode = addNode(segmentMap[back(j, n2Sub1) + poly1.size()], pNewNode) || addedNode;

							if (addedNode) connectionNodes.push_back(connectionNode);
						}
					}
				}
				else {
					// now two lines are parallel or overlap
					// continue to check if P2 is lies on segment #i of polygon 1 by using general line
					if (std::abs(cachedPoly1[i].directionalDistance(P2)) <= GEOMETRY_EPSILON) {
						// now two lines are overlap
						// check if P2 is in middle of segment 1
						t1 = computeT(u1, P2 - P1);
						l1 = t1 * u1Length;
						if (l1 >= -GEOMETRY_EPSILON && l1 < u1Length - GEOMETRY_EPSILON) {
							if (l1 > GEOMETRY_EPSILON && l1 < u1Length) {
								auto connectionNode = std::make_shared<ConnectionNode>(&P2 );
								auto pNewNode = connectionNode.get();

								// add node to segment #i of polygon 1
								addedNode = addNode(segmentMap[i], pNewNode);

								// add node to segment of polygon 2
								addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;
								// add node to previous segment of segment #j of polygon 2
								addedNode = addNode(segmentMap[back(j, n2Sub1) + poly1.size()], pNewNode) || addedNode;

								if (addedNode) connectionNodes.push_back(connectionNode);
							}
							else if (std::abs(l1) <= GEOMETRY_EPSILON) {
								auto connectionNode = std::make_shared<ConnectionNode>(&P1 );
								auto pNewNode = connectionNode.get();

								// add node to segment #i of polygon 1
								addedNode = addNode(segmentMap[i], pNewNode);
								// add node to previous segment of segment #i of polygon 1
								addedNode = addNode(segmentMap[back(i, n1Sub1)], pNewNode) || addedNode;

								// add node to segment of polygon 2
								addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;
								// add node to previous segment of segment #j of polygon 2
								addedNode = addNode(segmentMap[back(j, n2Sub1) + poly1.size()], pNewNode) || addedNode;

								if (addedNode) connectionNodes.push_back(connectionNode);
							}
						}
						
						// check if P1 is in middle of segment 2
						t2 = computeT(u2, P1 - P2);
						l2 = t2 * u2Length;
						if (l2 > GEOMETRY_EPSILON && l2 < u2Length - GEOMETRY_EPSILON) {
							auto connectionNode = std::make_shared<ConnectionNode>(&P1 );
							auto pNewNode = connectionNode.get();

							// add node to segment #i of polygon 1
							addedNode = addNode(segmentMap[i], pNewNode);
							// add node to previous segment of segment #i of polygon 1
							addedNode = addNode(segmentMap[back(i, n1Sub1)], pNewNode) || addedNode;

							// add node to segment of polygon 2
							addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;

							if (addedNode) connectionNodes.push_back(connectionNode);
						}
						else if (std::abs(l2) <= GEOMETRY_EPSILON) {
							auto connectionNode = std::make_shared<ConnectionNode>(&P1 );
							auto pNewNode = connectionNode.get();

							// add node to segment #i of polygon 1
							addedNode = addNode(segmentMap[i], pNewNode);
							// add node to previous segment of segment #i of polygon 1
							addedNode = addNode(segmentMap[back(i, n1Sub1)], pNewNode) || addedNode;

							// add node to segment of polygon 2
							addedNode = addNode(segmentMap[j + poly1.size()], pNewNode) || addedNode;
							// add node to previous segment of segment #j of polygon 2
							addedNode = addNode(segmentMap[back(j, n2Sub1) + poly1.size()], pNewNode) || addedNode;

							if (addedNode) connectionNodes.push_back(connectionNode);
						}
					}
				}
			}
		}

		if (connectionNodes.size() == 0) {
			return false;
		}

		auto pConnectionNode = connectionNodes.front().get();
		if (connectionNodes.size() == 1) {
			polyOut.push_back(*pConnectionNode->pCoord);
			return true;
		}
		if (connectionNodes.size() == 2) {
			polyOut.push_back(*pConnectionNode->pCoord);
			polyOut.push_back(*pConnectionNode->node1->pCoord);
			return true;
		}
		
		auto pStartNode = pConnectionNode;
		decltype(pConnectionNode) previousNode = nullptr;
		do
		{
			polyOut.push_back(*pConnectionNode->pCoord);

			if (pConnectionNode->node1 != previousNode) {
				previousNode = pConnectionNode;
				pConnectionNode = pConnectionNode->node1;
			}
			else {
				previousNode = pConnectionNode;
				pConnectionNode = pConnectionNode->node2;
			}
			if (pConnectionNode == nullptr) {
				break;
			}
		} while (pConnectionNode != pStartNode);

		return pConnectionNode != nullptr;
	}

	template <class Pt>
	void merge(std::vector<Pt>& poly, float epsilonDistance) {
		int nSub1 = (int)poly.size() - 1;

		if (nSub1 > 0) {
			if (sqrt(lengthSquared(poly[0] - poly[nSub1])) <= epsilonDistance) {
				nSub1--;
			}
		}

		int j = 0;
		for (int i = j + 1; i <= nSub1; i++) {
			if (sqrt(lengthSquared(poly[j] - poly[i])) > epsilonDistance) {
				j++;
				if (i > j) {
					poly[j] = poly[i];
				}
			}
		}

		if (nSub1 >= 0) {
			// make j become number of point
			j++;
		}

		poly.resize(j);
	}
}