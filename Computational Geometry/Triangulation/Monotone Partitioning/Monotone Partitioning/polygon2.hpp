#ifndef XNUMER_GEOMETRY_TRIANGULATION_POLYGON2_HPP
#define XNUMER_GEOMETRY_TRIANGULATION_POLYGON2_HPP

#include "../../../Math and utility/Geometry2D/geometry_2d_functions.hpp"
#include <type_traits>


template<typename T>
class PolygonVertex2 {
public:
	PolygonVertex2(const Point2<T>& firstVertex);

	PolygonVertex2* insert(PolygonVertex2* newVertex);

	Point2<T> coordinates;
	PolygonVertex2* next;
	PolygonVertex2* prev;
	PolygonVertex2* nextMonotoneVertex;
	int ind;
};

#include "polygon2_impl.hpp"

#endif


