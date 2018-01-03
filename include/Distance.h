#ifndef DISTANCE_H_
#define DISTANCE_H_

#include <vector>
#include "Types.h"
#include "Eigen/Dense"

using namespace Eigen;

double DFT(const MatrixXd &,const MatrixXd &);
double DFT(Object &,Object &);
double DTW(Object &,Object &);
double DTW(const MatrixXd & , const MatrixXd & );
Object * MeanFrechet(Object *,Object *,double *dist);
double c_RMSD(int,int);
double Frechet(int,int);
double DTW(int ,int );
double Find_Distance(int,int,double(*distance)(int,int),double ** Distance_Table);

#endif