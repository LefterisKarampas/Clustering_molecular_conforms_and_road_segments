#include "../include/HashTable.h"
#include "../include/List.h"
#include "../include/Types.h"
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <vector>
#include "../include/generator.h"



//rand [M,N]
#define MIN 1
#define MAX 1000

#define W 4


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

Point * Bucket::Bucket_Insert(int index,Point *point,int pos){
	if(this->list == NULL){								//If list is empty
		this->list = new List();						//Initialize it
	}
	return this->list->List_Insert(index,point,pos);  	//Insert a new object
}

void Bucket::Size(){
	if(this-> list != NULL){
		this->list->Size();
	}
	else{
		cout << 0 << endl;
	}
}


/*void Bucket::Bucket_Search(int center,Object & object,Point * point,std::vector<int> *Closest_Neighbors,
	std::vector<double> *Dist,long double (*distance)( Object&, Object &)){
	if(this->list == NULL){								//If list is empty
		return;
	}
	this->list->List_Search(center,object,point,Closest_Neighbors,Dist,distance); 
}


void Bucket::Bucket_Search(int center,Object & object,Point * point,std::vector<int> *Closest_Neighbors,
	long double (*distance)( Object&, Object &)){
	if(this->list == NULL){								//If list is empty
		return;
	}
	this->list->List_Search(center,object,point,Closest_Neighbors,distance); 
}*/




//----------------------------HASHTABLE FUNCTIONS---------------------------------------


		//Create the HashTable for LSH
HashTable::HashTable(const int k_vect,const int n,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *)):k_vec_(k_vect),buckets(n),Hash_Function(hash_function){
	this->T = new Bucket*[n];						//Create Array of n Buckets
	for(int i=0;i<n;i++){
		this->T[i] = new Bucket();				//Initialize each bucket
	}
	if(this->k_vec_ > 0){										//If k_vec > 0, we have LSH Hash_function
		this->t = (double *) malloc(sizeof(double)*k_vec_);		//othewise classic hash function
		this->v = new std::vector<double> * [k_vec_];			//Initialize the v and t parameters for function
		for(int i=0;i<k_vec_;i++){
			this->v[i] = new std::vector<double>();				//We have to create k_vec vectors v,t
			do{
				this->t[i] = Uniform_Generator(0,1);
			}while(t[i]<0 || t[i]>=W);
			this->r.push_back( MIN + (rand() / (RAND_MAX + 1.0))*(MAX-MIN+1));
		}
	}
}

		//Create the HashTable for LSH
HashTable::HashTable(const int n):buckets(n),k_vec_(-1){
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
	if(this->k_vec_ > 0){								//If we have k_vec > 0, delete and the others structure v,t
		free(this->t);
		for(int i =0;i<this->k_vec_;i++){
			delete this->v[i];
		}
		delete[] this->v;
	}
}



//This function call the dynamic hash_function and
int HashTable::Hash(Point *point){		//return the hash_value
	if(this->k_vec_ == 0){								//If we use classic_function
		unsigned int size = point->size();
		if(size > this->r.size()){						
			unsigned int rsize = this->r.size();
			for(unsigned int i=0;i<size-rsize;i++){
				this->r.push_back( MIN + (rand() / (RAND_MAX + 1.0))*(MAX-MIN+1));	//Create more ri
			}
		}
	}
	//Call the hash_function and return the number of bucket
	return (*this->Hash_Function)(*point,this->r,this->buckets,this->k_vec_,this->v,this->t);
}




int HashTable::Hash_Insert(int index,Point *point){			//Insert a new object to HT

	int bucket; 
	bucket = this->Hash(point);							//Get the number of bucket
	if(bucket >= this->buckets){							//Check if all OK
		cerr << "Fail hash function: Index = " << bucket << endl;
		return -1;
	}
	return this->T[bucket]->Bucket_Insert(index,point);		//Insert the object to the bucket
}

Point * HashTable::Hash_Insert(int index, Point *point,int pos){
	int bucket;
	bucket = hash(*point) % this->buckets;
	//cout << "Max_bucket: "<< this->buckets <<", Bucket: " << bucket << endl;
	return this->T[bucket]->Bucket_Insert(index,point,pos);
}

void HashTable::Size(){
	for(int i=0;i<this->buckets;i++){
		cout << "Bucket: " << i << " has size: " ;
		this->T[i]->Size();
	}
}

/*
void HashTable::Hash_Search(int center,Object & object,Point * Grid_Curve,std::vector<int> *Closest_Neighbors,
	std::vector<double> *Dist,long double (*distance)( Object&, Object &)){
	int bucket = this->Hash(Grid_Curve);									//Get the number of bucket
	if(bucket >= this->buckets){								//Check if all OK
		cerr << "Fail hash function: Index = " << bucket << endl;
		return ;
	}
	this->T[bucket]->Bucket_Search(center,object,Grid_Curve,Closest_Neighbors,Dist,distance);	
}


void HashTable::Hash_Search(int center,Object & object,Point * Grid_Curve,std::vector<int> *Closest_Neighbors,
	long double (*distance)( Object&, Object &)){
	int bucket = this->Hash(Grid_Curve);									//Get the number of bucket
	if(bucket >= this->buckets){								//Check if all OK
		cerr << "Fail hash function: Index = " << bucket << endl;
		return ;
	}
	this->T[bucket]->Bucket_Search(center,object,Grid_Curve,Closest_Neighbors,distance);	
}*/
