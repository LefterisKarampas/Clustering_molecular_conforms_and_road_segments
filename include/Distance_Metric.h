#ifndef DISTANCE_METRIC_H_
#define DISTANCE_METRIC_H_

#include <vector>
#include "Types.h"
#include "Eigen/Dense"

using namespace Eigen;

template <typename T>
long double Euclidean_Distance(const T & ,const T &);


double DFT(const MatrixXd &,const MatrixXd &);
double DFT(Object &,Object &);
double DTW(Object &,Object &);
double DTW(const MatrixXd & , const MatrixXd & );
double c_RMSD(int,int);
double Frechet(int,int);
double DTW(int ,int );
double Find_Distance(int,int,double(*distance)(int,int),double ** Distance_Table);

#endif