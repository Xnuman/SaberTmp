#ifndef XNUMER_SEARCH_RECTANGLE_PROJECT_POINT_2_HPP
#define XNUMER_SEARCH_RECTANGLE_PROJECT_POINT_2_HPP

#include <type_traits>
#include <utility>

template<typename T>
class Point2 {
public:
	Point2();
	Point2(const T& x, const T& y);
	Point2(std::pair<T, T> p);
	Point2(const Point2& other);
	Point2(Point2&& other);

	T getX() const;
	T getY() const;
	void setX(const T& x);
	void setY(const T& y);

	Point2 operator-();

	Point2& operator=(const Point2& other);
	Point2& operator=(Point2&& other);

	Point2& operator+=(const Point2& rhs);
	Point2& operator-=(const Point2& rhs);

	template<typename V, typename G>
	using enableIfArithmetic = typename std::enable_if_t<std::is_arithmetic_v<V>, G>;

	template<typename V>
	enableIfArithmetic<V, Point2&> operator+=(const V& rhs);

	template<typename V>
	enableIfArithmetic<V, Point2&> operator-=(const V& rhs);

	template<typename V>
	enableIfArithmetic<V, Point2&> operator*=(const V& rhs);

	bool operator==(const Point2& other) const;
	bool operator!=(const Point2& other) const;

	friend Point2 operator+(Point2 lhs, const Point2& rhs);

	template<typename V>
	friend enableIfArithmetic<V, Point2> operator+(Point2 lhs, const V& rhs);

	friend Point2 operator-(Point2 lhs, const Point2& rhs);

	template<typename V>
	friend enableIfArithmetic<V, Point2> operator-(Point2 lhs, const V& rhs);

	template <typename V>
	friend enableIfArithmetic<V, Point2> operator*(Point2 lhs, V rhs);

	template<typename V>
	friend enableIfArithmetic<V, Point2> operator*(V lhs, Point2 rhs);

	friend float dist(const Point2& a, const Point2& b);

private:
	T x, y;
};

#include "point2_impl.hpp"

#endif // !XNUMER_POINT_2_HPP

