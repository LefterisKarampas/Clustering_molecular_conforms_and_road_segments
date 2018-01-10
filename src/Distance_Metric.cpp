#include <iostream>
#include "../include/Distance.h"
#include "../include/Types.h"
#include "../include/Transform.h"
#include <vector>
#include <algorithm>    // std::reverse
#include <math.h>
#include "../include/Eigen/SVD"
#include "../include/Eigen/Dense"


using namespace std;
using namespace Eigen;

extern Object_Info ** object_info;


double DFT(Object & v1,Object &v2){
	unsigned int n = v1.size();
	unsigned int m = v2.size();
	long double C[n][m];
	for(unsigned int i=0;i<n;i++){
		for(unsigned int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(v1[0],v2[0]);
			}
			else if(i == 0 && j>0){
				C[i][j] = my_max(C[i][j-1],Euclidean_Distance(v1[i],v2[j]));
			}
			else if(j == 0){
				C[i][j] = my_max(C[i-1][j],Euclidean_Distance(v1[i],v2[j]));
			}
			else{
				C[i][j] = my_max(my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1]),
					Euclidean_Distance(v1[i],v2[j])); 
			}
		}
	}
	return C[n-1][m-1];
}

double DFT(const MatrixXd & v1, const MatrixXd & v2){
	unsigned int n = v1.rows();
	unsigned int m = v2.rows();
	long double C[n][m];
	for(unsigned int i=0;i<n;i++){
		for(unsigned int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(v1.row(0),v2.row(0));
			}
			else if(i == 0 && j>0){
				C[i][j] = my_max(C[i][j-1],Euclidean_Distance(v1.row(i),v2.row(j)));
			}
			else if(j == 0){
				C[i][j] = my_max(C[i-1][j],Euclidean_Distance(v1.row(i),v2.row(j)));
			}
			else{
				C[i][j] = my_max(my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1]),
					Euclidean_Distance(v1.row(i),v2.row(j))); 
			}
		}
	}
	return C[n-1][m-1];
}

double DTW(Object &v1,Object &v2){
	unsigned int n = v1.size();
	unsigned int m = v2.size();
	long double C[n][m];
	for(unsigned int i=0;i<n;i++){
		for(unsigned int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(v1[0],v2[0]);
			}
			else if(i == 0 && j>0){
				C[i][j] = C[i][j-1] + Euclidean_Distance(v1[i],v2[j]);
			}
			else if(j == 0){
				C[i][j] = C[i-1][j] + Euclidean_Distance(v1[i],v2[j]);
			}
			else{
				C[i][j] = my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1])+Euclidean_Distance(v1[i],v2[j]); 
			}
		}
	}
	return C[n-1][m-1];
}

double DTW(const MatrixXd & v1, const MatrixXd & v2){
	unsigned int n = v1.rows();
	unsigned int m = v2.rows();
	long double C[n][m];
	for(unsigned int i=0;i<n;i++){
		for(unsigned int j=0;j<m;j++){
			if(i == 0 && j == 0){
				C[0][0] = Euclidean_Distance(v1.row(0),v2.row(0));
			}
			else if(i == 0 && j>0){
				C[i][j] = C[i][j-1] + Euclidean_Distance(v1.row(i),v2.row(j));
			}
			else if(j == 0){
				C[i][j] = C[i-1][j] + Euclidean_Distance(v1.row(i),v2.row(j));
			}
			else{
				C[i][j] = my_min(my_min(C[i-1][j],C[i-1][j-1]),C[i][j-1])+Euclidean_Distance(v1.row(i),v2.row(j)); 
			}
		}
	}
	return C[n-1][m-1];
}


double c_RMSD(int indexX,int indexY){
	MatrixXd *X;
	MatrixXd *Y;

	X = object_info[indexX]->Get_Centroid();
	if(X == NULL){
		X = Translate(indexX);
	}
	
	Y = object_info[indexY]->Get_Centroid();
	if(Y == NULL){
		Y = Translate(indexY);
	}

	MatrixXd Q;
	int n = X->rows();
	Q = Rotate(*X,*Y);
	
	return ((*X)*Q - (*Y)).norm() / (double)sqrt(n);
}

double Frechet(int indexX,int indexY){
	MatrixXd *X;
	MatrixXd *Y;

	X = object_info[indexX]->Get_Centroid();
	if(X == NULL){
		X = Translate(indexX);
	}

	Y = object_info[indexY]->Get_Centroid();
	if(Y == NULL){
		Y = Translate(indexY);
	}

	MatrixXd Q;
	Q = Rotate(*X,*Y);
	return DFT((*X)*Q,*Y);
}

double DTW(int indexX,int indexY){
	MatrixXd *X;
	MatrixXd *Y;

	X = object_info[indexX]->Get_Centroid();
	if(X == NULL){
		X = Translate(indexX);
	}

	Y = object_info[indexY]->Get_Centroid();
	if(Y == NULL){
		Y = Translate(indexY);
	}

	MatrixXd Q;
	return DTW((*X)*Q,*Y);
	Q = Rotate(*X,*Y);
}



double Find_Distance(int x,int y,double(*distance)(int,int),double ** Distance_Table){
	double dist;
	int index_b;
	int index_l;

	if(x > y){
		index_b = x;
		index_l = y;
	}
	else{
		index_b = y;
		index_l = x;
	}
	if(Distance_Table[index_b][index_l] != -1){
		dist = Distance_Table[index_b][index_l];
	}
	else{
		dist = (*distance)(index_b,index_l);
		Distance_Table[index_b][index_l] = dist;
	}
	return dist;
}