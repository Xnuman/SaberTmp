#ifndef XNUMER_GEOMETRY_2D_FUNCTIONS_HPP
#define XNUMER_GEOMETRY_2D_FUNCTIONS_HPP

#include "point2/point2.hpp"
#include <cmath>
#include <vector>

template<typename T>
constexpr T precision_threshold = (std::is_same_v<T, float> ? static_cast<T>(1E-07f) : static_cast<T>(1E-10));

template<typename T>
constexpr T abs(T value) {
	if constexpr (std::is_floating_point_v<T>) {
		return std::fabs(value);
	}
	else {
		return value < 0 ? -value : value;
	}
}

template<typename T>
constexpr bool isEqual(const T& a, const T& b) {
	if constexpr (std::is_floating_point_v<T>) {
		return abs(a - b) < precision_threshold<T>;
	}
	else {
		return a == b;
	}
}


template<typename T>
T triangleArea2(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) {
	return ((b.getX() - a.getX()) * (c.getY() - a.getY())) - ((b.getY() - a.getY()) * (c.getX() - a.getX()));
}

template<typename T>
T triangleArea(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) {
	return abs(triangleArea2(a, b, c)) / static_cast<T>(2.0f);
}

template<typename T>
bool isLeftTurn(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) {
	return triangleArea2(a, b, c) > 0;
}

template<typename T>
bool isCollinear(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) {
	return isEqual(triangleArea2(a, b, c), static_cast<T>(0));
}

template<typename T>
bool isRightTurn(const Point2<T>& a, const Point2<T>b, const Point2<T>& c) {
	return triangleArea2(a, b, c) < 0;
}


// TODO: replace vector with linked list
template<typename T>
T polygonArea(const std::vector<Point2<T>>& polygonVertices, const Point2<T> point = Point2<T>{ 0.0f, 0.0f }) {
	T sum = 0;

	auto& prev = polygonVertices.back();
	for (auto& vertex : polygonVertices) {
		sum += triangleArea2(point, vertex, prev) / 2;
		prev = vertex;
	}

	return sum;
}

// is point C lies between A and B?
template<typename T>
bool isBetween(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) {
	if (!isCollinear(a, b, c)) {
		return false;
	}

	if (!isEqual(a.getX(), b.getX())) {
		return (((a.getX() <= c.getX()) && (c.getX() <= b.getX())) ||
			((a.getX() >= c.getX()) && (c.getX() >= b.getX())));
	}
	else {
		return (((a.getY() <= c.getY()) && (c.getY() <= b.getY())) ||
			((a.getY() >= c.getY()) && (c.getY() >= b.getY())));
	}
}

template<typename T>
bool isIntersectProp(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c, const Point2<T>& d) {
	return ((isLeftTurn(a, b, c) != isLeftTurn(a, b, d)) &&
		(isLeftTurn(c, d, a) != isLeftTurn(c, d, b)));
}

template<typename T>
bool isIntersect(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c, const Point2<T>& d) {
	if (isIntersectProp(a, b, c, d)) {
		return true;
	}
	else if ((isBetween(a, b, c)) ||
		(isBetween(a, b, d)) ||
		(isBetween(c, d, a)) ||
		(isBetween(c, d, b))) {
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
bool isDiagonalie(const Point2<T>& a, const Point2<T>& b, const std::vector<Point2<T>>& polygonVertices) {

	Point2<T> c1, c2;

	auto& prev = polygonVertices.back();

	for (auto& vertex : polygonVertices) {
		c1 = vertex;
		c2 = prev;
		prev = vertex;

		if (a != c1 && a != c2 && b != c1 && b != c2 && isIntersect(a, b, c1, c2)) {
			return false;
		}
	}

	return true;
}

// true if ab lies in cone formed by a0, a, a1, where a0 and a1 are edges incident to a
template<typename T>
bool isInside(const Point2<T>& a, const Point2<T>& b, const Point2<T>& a0, const Point2<T>& a1) {

	// first check if a is convex or reflex
	if (isLeftTurn(a0, a, a1)) {
		//if convex check if ab lays between aa0 and aa1
		return (isLeftTurn(a, b, a0) && isRightTurn(a, b, a1));
	}
	else {
		//if reflex then we check that ab doesn't lay in the exterior cone a0, a, a1
		return !(isLeftTurn(a, b, a1) && isRightTurn(a, b, a0));
	}

}

//TODO: replace vector with linked list
template<typename T>
bool isDiagonal(const Point2<T>& a, const Point2<T>& b, const std::vector<Point2<T>>& polygonVertices) {

	Point2<T> a0, a1, b0, b1;

	for (int i = 0; i != polygonVertices.size(); ++i) {
		if (polygonVertices[i] == a) {
			if (i == 0) {
				a0 = polygonVertices[polygonVertices.size() - 1];
			}
			else {
				a0 = polygonVertices[i - 1];
			}
			if (i == polygonVertices.size() - 1) {
				a1 = polygonVertices[0];
			}
			else {
				a1 = polygonVertices[i + 1];
			}
		}
		else if (polygonVertices[i] == b) {
			if (i == 0) {
				b0 = polygonVertices[polygonVertices.size() - 1];
			}
			else {
				b0 = polygonVertices[i - 1];
			}
			if (i == polygonVertices.size() - 1) {
				b1 = polygonVertices[0];
			}
			else {
				b1 = polygonVertices[i + 1];
			}
		}
	}

	return isInside(a, b, a0, a1) && isInside(b, a, b0, b1) && isDiagonalie(a, b, polygonVertices);
}

template<typename T>
bool isInteriorCup(const Point2<T>& v, const std::vector<Point2<T>>& polygonVertices) {

	Point2<T> v0, v1;

	for (int i = 0; i != polygonVertices.size(); ++i) {
		if (polygonVertices[i] == v) {
			if (i == 0) {
				v0 = polygonVertices[polygonVertices.size() - 1];
			}
			else {
				v0 = polygonVertices[i - 1];
			}
			if (i == polygonVertices.size() - 1) {
				v1 = polygonVertices[0];
			}
			else {
				v1 = polygonVertices[i + 1];
			}
			break;
		}
	}

	return (!isLeftTurn(v0, v, v1) && (
		((v0.getY() < v.getY()) && v1.getY() < v.getY()) ||
		((v0.getY() > v.getY()) && v1.getY() > v.getY())));
}


#endif
