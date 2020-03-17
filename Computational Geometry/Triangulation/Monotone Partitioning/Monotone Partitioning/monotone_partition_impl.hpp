#ifndef XNUMER_GEOMETRY_TRIANGULATION_MONOTONE_PARTITIONING_IMPL_HPP
#define XNUMER_GEOMETRY_TRIANGULATION_MONOTONE_PARTITIONING_IMPL_HPP

#include <set>
#include <algorithm>
#include <iterator>
#include "../../../Math and utility/pseudo_list.hpp"

template<typename T>
bool sweepLineEdgeComparator<T>::operator() (const Edge<T>& left, const Edge<T>& right) const {
	if (left.top.getX() == right.top.getX() && left.down.getX() == right.down.getX()) {
		return false;
	}
	return left.top.getX() <= right.top.getX() && left.down.getX() <= right.down.getX();
}

template<typename T>
bool sweepLineEdgeComparator<T>::operator() (const Edge<T>& left, const Point2<T>& right) const {
	return edgeIntersectLineY(left, right.getY()).getX() < right.getX();
}

template<typename T>
bool sweepLineEdgeComparator<T>::operator() (const Point2<T>& left, const Edge<T>& right) const {
	return left.getX() < edgeIntersectLineY(right, left.getY()).getX();
}


namespace impl {
	template<typename T>
	void initMonotonePartition(
		std::vector<Edge<T>>& edges, PseudoList<Point2<T>>& pseudo_vertices, 
		std::vector<typename PseudoList<Point2<T>>::Node>& pseudo_sorted_vertices)
	{

		using Node = typename PseudoList<Point2<T>>::Node;

		// start pseudo refactoring
		//PseudoList<Point2<T>> pseudo_vertices = { { 1, 2 }, { 3, 4 } };
		//std::vector<typename PseudoList<Point2<T>>::Node> pseudo_sorted_vertices;

		Node pseudo_begin = pseudo_vertices.front();
		Node pseudo_current = pseudo_vertices.front();

		do {
			pseudo_current.setNextMonotone(pseudo_current.next().getInd());
			pseudo_sorted_vertices.push_back(pseudo_current);
			edges.push_back(Edge(pseudo_current.data(), pseudo_current.next().data()));
			pseudo_current = pseudo_current.next();
		} while (pseudo_current != pseudo_begin);

		std::sort(pseudo_sorted_vertices.begin(), pseudo_sorted_vertices.end(),
			[](Node a, Node b) {
				if (a.data().getY() != b.data().getY()) {
					return a.data().getY() > b.data().getY();
				}
				else {
					return a.data().getX() < b.data().getX();
				}
			});

		//end pseudo refactoring

		/*PolygonVertex2<T>* begin = vertices;
		PolygonVertex2<T>* current = vertices;

		do {
			sortedVertices.push_back(current);
			edges.push_back(Edge(current->coordinates, current->next->coordinates));
			current = current->next;
		} while (current != begin);

		std::sort(sortedVertices.begin(), sortedVertices.end(), [](PolygonVertex2<T>* a, PolygonVertex2<T>* b) {
			if (a->coordinates.getY() != b->coordinates.getY()) {
				return a->coordinates.getY() > b->coordinates.getY();
			}
			else {
				return a->coordinates.getX() < b->coordinates.getX();
			}
			});*/
	}
#define PSEUDO
	template<typename T>
	void buildMonotoneDiagonals(std::vector<PolygonVertex2<T>*>& sortedVertices, std::vector<typename PseudoList<Point2<T>>::Node>& sorted_pseudo_vertices) {
		std::set<Edge<T>, decltype(sweepLineEdgeComparator<T>())> sweepLine;
#ifdef PSEUDO
		

		// pseudo refactoring start

		using Node = typename PseudoList<Point2<T>>::Node;

		Node rememberedVertex = sorted_pseudo_vertices.front(); // dummy
		bool isRemembered = false;

		for (Node& vertex : sorted_pseudo_vertices) {
			bool firstDeleted = false;
			bool secondDeleted = false;

			Edge<T> e1{ vertex.data(), vertex.next().data() };
			Edge<T> e2{ vertex.prev().data(), vertex.data() };

			auto firstEdge = sweepLine.find(e1);
			if (firstEdge != sweepLine.end()) {
				sweepLine.erase(firstEdge);
				firstDeleted = true;
			}

			auto secondEdge = sweepLine.find(e2);
			if (secondEdge != sweepLine.end()) {
				sweepLine.erase(secondEdge);
				secondDeleted = true;
			}

			auto rightEdge = sweepLine.upper_bound(vertex.data());
			auto leftEdge = sweepLine.lower_bound(vertex.data());

			if (leftEdge != sweepLine.begin()) {
				leftEdge--;
			}

			if (leftEdge != sweepLine.end() && rightEdge != sweepLine.end()) {
				T leftIntersectionX = edgeIntersectLineY(*leftEdge, vertex.data().getY()).getX();
				if ((leftIntersectionX < vertex.data().getX()) ||
					(leftIntersectionX == vertex.data().getX() && leftEdge != sweepLine.begin()))
				{
					if (isRemembered) {
						vertex.setNextMonotone(rememberedVertex.getInd());
					}
					rememberedVertex = vertex;
					isRemembered = true;
				}
			}

			if (!firstDeleted) {
				sweepLine.insert(e1);
			}
			if (!secondDeleted) {
				sweepLine.insert(e2);
			}
		}


		//pseudo refactoring end
#else
		PolygonVertex2<T>* rememberedVertex = nullptr;

		for (PolygonVertex2<T>*& vertex : sortedVertices) {
			bool firstDeleted = false;
			bool secondDeleted = false;

			Edge<T> e1{ vertex->coordinates, vertex->next->coordinates };
			Edge<T> e2{ vertex->prev->coordinates, vertex->coordinates };

			auto firstEdge = sweepLine.find(e1);
			if (firstEdge != sweepLine.end()) {
				sweepLine.erase(firstEdge);
				firstDeleted = true;
			}
			
			auto secondEdge = sweepLine.find(e2);
			if (secondEdge != sweepLine.end()) {
				sweepLine.erase(secondEdge);
				secondDeleted = true;
			}

			auto rightEdge = sweepLine.upper_bound(vertex->coordinates);
			auto leftEdge = sweepLine.lower_bound(vertex->coordinates);

			if (leftEdge != sweepLine.begin()) {
				leftEdge--;
			}

			if(leftEdge != sweepLine.end() && rightEdge != sweepLine.end()){
				T leftIntersectionX = edgeIntersectLineY(*leftEdge, vertex->coordinates.getY()).getX();
				if ((leftIntersectionX < vertex->coordinates.getX()) ||
					(leftIntersectionX == vertex->coordinates.getX() && leftEdge != sweepLine.begin()))
				{
					if (rememberedVertex) {
						vertex->nextMonotoneVertex = rememberedVertex;
					}
					rememberedVertex = vertex;
				}
			}

			if (!firstDeleted) {
				sweepLine.insert(e1);
			}
			if (!secondDeleted) {
				sweepLine.insert(e2);
			}
		}
#endif
	}

	template<typename T>
	std::vector < PseudoList<Point2<T>>> splitIntoMonotones(std::vector<typename PseudoList<Point2<T>>::Node>& sorted, PseudoList<Point2<T>>& polygon) {

		std::vector<PseudoList<Point2<T>>> monotones;

		using Node = typename PseudoList<Point2<T>>::Node;

		Node begin = polygon.front();
		Node remembered = begin;
		uint32_t diffSize = 0;
		monotones.push_back(PseudoList<Point2<T>>());
		do {
			if (remembered.getNextMonotone().getInd() != remembered.next().getInd() && remembered.getInd() > remembered.getNextMonotone().getInd()) {
				int currentInd = remembered.getNextMonotone().getInd();
				Node currentVertex = remembered.getNextMonotone();
				monotones.push_back(PseudoList<Point2<T>>());
				while (currentInd != remembered.getInd()) {
					monotones.back().push_back(currentVertex.data());

					uint32_t diff = currentInd; /*- diffSize;*/

					if (currentInd != remembered.getNextMonotone().getInd()) {
						monotones.front().remove(monotones.front().front().getInd() + (diff));
						diffSize++;
					}

					currentVertex = currentVertex.getNextMonotone();
					currentInd = currentVertex.getInd();
				}
				currentVertex.setNextMonotone(currentVertex.next().getInd());
				monotones.back().push_back(currentVertex.data());
			}
			monotones.front().push_back(remembered.data());
			remembered = remembered.next();
		} while (remembered != begin);

		return monotones;

	}

	/*template<typename T>
	std::vector<std::vector<PolygonVertex2<T>*>> splitIntoMonotones(PolygonVertex2<T>* polygonVertices) {

		std::vector<std::vector<PolygonVertex2<T>*>> monotones;

		PolygonVertex2<T>* begin = polygonVertices;
		PolygonVertex2<T>* rememberedVertex = polygonVertices;
		int diffSize = 0;
		monotones.push_back(std::vector<PolygonVertex2<T>*>());
		do {
			if (rememberedVertex->nextMonotoneVertex->ind != rememberedVertex->next->ind &&
				rememberedVertex->ind > rememberedVertex->nextMonotoneVertex->ind) {

				int currentInd = rememberedVertex->nextMonotoneVertex->ind;
				PolygonVertex2<T>* currentVertex = rememberedVertex->nextMonotoneVertex;
				monotones.push_back(std::vector<PolygonVertex2<T>*>());
				while (currentInd != rememberedVertex->ind) {

					monotones.back().push_back(currentVertex);

					int diff = currentInd - diffSize;

					if (currentInd != rememberedVertex->nextMonotoneVertex->ind) {
						monotones.front().erase(monotones.front().begin() + (diff));
						diffSize++;
					}

					currentVertex = currentVertex->nextMonotoneVertex;
					currentInd = currentVertex->ind;
				}
				monotones.back().push_back(currentVertex);
				currentVertex->nextMonotoneVertex = currentVertex->next;
			}
			monotones.front().push_back(rememberedVertex);
			rememberedVertex = rememberedVertex->next;
		} while (rememberedVertex != begin);

		for (auto & monotone : monotones) {
			auto prev = monotone.back();
			for (auto& monotoneVertex : monotone) {
				monotoneVertex->prev = prev;
				prev->next = monotoneVertex;
				prev = monotoneVertex;
			}
		}

		return monotones;
	}*/
}

template<typename T>
void splitPolygonIntoMonotones(PseudoList<Point2<T>>& polygon, std::vector<PseudoList<Point2<T>>>& monotones) {

	std::vector<PolygonVertex2<T>*> sortedVertices;
	std::vector<Edge<T>> edges;

	PseudoList<Point2<T>> pseudo_vertices{ { 1, 2 }, { 3, 4 } };
	std::vector<typename PseudoList<Point2<T>>::Node> sorted_pseudo_vertices;

	impl::initMonotonePartition(edges, polygon, sorted_pseudo_vertices); // O(n logn)
	impl::buildMonotoneDiagonals(sortedVertices, sorted_pseudo_vertices); // O (n logn)
	//monotones = impl::splitIntoMonotones(polygonVertices); // worst case 3 * (n-2)
	monotones = impl::splitIntoMonotones(sorted_pseudo_vertices, polygon);

}










#endif
