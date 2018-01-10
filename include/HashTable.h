#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "List.h"
#include "Types.h"
#include <vector>
#include <iostream>
#include "Ways.h"


class Bucket{
private:
	List *list;		//Each Bucket has a List
public:
	Bucket();
	~Bucket();
	int Bucket_Insert(int,Point *);
	Point * Bucket_Insert(int,Point *,int,Way **);
	void Size();
};


class HashTable{
protected:
	Bucket ** T;					//Array of Buckets
	int buckets;						//Number of buckets
public:
	HashTable(const int);
	~HashTable();
	Point * Hash_Insert(int,Point *,int,Way **);
	void Size();
};


#endif