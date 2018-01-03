#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "List.h"
#include "Curve.h"
#include "Types.h"
#include <vector>
#include <iostream>

class Bucket{
private:
	List *list;		//Each Bucket has a List
public:
	Bucket();
	~Bucket();
	int Bucket_Insert(int,Point *);
	void Bucket_Search(int ,Object & ,Point *,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void Bucket_Search(int ,Object & ,Point *,std::vector<int> *,
		long double (*distance)( Object&, Object &));
	//List * Bucket_Search(Curve *,bool *);
	//void Clear_up();
	//Curve * find_nearest_min(Curve *,Curve *,long double *,bool *,double,std::vector<char *> *,Curve *,long double *,long double(*distance)(const Object&,Object&));
	//Curve * find_nearest(Curve*,Curve *,long double *,long double(*distance)(const Object &,const Object &));
};




class HashTable{
private:
	Bucket ** T;					//Array of Buckets
	//Dynamic hash_function
	int (*Hash_Function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *);
	int buckets;						//Number of buckets
	int k_vec_;
	std::vector<int> r;					//ri which used in hash_function
	std::vector<double> **v;			//v which used to calculate the LSH hash_function
	double * t;
public:
	HashTable(const int,const int,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	~HashTable();
	int Hash(Point *);
	int Hash_Insert(int,Point *);
	void Hash_Search(int ,Object & ,Point *,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void Hash_Search(int ,Object & ,Point *,std::vector<int> *,
		long double (*distance)( Object&, Object &));
	//List * Hash_Search(Curve * x,bool *);
	//void Clear_up();
	//Curve * Check_all(Curve *,Curve *,long double *,bool *,double,std::vector<char *> *,Curve *,long double *,long double(*distance)(const Object&,const Object&));
};


#endif