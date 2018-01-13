#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "../../include/LSH/LSH_HashTable.h"
#include "../../include/generator.h"

using namespace std;

//rand [M,N]
#define MIN 1
#define MAX 1000

#define W 4

//Create the HashTable for LSH
LSH_HashTable::LSH_HashTable(const int k_vect,const int n,
	int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *)):HashTable(n),k_vec_(k_vect),Hash_Function(hash_function){
	this->t = (double *) malloc(sizeof(double)*k_vect);		//othewise classic hash function
	this->v = new std::vector<double> * [k_vect];			//Initialize the v and t parameters for function
	for(int i=0;i<k_vect;i++){
		this->v[i] = new std::vector<double>();				//We have to create k_vec vectors v,t
		do{
			this->t[i] = Uniform_Generator(0,1);
		}while(t[i]<0 || t[i]>=W);
		this->r.push_back( MIN + (rand() / (RAND_MAX + 1.0))*(MAX-MIN+1));
	}
	
}

LSH_HashTable::~LSH_HashTable(){
	free(this->t);
	for(int i =0;i<this->k_vec_;i++){
		delete this->v[i];
	}
	delete[] this->v;
}


//This function call the dynamic hash_function and
int LSH_HashTable::Hash(Point *point){		//return the hash_value
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


int LSH_HashTable::Hash_Insert(int index,Point *point){			//Insert a new object to HT
	int bucket; 
	bucket = this->Hash(point);							//Get the number of bucket
	if(bucket >= this->buckets){							//Check if all OK
		cerr << "Fail hash function: Index = " << bucket << endl;
		return -1;
	}
	return this->T[bucket]->Bucket_Insert(index,point);		//Insert the object to the bucket
}

void LSH_HashTable::Print_Buckets(ofstream & output){
	for(int i=0;i<this->buckets;i++){
		this->T[i]->Print_List(output);
	}
}

vector<vector<int> *> LSH_HashTable::Get_Clusters(){
	vector<vector<int> *> neigh;
	for(int i=0;i<this->buckets;i++){
		vector<int> *temp = this->T[i]->Get_Values();
		if(temp != NULL){
			neigh.push_back(temp);
		}
	}
	return neigh;
}
