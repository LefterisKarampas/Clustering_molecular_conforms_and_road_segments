#ifndef LSH_CURVE_H_
#define LSH_CURVE_H_

#include "Grid.h"
#include "HashTable.h"
#include "List.h"
#include "Types.h"
#include <vector>
#include <iostream>

class LSH_Curve{
private:
	Grid** G;						//Array of Grids
	HashTable *HT;					//LSH HashTAble
	int k;								//Pointumber of k Grids
	int id;								//LSH id number
public:
	LSH_Curve(int,int,int,int,int,int,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	~LSH_Curve();
	Point * Create_GridCurve(const Object &);
	int LSH_Insert(int);
	void LSH_RangeSearch(int ,Object & ,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void LSH_RangeSearch(int ,Object & ,std::vector<int> *,
		long double (*distance)( Object&, Object &));
	//List * LSH_Search(Object *,char *,bool *);
	//Curve * Check_all(Object *,char *,Curve * ,long double *,bool *,double,std::vector<char *> *,Curve *,long double *,long double (*distance)(const Object &,const Object &));
};


#endif