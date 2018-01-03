#include "../include/Object_Info.h"
#include <stdlib.h>
#include "../include/Eigen/Dense"

using namespace Eigen;


Object_Info::Object_Info(Object *object){
	this->object = object;
	this->flag = 0;
	this->centroid = NULL;
	this->second_best = -1;
}

Object_Info::~Object_Info(){
	delete this->object;
	if(this->centroid != NULL){
		delete this->centroid;
	}
}

void Object_Info::Set_Centroid(MatrixXd * centroid){
	this->centroid = centroid;
}

MatrixXd * Object_Info::Get_Centroid(){
	return this->centroid;
}

Object & Object_Info::Get_Object(){
	return *(this->object);
}

Object * Object_Info::Get_Object_P(){
	return this->object;
}


const int & Object_Info::Get_Flag(){
	return this->flag;
};


void Object_Info::Increase_Flag(){
	this->flag++;
}

void Object_Info::Set_second_neigh(int second_best){
	this->second_best = second_best;
}

int Object_Info::Get_second_neigh(){
	return this->second_best;
}

void Object_Info::Clear_Flag(){
	this->flag = 0;
}