#ifndef XNUMER_GEOMETRY_MONOTONE_TRIANGULATION_IMPL_HPP
#define XNUMER_GEOMETRY_MONOTONE_TRIANGULATION_IMPL_HPP

#include "../../../Math and utility/utility_functions.hpp"
#include <algorithm>
#include <stack>
#include "edge.hpp"

template<typename T>
void monotoneTriangulation(std::vector<PolygonVertex2<T>*>& monotone, std::vector<std::pair<Point2<T>, Point2<T>>>& triangulation) {
	std::sort(monotone.begin(), monotone.end(), [&](PolygonVertex2<T>*& a, PolygonVertex2<T>*& b) {
		if (a->coordinates.getY() == b->coordinates.getY()) {
			return a->coordinates.getX() < b->coordinates.getX();
		}
		else {
			return a->coordinates.getY() > b->coordinates.getY();
		}
		});

	if (monotone.size() == 3) {
		triangulation = {
			std::pair<Point2<T>, Point2<T>>{(monotone[0])->coordinates, (monotone[1])->coordinates},
			std::pair<Point2<T>, Point2<T>>{(monotone[1])->coordinates, (monotone[2])->coordinates},
			std::pair<Point2<T>, Point2<T>>{(monotone[2])->coordinates, (monotone[0])->coordinates}
		};
		return;
	}

	std::stack<PolygonVertex2<T>*> S;
	S.push(monotone[0]);
	S.push(monotone[1]);

	auto polygon = utility::range_based_wrapper(monotone.begin() + 2, monotone.end());

	for (auto vertex : polygon) {
		if (vertex->prev->ind != S.top()->ind) {
			while (S.size() > 0) {
				if (S.size() != 1) {
					std::pair<Point2<T>, Point2<T>> e{ vertex->coordinates, (S.top())->coordinates };
					triangulation.push_back(e);
				}
				S.pop();
			}
			S.push(vertex->prev);
			S.push(vertex);
		}
		else {
			PolygonVertex2<T>* last = S.top();
			S.pop();
			while (isValidDiagonal(vertex, S.top(), last)) {
				last = S.top();
				S.pop();
				triangulation.push_back(std::pair<Point2<T>, Point2<T>>{vertex->coordinates, last->coordinates});
			}
			S.push(last);
			S.push(vertex);
		}
	}
	//S.pop();
	//while (S.size() > 0) {
	//	if (S.size() != 1) {
	//		triangulation.push_back(Edge<T>{vertex->coordinates, S.top()})
	//	}
	//	S.pop();
	//}

}

template<typename T>
bool isValidDiagonal(PolygonVertex2<T>*& e1, PolygonVertex2<T>*& e2, PolygonVertex2<T>*& v) {
	if (e1->ind < v->ind) {
		return isRightTurn(e1->coordinates, e2->coordinates, v->coordinates);
	}
	else {
		return isLeftTurn(e1->coordinates, e2->coordinates, v->coordinates);
	}
}

#endif

