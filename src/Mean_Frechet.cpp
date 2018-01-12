#include <iostream>
#include "../include/Distance.h"
#include "../include/Types.h"
#include "../include/Transform.h"
#include <vector>
#include <algorithm>    // std::reverse
#include <math.h>
#include "../include/Eigen/Dense"

using namespace Eigen;
using namespace std;

template <typename T>
void Mean(T x1,T x2,Point *p){
	for(unsigned int i=0;i<x1.size();i++){
		double mid;
		mid = (x2[i] + x1[i]) /2.0; 
		p->push_back(mid);
	}
}

int find_min(double x1,double x2,double x3){
	if(x1<x2){
		if(x1<x3){
			return 0;
		}
		else{
			return 2;
		}
	}
	else if(x2<x3){
		return 1;
	}
	return 2;
}

Object * MeanFrechetMatrix(const MatrixXd &x,const MatrixXd &y,double *dist){
	unsigned int n = x.rows();
	unsigned int m = y.rows();
	double C[n][m];
	for(unsigned int i=0;i<n;i++){
		for(unsigned int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(x.row(0),y.row(0));
			}
			else if(i == 0 && j>0){
				C[i][j] = my_max(C[i][j-1],Euclidean_Distance(x.row(i),y.row(j)));
			}
			else if(j == 0){
				C[i][j] = my_max(C[i-1][j],Euclidean_Distance(x.row(i),y.row(j)));
			}
			else{
				C[i][j] = my_max(my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1]),
					Euclidean_Distance(x.row(i),y.row(j))); 
			}
		}
	}
	*dist = C[n-1][m-1];

	//Find the Mean Frechet Object
	unsigned int i = n-1;
	unsigned int j = m-1;
	Object * traversal = new Object;
	while(i != 0 && j != 0){
		Point p ;
		Mean(x.row(i),y.row(j),&p);
		traversal->push_back(p);
		int min_index;
		min_index = find_min(C[i-1][j],C[i][j-1],C[i-1][j-1]);
		if(min_index == 0){
			i--;
		}
		else if(min_index == 1){
			j--;
		}
		else{
			i--;
			j--;
		}
	}
	if(i != 0){
		while(i > 0){
			Point p;
			Mean(x.row(i),y.row(j),&p);
			traversal->push_back(p);
			i--;
		}
	}
	else{
		while(j > 0){
			Point p;
			Mean(x.row(i),y.row(j),&p);
			traversal->push_back(p);
			j--;
		}
	}
	Point p;
	Mean(x.row(0),y.row(0),&p);
	traversal->push_back(p);
  	std::reverse(traversal->begin(),traversal->end());
	return traversal;
}

Object * MeanFrechet(Object *v1,Object *v2,double *dist){
	if(v2 == NULL){
		Object *obj = new Object;
		for(unsigned int i=0;i<v1->size();i++){
			Point p;
			for(unsigned int j=0;j<(*v1)[i].size();j++){
				p.push_back((*v1)[i][j]);
			}
			obj->push_back(p);
		}
		return obj;
	}

	MatrixXd *X;
	MatrixXd *Y;
	//Translate them
	cout << "Translate" << endl;
	X = Translate(v1);
	Y = Translate(v2);
	cout << "After Translate" << endl;
	//Rotate
	MatrixXd Q;
	cout << "Rotate"  << endl;
	Q = Rotate(*X,*Y);
	cout <<"After Rotate" << endl;
	//Create Frechet Matrix 
	cout << "HERE"  << endl;
	Object *obj = MeanFrechetMatrix((*X)*Q,*Y,dist);
	cout << "OUT"  << endl;
	delete X;
	delete Y;
	return obj;
}