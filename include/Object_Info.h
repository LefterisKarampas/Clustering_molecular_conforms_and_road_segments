#ifndef _CURVE_INFO_H_
#define _CURVE_INFO_H_


#include "Types.h"
#include "../include/Eigen/Dense"

using namespace Eigen;

class Object_Info{
	Object * object;
	int flag;
	MatrixXd * centroid;
	int second_best;
public:
	Object_Info(Object *);
	~Object_Info();
	Object & Get_Object();
	Object * Get_Object_P();
	void Set_Centroid(MatrixXd *);
	MatrixXd * Get_Centroid();
	const int & Get_Flag();
	void Increase_Flag();
	void Set_second_neigh(int);
	int Get_second_neigh();
	void Clear_Flag();
};

#endif