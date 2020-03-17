#include "polygon2.hpp"
#include "monotone_partition.hpp"
#include "monotone_triangulation.hpp"
#include "../../../Math and utility/utility_functions.hpp"
#include <vector>
#include <iostream>
#include "../../../Math and utility/pseudo_list.hpp"

template<typename T>
struct abba {
	void begin() {}
	void end() {}
	void size() {}
	using value_type = T;
};

template<typename T>
class A {
public:
	T data;
	template<typename V, typename = typename std::enable_if_t<is_valid_container<V,T>::value>>
	A(V container) {
		for (auto a : container) {
			std::cout << a << std::endl;
		}
	}
};

int main() {

	std::vector<Point2<int>> vec{ Point2<int>{1, 1} };


	PseudoList<Point2<int>> list3{ { 1, 2 } };
	auto node = list3.push_front({ 1, 1 });
	list3.push_front({ 1, 3 });
	
	std::cout << node.data().getX() << ", " << node.data().getY() << std::endl;
	std::cout << node.next().data().getX() << ", " << node.next().data().getY() << std::endl;
	std::cout << node.prev().data().getX() << ", " << node.prev().data().getY() << std::endl;

	//for (int i = 0; i != list3.size(); ++i) {

	//}

	std::cout << std::boolalpha;
	std::cout << is_valid_container<decltype(vec), Point2<int>>::value << std::endl;



	std::vector<PolygonVertex2<int>> vertexVector{
		PolygonVertex2{ Point2<int>{15, 4} },
		PolygonVertex2{ Point2<int> {10, 13} },
		PolygonVertex2{ Point2<int> {8, 9} },
		PolygonVertex2{ Point2<int> {6, 11} },
		PolygonVertex2{ Point2<int> {3, 8} },
		PolygonVertex2{ Point2<int> {5, 5} },
		PolygonVertex2{ Point2<int> {6, 7} },
		PolygonVertex2{ Point2<int> {7, 1} },
		PolygonVertex2{ Point2<int> {9, 4} },
		PolygonVertex2{ Point2<int> {11, 0} }
	};

	PolygonVertex2<int>* last = &vertexVector[0];

	utility::range_based_wrapper range(vertexVector.begin() + 1, vertexVector.end());

	for (auto& i : range) {
		last = last->insert(&i);
	}

	last->next = &vertexVector[0];
	last->next->prev = last;


	PolygonVertex2<int>* v = last->next;
	PolygonVertex2<int>* tmp = v;
	do {
		tmp->nextMonotoneVertex = tmp->next;
		tmp = tmp->next;
	} while (tmp != v);

	/*std::vector<std::vector<PolygonVertex2<int>*>> monotones;*/
	std::vector<PseudoList<Point2<int>>> monotones;

	PseudoList<Point2<int>> polygon {
		{ 15, 4 },
	{ 10, 13 },
	{ 8, 9 },
	{ 6, 11 },
	{ 3, 8 },
	{ 5, 5 },
	{ 6, 7 },
	{ 7, 1 },
	{ 9, 4 },
	{ 11, 0 }
	};

	splitPolygonIntoMonotones(polygon, monotones);

	std::vector<std::pair<Point2<int>, Point2<int>>> monotoneTriang;
	uint32_t counter = 0;
	for (int i = 0; i != monotones.size(); ++i) {
		std::cout << ++counter << "-th monotone: " << std::endl;
		auto vertex = monotones[i].front();
		do {
			std::cout << "(" << vertex.data().getX() << ", " << vertex.data().getY() << ")" << std::endl;
			vertex = vertex.next();
		} while (vertex != monotones[i].front());
		std::cout << std::endl;
	}

	//for (auto& monotone : monotones) {
	//	monotoneTriangulation(monotone, monotoneTriang);
	//	for (auto& edge : monotoneTriang) {
	//		std::cout << "(" << edge.first.getX() << ", " << edge.first.getY() << ") -- "
	//			<< "(" << edge.second.getX() << ", " << edge.second.getY() << ")" << std::endl;
 //		}
	//}

	//int counter = 0;
	//for (auto& monotone : monotones) {
	//	++counter;
	//	std::cout << counter << "-th monotone:" << std::endl;
	//	for (auto& vertex : monotone) {
	//	//	std::cout << "{" << vertex->coordinates.getX() << ", " << vertex->coordinates.getY() << "}";
	//		std::cout << vertex->ind << ", ";
	//	}
	//	std::cout << std::endl;
	//}

}