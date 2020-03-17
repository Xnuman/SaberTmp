#pragma once

template<typename T>
struct dcelHalfEdge;

template<typename T>
struct dcelVertex;

template<typename T>
struct dcelFace {
public:
	dcelHalfEdge<T>* boundary;
	T data;
	int ind;
};