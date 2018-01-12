#include <vector>
#include <iostream>
#include "../../include/LSH.h"
#include "../../include/HashFunctions.h"
#include "../../include/LSH_HashTable.h"
#include "../../include/Types.h"
#include "../../include/Object_Info.h"
#include "../../include/Grid.h"

extern Object_Info** object_info;


LSH::LSH(int k,int dim,int k_vect,int num_points,int buckets,int id,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	this->k = k;
	this->id = id;
	this->G = new Grid*[k];									//Create a array of k Grids
	for(int i =0;i<k;i++){
		this->G[i] = new Grid(dim,num_points);					//Initialize each Grid
	}
	this->HT = new LSH_HashTable(k_vect,buckets,hash_function);		//Create the LSH HashTable 
}




LSH::~LSH(){
	for(int i =0;i<this->k;i++){
		delete this->G[i];											//Delete each Grid
	}
	delete[] this->G;												//Delete the array of Grids
	delete this->HT;												//Delete the LSH HashTable
}




Point * LSH::Create_Point(const Object & v){						//Get a curve and return the grid_curve
	Point *Grid_Concat;
	for(int i =0;i<this->k;i++){									//For each grid
		if(i==0){
			Grid_Concat = this->G[i]->Create_Point(v);			//get the grid_curve
		}
		else{
			Point *temp = this->G[i]->Create_Point(v);				//get the grid_curve
			unsigned int size = Grid_Concat->size();
			bool cond = false;
			for(unsigned int i=0;i<temp->size();i++){				//Concat the grid_curves without have 2 consecutive points
				if(cond || (((*Grid_Concat)[size-1]) != (*temp)[i])){
					cond = true;
					Grid_Concat->push_back((*temp)[i]);
				}
			}
			delete temp;
		}
	}
	return Grid_Concat;
}



int LSH::LSH_Insert(int index){					//Insert a curve in the LSH HashTable
	Point* Grid_Concat = Create_Point(object_info[index]->Get_Curve());						//First get the grid_curve
	return this->HT->Hash_Insert(index,Grid_Concat);							//THen insert the new object to HT
}
