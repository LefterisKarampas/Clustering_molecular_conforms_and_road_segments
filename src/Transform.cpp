#include "../include/Eigen/SVD"
#include "../include/Eigen/Dense"
#include "../include/Object_Info.h"
#include "../include/Types.h"
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
using namespace Eigen;

extern Object_Info ** object_info;

MatrixXd * Translate(int index){
	Object obj = object_info[index]->Get_Object();
	Point centroid;
	int dim;
	unsigned int i;
	unsigned int j;
	unsigned int size = obj.size();

	//Sum all 
	for(i=0;i<size;i++){
		for(j =0;j<obj[i].size();j++){
			if(i == 0){
				centroid.push_back(obj[i][j]);
			}
			else{
				centroid[j] += obj[i][j];
			}
		}
		if(i == 0){
			dim = j;
		}
	}

	//Find centroid
	for(j=0;j<dim;j++){
		centroid[j] = centroid[j] / (double)size;
	}

	MatrixXd * new_object = new MatrixXd(size,dim);
	//Translate it
	for(i=0;i<size;i++){
		Point point;
		for(j=0;j<obj[i].size();j++){
			(*new_object)(i,j) = (obj[i][j] - centroid[j]);
		}
	}
	object_info[index]->Set_Centroid(new_object);
	return new_object;
}


MatrixXd * Translate(Object *object){
	Point centroid;
	int dim;
	unsigned int i;
	unsigned int j;
	unsigned int size = object->size();

	//Sum all 
	for(i=0;i<size;i++){
		for(j =0;j<(*object)[i].size();j++){
			if(i == 0){
				centroid.push_back((*object)[i][j]);
			}
			else{
				centroid[j] += (*object)[i][j];
			}
		}
		if(i == 0){
			dim = j;
		}
	}

	//Find centroid
	for(j=0;j<dim;j++){
		centroid[j] = centroid[j] / (double)size;
	}

	MatrixXd * new_object = new MatrixXd(size,dim);
	//Translate it
	for(i=0;i<size;i++){
		Point point;
		for(j=0;j<(*object)[i].size();j++){
			(*new_object)(i,j) = ((*object)[i][j] - centroid[j]);
		}
	}
	return new_object;
}




MatrixXd Rotate(MatrixXd X,MatrixXd Y){
 	MatrixXd m = X.transpose() * Y;
	JacobiSVD<MatrixXd> svd(m, ComputeThinU | ComputeThinV);
	if((svd.singularValues())(2) <= 0 ){
		cerr << "S3 <= 0" << endl;
		exit(1);
	}
	MatrixXd U = svd.matrixU();
	MatrixXd Q = U * svd.matrixV().transpose();
	if(Q.determinant() < 0){
		for(unsigned int j =0;j<Q.cols();j++){
			U(2,j) = -U(2,j);
		}
		Q = U * svd.matrixV().transpose();
	}
	return Q;
}
