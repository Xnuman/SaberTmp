#ifndef XNUMER_SEARCH_RECTANGLE_POINT_2_IMPL_HPP
#define XNUMER_SEARCH_RECTANGLE_POINT_2_IMPL_HPP

#include <cmath>

template<typename T>
Point2<T>::Point2() : x(T(0)), y(T(0)) {}

template<typename T>
Point2<T>::Point2(const T& x, const T& y) : x(x), y(y) {}

template<typename T>
Point2<T>::Point2(std::pair<T, T> p) : x(p.first, p.second) {}

template<typename T>
Point2<T>::Point2(const Point2<T>& other) : x(other.x), y(other.y) {}

template<typename T>
Point2<T>::Point2(Point2<T>&& other) {
	x = std::move(other.x);
	y = std::move(other.y);
}

template<typename T>
Point2<T> Point2<T>::operator-() {
	return Point2(-x, -y);
}

template<typename T>
Point2<T>& Point2<T>::operator=(const Point2<T>& other) {
	if (&other != this) {
		x = other.x;
		y = other.y;
	}
	return *this;
}

template<typename T>
Point2<T>& Point2<T>::operator=(Point2<T>&& other) {
	x = std::move(other.x);
	y = std::move(other.y);
	return *this;
}

template<typename T>
Point2<T>& Point2<T>::operator+=(const Point2& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template<typename T>
Point2<T>& Point2<T>::operator-=(const Point2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template<typename V, typename G>
using enableIfArithmetic = typename std::enable_if_t<std::is_arithmetic_v<V>, G>;

template<typename T>
template<typename V>
enableIfArithmetic<V, Point2<T>&> Point2<T>::operator+=(const V& rhs) {
	x += static_cast<T>(rhs);
	y += static_cast<T>(rhs);
	return *this;
}

template<typename T>
template<typename V>
enableIfArithmetic<V, Point2<T>&> Point2<T>::operator-=(const V& rhs) {
	return operator+=(-rhs);
}

template<typename T>
template<typename V>
enableIfArithmetic<V, Point2<T>&> Point2<T>::operator*=(const V& rhs) {
	x *= rhs;
	y *= rhs;
	return *this;
}

template<typename T>
bool Point2<T>::operator==(const Point2<T>& other) const {
	return ((this->x == other.x) && (this->y == other.y));
}

template<typename T>
bool Point2<T>::operator!=(const Point2<T>& other) const {
	return !operator==(other);
}

template<typename T>
Point2<T> operator+(Point2<T> lhs, const Point2<T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T, typename V>
enableIfArithmetic<V, Point2<T>> operator+(Point2<T> lhs, const V& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
Point2<T> operator-(Point2<T> lhs, const Point2<T>& rhs) {
	return lhs + (-rhs);
}

template<typename T, typename V>
enableIfArithmetic<V, Point2<T>> operator-(Point2<T> lhs, const V& rhs) {
	return lhs + (-rhs);
}

template <typename T, typename V>
enableIfArithmetic<V, Point2<T>> operator*(Point2<T> lhs, V rhs) {
	return lhs *= rhs;
}

template <typename T, typename V>
enableIfArithmetic<V, Point2<T>> operator*(V lhs, Point2<T> rhs) {
	return rhs * lhs;
}

template<typename T>
float dist(const Point2<T>& a, const Point2<T>& b) {
	return std::sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

template<typename T>
T Point2<T>::getX() const {
	return x;
}

template<typename T>
T Point2<T>::getY() const {
	return y;
}

template<typename T>
void Point2<T>::setX(const T& x) {
	this->x = x;
}

template<typename T>
void Point2<T>::setY(const T& y) {
	this->y = y;
}


#endif
