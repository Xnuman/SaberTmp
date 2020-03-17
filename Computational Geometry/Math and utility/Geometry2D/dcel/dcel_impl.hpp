#ifndef XNUMER_GEMOETRY_DCEL_IMPL_HPP
#define XNUMER_GEOMETRY_DCEL_IMPL_HPP

//#include "dcel.hpp"

	template<typename T>
	DCEL<T>::DCEL() : DCEL<T>::firstVertex(nullptr), DCEL<T>::firstHalfEdge(nullptr), DCEL<T>::firstFace(nullptr) {}

	template<typename T>
	DCEL<T>::DCEL(const std::vector<Point2<T>>& points) {

		dcelVertex<T>* newVertex = nullptr;
		dcelHalfEdge<T>* newEdge = nullptr, *prevEdge = nullptr;


		if (points.size() > 1) {
			for (int i = 0; i != points.size(); ++i) {
				newVertex = new dcelVertex<T>();
				newVertex->coord = points[i];
				newVertex->ind = i;
				newEdge = new dcelHalfEdge<T>();
				newVertex->edge = newEdge;
				newEdge->origin = newVertex;
				newEdge->ind = i;
				if (prevEdge) {
					prevEdge->end = newVertex;
					prevEdge->next = newEdge;
					newEdge->prev = prevEdge;
				}
				prevEdge = newEdge;
				newEdge = newEdge->next;

				if (i == 0) {
					firstVertex = newVertex;
					firstEdge = prevEdge;
				}

				if (i == points.size() - 1) {
					prevEdge->next = firstEdge;
					prevEdge->end = firstVertex;
					firstEdge->prev = prevEdge;
				}
			}

			newEdge = nullptr, prevEdge = nullptr;
			dcelHalfEdge<T>* currentEdge = firstEdge;

			do {
				newEdge = new dcelHalfEdge<T>();
				newEdge->origin = currentEdge->end;
				newEdge->end = currentEdge->origin;
				if (prevEdge) {
					prevEdge->next = newEdge;
					newEdge->prev = prevEdge;
				}
				prevEdge = newEdge;
				newEdge = newEdge->next;

				currentEdge->twin = prevEdge;
				currentEdge = currentEdge->prev;
			} while (currentEdge != firstEdge);

		}

	}

	template<typename T>
	DCEL<T>::DCEL(std::initializer_list<Point2<T>> initList) : DCEL(std::vector<Point2<T>>{initList}) {}

	template<typename T>
	DCEL<T>::~DCEL() {

		dcelVertex<T>* vertex = firstVertex->edge->end;

		while (vertex && vertex != firstVertex) {
			dcelVertex<T>* tmp = vertex->edge->end;
			delete vertex->edge->twin;
			vertex->edge->twin = nullptr;
			delete vertex->edge;
			vertex->edge = nullptr;
			delete vertex;
			vertex = tmp;
		}

		delete firstVertex->edge->twin;
		delete firstVertex->edge;
		delete firstVertex;
		firstVertex = nullptr;

	}

#endif
