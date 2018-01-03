#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Eigen/Dense"
#include "Types.h"

using namespace Eigen;

MatrixXd * Translate(int);
MatrixXd * Translate(Object *);
MatrixXd Rotate(MatrixXd,MatrixXd);

#endif //_TRANSFORM_H_