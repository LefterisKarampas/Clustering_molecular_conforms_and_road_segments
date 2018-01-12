#include <iostream>
#include <vector>
#include <math.h>

using namespace std;


template <typename T>
T my_max(T x, T y){
	return (x>y?x:y);
};

template <typename T>
T my_min(T x, T y){
	return (x>y?y:x);
};


template <typename T>
double Euclidean_Distance(const T & v1,const T &v2){
 	long double sum = 0;
 	for(unsigned int i = 0;i<v1.size();i++){
 		sum += pow((v1[i] - v2[i]),2);
 	}
 	return sqrt(sum);
 };











