#ifndef XNUMER_GEOMETRY_TRIANGULATION_MONOTONE_PARTITIONING_HPP
#define XNUMER_GEOMETRY_TRIANGULATION_MONOTONE_PARTITIONING_HPP

#include "../../../Math and utility/Geometry2D/geometry_2d_functions.hpp"
#include "polygon2.hpp"
#include <vector>
#include "../../../Math and utility/pseudo_list.hpp"

#include "edge.hpp"

template<typename T>
struct sweepLineEdgeComparator {
	using is_transparent = void;

	bool operator() (const Edge<T>& left, const Edge<T>& right) const;
	bool operator() (const Edge<T>& left, const Point2<T>& right) const;
	bool operator() (const Point2<T>& left, const Edge<T>& right) const;
};

//template<typename T>
//void splitPolygonIntoMonotones(PolygonVertex2<T>* polygonVertices, std::vector<std::vector<int>>& monotones);

template<typename T>
void splitPolygonIntoMonotones(PseudoList<Point2<T>>& polygon, std::vector<PseudoList<Point2<T>>>& monotones);


#include "monotone_partition_impl.hpp"
#endif
