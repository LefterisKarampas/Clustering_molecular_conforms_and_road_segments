#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>
#include "../../include/Clustering/Assignment.h"
#include "../../include/Clustering/Object_Info.h"
#include "../../include/Clustering/Cluster.h"
#include "../../include/Distance/Distance_Metric.h"


using namespace std;

extern Object_Info ** object_info;
extern double ** Distance_Table;

double Lloyd_Assignment(Clusters *clusters,int n,double (*distance)(int,int)){
	if(n <= 0){
		cerr << "We don't have any curve\n" << endl;
	}
	double objective_value = 0;
	int flag = 0;
	int t;

	//First assignment
	if(object_info[0]->Get_Flag() == 0){
		//For each object
		for(int i=0;i<n;i++){
			int min;
			double min_dist;
			int second_best;
			double second_dist;
			//For each cluster
			for(unsigned int j=0;j<clusters->size();j++){
				//Get cluster center
				int center = (*clusters)[j]->Cluster_Get_Center();
				//Compute the distance between object and cluster center
				double dist;
				if(center == i){
					flag = 1;
					t = j;
					dist = 0;
				}
				else{
					dist = Find_Distance(center,i,distance,Distance_Table);
				}
				//Hold the min distance and the second min
				if(j==0){
					min = j;
					min_dist = dist;
				}
				else if(min_dist > dist){
					second_best = min;
					second_dist = min_dist;
					min_dist = dist;
					min = j;
				}
				else if(j == 1 || dist < second_dist){
					second_best = j;
					second_dist = dist;
				}
			}
			//In the case where object is center and has min_distance with another center
			//Change min to current cluster and second best with the other cluster
			//This only will happen after Rotate and have only 1 point
			if(flag && min != t){
				(*clusters)[t]->Cluster_Insert(i,min_dist);
				objective_value += min_dist;
				object_info[i]->Increase_Flag();
				object_info[i]->Set_second_neigh(min);
			}
			else{
				(*clusters)[min]->Cluster_Insert(i,min_dist);
				objective_value += min_dist;
				object_info[i]->Increase_Flag();
				object_info[i]->Set_second_neigh(second_best);
			}
			flag = 0;
		}
	}
	//Already Initialization Clusters
	else{
		//Create new neighbors
		Neighbors *new_neigh[clusters->size()];
		for(unsigned int i=0;i<clusters->size();i++){
			new_neigh[i] = new Neighbors();
		}
		//For each cluters
		for(unsigned int i=0;i<clusters->size();i++){
			//Get its neighbors
			std::vector<int> neigh = (*clusters)[i]->Cluster_Get_Neighbors();
			//For each object in cluster
			for(unsigned int j=0;j<neigh.size();j++){
				int min;
				double min_dist;
				int second_best;
				double second_dist;
				//For each cluster
				for(unsigned int k=0;k<clusters->size();k++){
					//Get the cluster's center
					int center = (*clusters)[k]->Cluster_Get_Center(); 
					//Find the distance between object and center
					double dist;
					if(center == neigh[j]){
						dist = 0;
						flag = 1;
						t = k;
					}
					else{
						dist = Find_Distance(center,neigh[j],distance,Distance_Table);
					}
					//Hold the cluster with the min distance from object
					//and the second_min
					if(k==0){
						min = k;
						min_dist = dist;
					}
					else if(min_dist > dist){
						second_best = min;
						second_dist = min_dist;
						min_dist = dist;
						min = k;
					}
					else if(k == 1 || dist < second_dist){
						second_best = k;
						second_dist = dist;
					}
				}
				//In the case where object is center and has min_distance with another center
				//Change min to current cluster and second best with the other cluster
				//This only will happen after Rotate and have only 1 point
				if(flag && (min != t)){
					new_neigh[t]->push_back(neigh[j]);
					objective_value += min_dist;
					object_info[neigh[j]]->Set_second_neigh(min);
					object_info[neigh[j]]->Increase_Flag();
				}
				else{
					new_neigh[min]->push_back(neigh[j]);
					objective_value += min_dist;
					object_info[neigh[j]]->Set_second_neigh(second_best);
					object_info[neigh[j]]->Increase_Flag();
				}
				flag = 0;
			}
		}
		//For each cluster update neighbors
		for(unsigned int i=0;i<clusters->size();i++){
			(*clusters)[i]->Cluster_Insert(new_neigh[i]);
		}
	}
	//Return objective_value
	return objective_value;
}


//LSH_RangeSearch Assignment has not a good convergence, just let it for future work!

/*double LSH_RangeSearch_Assignment(Clusters *clusters,int n,LSH_Curve ** LSH,int num_HT,double (*distance)(int,int)){
	int flag = 0;
	int num_clusters = clusters->size();
	int center;
	int t = object_info[0]->Get_Flag();

	std::vector<int> *Closest_Neighbors = new std::vector<int>[num_clusters];
	std::vector<double> *Dist;
	if((*clusters)[0].Cluster_Get_Center() < 0){
		Dist =  new std::vector<double>[num_clusters];
	}
	//For each cluster, find the LSH Neighbors
	for(int i=0;i<num_clusters;i++){
		(*clusters)[i].Cluster_ClearValue();
		(*clusters)[i].Cluster_ClearNeigh();
		int center = (*clusters)[i].Cluster_Get_Center();
		for(int j=0;j<num_HT;j++){
			if(center < 0){
				LSH[j]->LSH_RangeSearch(center,(*clusters)[i].Cluster_Get_CenterCuve(),&(Closest_Neighbors[i]),&(Dist[i]),distance);
			}
			else{
				LSH[j]->LSH_RangeSearch(center,(*clusters)[i].Cluster_Get_CenterCuve(),&(Closest_Neighbors[i]),distance);
			}
		}
	}
	//Assignment the LSH range neighbors
	double objective_value = 0;
	double r = 0.005;
	int loop = 0;
	int max_assign = 0;
	do{
		r *=2;
		flag = 0;	
		for(int i=0;i<num_clusters;i++){
			if(loop == 0){
				sort( Closest_Neighbors[i].begin(), Closest_Neighbors[i].end() );
				Closest_Neighbors[i].erase( unique( Closest_Neighbors[i].begin(), 
					Closest_Neighbors[i].end() ), Closest_Neighbors[i].end() );
			}
			int center = (*clusters)[i].Cluster_Get_Center();
			int counter = Closest_Neighbors[i].size();
			int j = 0;
			int k = counter -1;
			while(j < counter){
				int neigh = Closest_Neighbors[i][k];
				if(object_info[neigh]->Get_LSH_Center() == center){
					double dist = object_info[neigh]->Get_LSH_dist();
					if((r > dist) && ((object_info[neigh]->Get_Flag() % 2) == 1)){
						object_info[neigh]->increment();
						flag =1;
						objective_value += dist;
						(*clusters)[i].Cluster_Insert(neigh,dist);
						Closest_Neighbors[i].erase( Closest_Neighbors[i].begin() + k);
						max_assign++;

					}
				}
				else{
					Closest_Neighbors[i].erase( Closest_Neighbors[i].begin() + k);
				}
				k--;
				j++;
			}
		}
		loop++;
	}while(flag && r<10);


	//Assignment all the curves which have not already assigned
	for(int j=0;j<n;j++){
		double dist = -1;
		int min;
		for(int i=0;i<num_clusters;i++){
			double temp_dist;
			int center = (*clusters)[i].Cluster_Get_Center();
			if(object_info[j]->Get_Flag() < t+2){
				if(center < 0){
					temp_dist = (*distance)((*clusters)[i].Cluster_Get_CenterCuve(),object_info[j]->Get_Curve());
				}
				else if(center > j){
					if(Distance_Table[center][j] == -1){
						Distance_Table[center][j] = (*distance)((*clusters)[i].Cluster_Get_CenterCuve(),object_info[j]->Get_Curve());
					}
					temp_dist = Distance_Table[center][j];
				}
				else if(center < j){
					if(Distance_Table[j][center] == -1){
						Distance_Table[j][center] = (*distance)((*clusters)[i].Cluster_Get_CenterCuve(),object_info[j]->Get_Curve());
					}
					temp_dist = Distance_Table[j][center];
				}
			}
			else{
				continue;
			}
			if(dist == -1){
				min = i;
				dist = temp_dist;
			}
			else if(temp_dist < dist){
				min = i;
				dist = temp_dist;
			}
		}
		if(dist != -1){
			max_assign++;
			(*clusters)[min].Cluster_Insert(j,dist);
			objective_value += dist;
			if(object_info[j]->Get_Flag() % 2 == 0){
				object_info[j]->increment();
			}
			object_info[j]->increment();
		}
	}
	delete[] Closest_Neighbors;
	if((*clusters)[0].Cluster_Get_Center() < 0){
		delete[] Dist;
	}
	return objective_value;
}*/