#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <vector>
#include "../../include/HashTable.h"
#include "../../include/List.h"
#include "../../include/Types.h"



using namespace std;

unsigned long int hash(std::vector<double>);


//----------------------------BUCKET FUNCTIONS---------------------------------------


Bucket::Bucket(){								//Initialize the Bucket
	this->list = NULL;
}



Bucket::~Bucket(){							//Delete the Bucket
	if(this->list != NULL){
		delete this->list;							//Delete the list if exists
	}
}

int Bucket::Bucket_Insert(int index,Point *point){
	if(this->list == NULL){								//If list is empty
		this->list = new List();						//Initialize it
	}
	return this->list->List_Insert(index,point);  	//Insert a new object
}

Point * Bucket::Bucket_Insert(int index,Point *point,int pos,Way **ways){
	if(this->list == NULL){								//If list is empty
		this->list = new List();						//Initialize it
	}
	return this->list->List_Insert(index,point,pos,ways);  	//Insert a new object
}

void Bucket::Size(){
	if(this-> list != NULL){
		this->list->Size();
	}
	else{
		cout << 0 << endl;
	}
}




//----------------------------HASHTABLE FUNCTIONS---------------------------------------

//Create the HashTable for LSH
HashTable::HashTable(const int n):buckets(n){
	this->T = new Bucket*[n];						//Create Array of n Buckets
	for(int i=0;i<n;i++){
		this->T[i] = new Bucket();				//Initialize each bucket
	}
}


//Delete the HashTable
HashTable::~HashTable(){
	for(int i=0;i<this->buckets;i++){
		delete this->T[i];								//Delete each bucket
	}
	delete[] this->T;									//Delete the array of buckets
}

Point * HashTable::Hash_Insert(int index, Point *point,int pos,Way **ways){
	int bucket;
	bucket = hash(*point) % this->buckets;
	return this->T[bucket]->Bucket_Insert(index,point,pos,ways);
}

void HashTable::Size(){
	for(int i=0;i<this->buckets;i++){
		cout << "Bucket: " << i << " has size: " ;
		this->T[i]->Size();
	}
}