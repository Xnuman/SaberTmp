#ifndef XNUMER_GEOMETRY_EDGE_IMPL_HPP
#define XNUMER_GEOMETRY_EDGE_IMPL_HPP

template<typename T>
Edge<T>::Edge(const Point2<T>& a, const Point2<T>& b) : top(a), down(b) {
	if (top.getY() < down.getY()) {
		utility::swap(top, down);
	}
}

template<typename T>
Point2<T> edgeIntersectLineY(const Edge<T>& e, const T& y) {
	T x = (y * (e.top.getX() - e.down.getX()) +
		e.down.getX() * e.top.getY() -
		e.down.getY() * e.top.getX()) /
		(e.top.getY() - e.down.getY());

	return Point2<T>(x, y);
}

#endif
