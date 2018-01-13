#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include "../Types.h"
#include <iostream>
#include <vector>


typedef std::vector<int> Neighbors;


class Cluster{
	int center;
	Object *centroid;
	Neighbors *neighbors;
	double local_objective_value;
public:
	Cluster(int);
	Cluster(Neighbors *);
	~Cluster();
	int Cluster_Get_Center();
	void Cluster_Insert(int,double);
	void Cluster_Insert(Neighbors *);
	const Neighbors & Cluster_Get_Neighbors();
	void Cluster_Remove_Neigh(int);
	void Cluster_Update(int,double);
	void Cluster_ClearNeigh();
	int Get_num_neigh();
	Object * Get_Centroid();
};

typedef std::vector<Cluster *> Clusters; 


#endif