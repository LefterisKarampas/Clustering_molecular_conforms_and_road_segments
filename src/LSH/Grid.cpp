#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "../../include/LSH/Grid.h"
#include "../../include/generator.h"
#include "../../include/Types.h"

using namespace std;

Grid::Grid(int d){
	for(int i=0;i<d;i++){
		double x;
		do{
			x = Uniform_Generator(0,1);
		}while(x<0 || x>=d);
		this->t.push_back(x);			//Create a t~[0,d)^d
	}
	this->delta = 1;
}

Grid::~Grid(){
	;
}


Point * Grid::Create_Point(const Object & v){		//Create gric_curve
	Point *vec = new Point();
	double prev;
	//for each point
	for(unsigned int k=0;k<v.size();k++){
		unsigned int size = v[k].size();
		if(size != t.size()){
			cerr << "We expected " <<t.size()<<"-dimensional curve and get " << size <<"-dimensional curve!"<<endl;
			exit(1);
		}
		//for each dimension find the point on grid and shift the point
		for(unsigned int i=0;i<size;i++){
			double temp = (int)(((v[k])[i]+0.5)/this->delta) + this->t[i];
			if(i == 0 || prev != temp)
				vec->push_back(temp);	//If point is not equal with previous, push it to the vector
			prev = temp;
		}
	}
	return vec;							//Return the grid_curve
}

