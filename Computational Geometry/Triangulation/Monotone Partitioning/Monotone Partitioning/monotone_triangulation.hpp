#ifndef XNUMER_GEOMETRY_MONOTONE_TRIANGULATION_HPP
#define XNUMER_GEOMETRY_MONOTONE_TRIANGULATION_HPP

#include "polygon2.hpp"
#include <vector>

template<typename T>
void monotoneTriangulation(std::vector<PolygonVertex2<T>*>& monotone, std::vector<std::pair<Point2<T>, Point2<T>>>& triangulation);

#include "monotone_triangulation_impl.hpp"

#endif

