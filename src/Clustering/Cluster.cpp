#include <iostream>
#include <vector>
#include "../../include/Types.h"
#include "../../include/Clustering/Cluster.h"
#include "../../include/Clustering/Update.h"

using namespace std;


Cluster::Cluster(int center){
	this->center = center;
	this->centroid = NULL;
	this->neighbors = new Neighbors();
	this->local_objective_value = 0;
}

Cluster::Cluster(Neighbors *neigh){
	this->neighbors = neigh;
	this->local_objective_value = 0;
	this->centroid = Create_MeanFrechet(*neigh);
}

Cluster::~Cluster(){
	delete this->neighbors;
	if(this->centroid){
		delete this->centroid;
	}
}

int Cluster::Cluster_Get_Center(){
	return this->center;
}


void Cluster::Cluster_Insert(int neigh,double x){
	this->neighbors->push_back(neigh);
	this->local_objective_value +=x;
}

void Cluster::Cluster_Insert(Neighbors *neigh){
	delete this->neighbors;
	this->neighbors = neigh;
}

const Neighbors  & Cluster::Cluster_Get_Neighbors(){
	return *(this->neighbors);
}


void Cluster::Cluster_Remove_Neigh(int index_neigh){
	this->neighbors->erase(this->neighbors->begin() + index_neigh);
}


void Cluster::Cluster_Update(int new_center,double objective_value){
	this->center = new_center;
	this->local_objective_value = objective_value;
}


void Cluster::Cluster_ClearNeigh(){
	this->neighbors->clear();
}

int Cluster::Get_num_neigh(){
	return this->neighbors->size();
}

Object * Cluster::Get_Centroid(){
	return this->centroid;
}