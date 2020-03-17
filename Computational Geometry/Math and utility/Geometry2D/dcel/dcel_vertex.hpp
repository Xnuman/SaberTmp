#pragma once

template<typename T>
struct dcelHalfEdge;

template<typename T>
struct dcelFace;

template<typename T>
struct dcelVertex {
public:
	dcelHalfEdge<T>* edge;
	Point2<T> coord;
	T data;
	int ind;
};