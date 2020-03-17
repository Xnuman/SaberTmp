#ifndef XNUMER_GEOMETRY_POLYGON2_IMPL_HPP
#define XNUMER_GEOMETRY_POLYGON2_IMPL_HPP

template<typename T>
PolygonVertex2<T>::PolygonVertex2(const Point2<T>& firstVertex) : coordinates(firstVertex), next(this), prev(this), ind(0) {}

template<typename T>
PolygonVertex2<T>* PolygonVertex2<T>::insert(PolygonVertex2<T>* newVertex) {
		PolygonVertex2<T>* tmp = next;
		newVertex->next = tmp;
		newVertex->prev = this;
		newVertex->ind = this->ind + 1;
		next = newVertex;
		return newVertex;
}




#endif
