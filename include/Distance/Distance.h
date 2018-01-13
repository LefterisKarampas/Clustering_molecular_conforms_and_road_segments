#ifndef DISTANCE_H_
#define DISTANCE_H_

#include <iostream>
#include <vector>

template <typename T>
double Euclidean_Distance(const T & ,const T &);

template <typename T>
T my_max(T x, T y){
	return (x>y?x:y);
}

template <typename T>
T my_min(T x, T y){
	return (x>y?y:x);
}



#endif