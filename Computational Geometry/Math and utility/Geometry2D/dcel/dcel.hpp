#ifndef XNUMER_GEMOETRY_DCEL
#define XNUMER_GEOMETRY_DCEL

#include "point2.hpp"
#include <vector>
#include "dcel_vertex.hpp"
#include "dcel_half_edge.hpp"
#include "dcel_face.hpp"

	template<typename T>
	class DCEL {
	public:
		DCEL();
		DCEL(const std::vector<Point2<T>>& points);
		DCEL(std::initializer_list<Point2<T>> initList);

		~DCEL();

		dcelVertex<T>* firstVertex;
		dcelHalfEdge<T>* firstEdge;
		dcelFace<T>* firstFace;
	};

#include "dcel_impl.hpp"


#endif