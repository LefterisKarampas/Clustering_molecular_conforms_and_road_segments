#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "List.h"
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
	Point * Bucket_Insert(int,Point *,int);
	void Size();
	/*void Bucket_Search(int ,Object & ,Point *,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void Bucket_Search(int ,Object & ,Point *,std::vector<int> *,
		long double (*distance)( Object&, Object &));*/
};




class HashTable{
private:
	Bucket ** T;					//Array of Buckets
	//Dynamic hash_function
	int (*Hash_Function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *);
	int buckets;						//Number of buckets
	int k_vec_;
	//LSH Hash Extras
	std::vector<int> r;					//ri which used in hash_function
	std::vector<double> **v;			//v which used to calculate the LSH hash_function
	double * t;
public:
	HashTable(const int,const int,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *));
	HashTable(const int);
	~HashTable();
	int Hash_Insert(int,Point *);
	Point * Hash_Insert(int,Point *,int);
	void Size();
	//LSH Hash Extras
	int Hash(Point *);
	/*void Hash_Search(int ,Object & ,Point *,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void Hash_Search(int ,Object & ,Point *,std::vector<int> *,
		long double (*distance)( Object&, Object &));*/
};


#endif