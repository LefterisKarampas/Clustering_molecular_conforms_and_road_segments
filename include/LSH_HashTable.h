#ifndef _LSH_HASH_H_
#define _LSH_HASH_H_

#include "HashTable.h"
#include "Types.h"

class LSH_HashTable: public HashTable{
	int (*Hash_Function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *);
	int k_vec_;
	std::vector<int> r;					//ri which used in hash_function
	std::vector<double> **v;			//v which used to calculate the LSH hash_function
	double * t;
public:
	LSH_HashTable(const int,const int,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	~LSH_HashTable();
	int Hash(Point *);
	int Hash_Insert(int,Point *);
};

#endif