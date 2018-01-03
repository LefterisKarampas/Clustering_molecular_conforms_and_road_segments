#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/Curve.h"

using namespace std;

//Create a Curve object

Curve::Curve(Object * curve_,Point * grid_curve_,char *id_):curve(curve_),grid_curve(grid_curve_){
	this->id = id_;
}


Curve::~Curve(){				//Delete Curve object
	delete this->grid_curve;						//Delete only the grid_curve, id and curve maybe are used from other LSH HT
}	



const Point & Curve::Get_GridCurve(){
	return *(this->grid_curve);
}


bool Curve::Compare_GridCurve(Curve *v){
	if(!strcmp(v->GetId(),this->GetId())){
		return false;
	}
	Point t1 = v->Get_GridCurve();
	if(t1.size() != this->grid_curve->size()){
		return false;
	}
	else{
		for(unsigned int i=0;i<t1.size();i++){
			if(t1[i] != (*this->grid_curve)[i]){
				return false;
			}
		}
		return true;
	}
}


char * Curve::GetId(){
	return this->id;
}



Object & Curve::GetCurve(){
	return *(this->curve);
}



void Curve::Clear_up(){
	delete this->curve;
	free(this->id);
}

