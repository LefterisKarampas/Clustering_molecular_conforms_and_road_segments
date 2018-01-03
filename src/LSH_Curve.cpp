#include "../include/LSH_Curve.h"
#include "../include/HashFunctions.h"
#include "../include/HashTable.h"
#include "../include/Curve.h"
#include "../include/Curve_Info.h"
#include "../include/Grid.h"
#include <vector>
#include <iostream>

extern Curve_Info** curve_info;


LSH_Curve::LSH_Curve(int k,int dim,int k_vect,int num_points,int buckets,int id,int(*hash_function)(const Point &,const std::vector<int> &,int,int,std::vector<double> **,double *)){
	this->k = k;
	this->id = id;
	this->G = new Grid*[k];									//Create a array of k Grids
	for(int i =0;i<k;i++){
		this->G[i] = new Grid(dim,num_points);					//Initialize each Grid
	}
	this->HT = new HashTable(k_vect,buckets,hash_function);		//Create the LSH HashTable 
}




LSH_Curve::~LSH_Curve(){
	for(int i =0;i<this->k;i++){
		delete this->G[i];											//Delete each Grid
	}
	delete[] this->G;												//Delete the array of Grids
	delete this->HT;												//Delete the LSH HashTable
}




Point * LSH_Curve::Create_GridCurve(const Object & v){						//Get a curve and return the grid_curve
	Point *Grid_Concat;
	for(int i =0;i<this->k;i++){									//For each grid
		if(i==0){
			Grid_Concat = this->G[i]->Create_GridCurve(v);			//get the grid_curve
		}
		else{
			Point *temp = this->G[i]->Create_GridCurve(v);				//get the grid_curve
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



int LSH_Curve::LSH_Insert(int index){					//Insert a curve in the LSH HashTable
	Point* Grid_Concat = Create_GridCurve(curve_info[index]->Get_Curve());						//First get the grid_curve
	return this->HT->Hash_Insert(index,Grid_Concat);							//THen insert the new object to HT
}



void LSH_Curve::LSH_RangeSearch(int center,Object & curve,std::vector<int> *Closest_Neighbors,
	std::vector<double> *Dist,long double (*distance)( Object&, Object &)){
	Point * Grid_Concat = Create_GridCurve(curve);
	this->HT->Hash_Search(center,curve,Grid_Concat,Closest_Neighbors,Dist,distance);
}

void LSH_Curve::LSH_RangeSearch(int center,Object & curve,std::vector<int> *Closest_Neighbors,
	long double (*distance)( Object&, Object &)){
	Point * Grid_Concat = Create_GridCurve(curve);
	this->HT->Hash_Search(center,curve,Grid_Concat,Closest_Neighbors,distance);
}

/*
List * LSH_Curve::LSH_Search(Object * v,char *id,bool * flag){	 	//Search and return a List of neighbor Curves
	Point* Grid_Concat = Create_GridCurve(v);								//Create a grid_curve
	Curve * curve;
	curve = new Curve(v,Grid_Concat,id); 								
	List* result = this->HT->Hash_Search(curve,flag);				//Search into HT
	delete curve;														//Clear the used memory space to avoid memory leaks
	return result;														//return the List
}*/




								//Search for LSH nearest and true nearest neighbor
/*Curve * LSH_Curve::Check_all(Object *v,char *id,Curve * neigh,long double *neigh_dist,bool *cond,double R,std::vector<char *> *r_near,Curve *nearest_neigh,long double *nearest_dist,long double (*distance)(const Object&,const Object &)){
	Point * Grid_Concat = Create_GridCurve(v);								//Create a grid_curve
	Curve * curve;
	curve = new Curve(v,Grid_Concat,id);									//Create a Curve object that contains the Curve info
	nearest_neigh = this->HT->Check_all(curve,neigh,neigh_dist,cond,R,r_near,nearest_neigh,nearest_dist,distance);
	delete curve;														//Clear the used memory space to avoid memory leaks
	return nearest_neigh;												//Return the true nearest
}*/
