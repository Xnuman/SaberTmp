#ifndef XNUMER_GEOMETRY_EDGE_HPP
#define XNUMER_GEOMETRY_EDGE_HPP

#include "../../../Math and utility/utility_functions.hpp"
#include "../../../Math and utility/Geometry2D/point2/point2.hpp"

template<typename T>
struct Edge {
public:
	Edge(const Point2<T>& a, const Point2<T>& b);
	Point2<T> top, down;
};

template<typename T>
Point2<T> edgeIntersectLineY(const Edge<T>& e, const T& y);

#include "edge_impl.hpp"

#endif
