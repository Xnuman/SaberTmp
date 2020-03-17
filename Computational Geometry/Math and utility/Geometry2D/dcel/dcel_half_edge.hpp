#pragma once

template<typename T>
struct dcelVertex;

template<typename T>
struct dcelFace;

template<typename T>
struct dcelHalfEdge {
public:
	dcelHalfEdge<T>* next, *prev, *twin;
	dcelVertex<T>* origin;
	dcelVertex<T>* end;
	dcelFace<T>* leftFace;
	T data;
	int ind;
};
