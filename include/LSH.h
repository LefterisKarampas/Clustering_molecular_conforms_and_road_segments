#ifndef LSH_CURVE_H_
#define LSH_CURVE_H_

#include "Grid.h"
#include "LSH_HashTable.h"
#include "Types.h"
#include <vector>
#include <iostream>

class LSH{
private:
	Grid** G;						//Array of Grids
	LSH_HashTable *HT;					//LSH HashTAble
	int k;								//Pointumber of k Grids
	int id;								//LSH id number
public:
	LSH(int,int,int,int,int,int,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	~LSH();
	Point * Create_Point(const Object &);
	int LSH_Insert(int);
};


#endif