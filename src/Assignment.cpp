#include "../include/Assignment.h"
#include "../include/Object_Info.h"
#include "../include/Cluster.h"
#include "../include/LSH_Curve.h"
#include "../include/Distance.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>


using namespace std;

extern Object_Info ** object_info;
extern double ** Distance_Table;

double Lloyd_Assignment(Clusters *clusters,int n,double (*distance)(int,int)){
	if(n <= 0){
		cerr << "We don't have any curve\n" << endl;
	}
	double objective_value = 0;
	//First assignment
	if(object_info[0]->Get_Flag() == 0){
		for(int i=0;i<n;i++){
			int min;
			double min_dist;
			int second_best;
			double second_dist;
			for(unsigned int j=0;j<clusters->size();j++){
				int center = (*clusters)[j]->Cluster_Get_Center();
				double dist = Find_Distance(center,i,distance,Distance_Table);
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
			(*clusters)[min]->Cluster_Insert(i,min_dist);
			objective_value += min_dist;
			object_info[i]->Increase_Flag();
			object_info[i]->Set_second_neigh(second_best);
		}
	}
	else{
		int flag = object_info[0]->Get_Flag();
		for(unsigned int i=0;i<clusters->size();i++){
			int removed = 0;
			std::vector<int> neigh = (*clusters)[i]->Cluster_Get_Neighbors();
			for(unsigned int j=0;j<neigh.size();j++){
				if(object_info[neigh[j]]->Get_Flag() > flag){
					continue;
				}
				int min;
				double min_dist;
				int second_best;
				double second_dist;
				for(unsigned int k=0;k<clusters->size();k++){
					int center = (*clusters)[k]->Cluster_Get_Center();
					double dist;
					dist = Find_Distance(center,neigh[j],distance,Distance_Table);
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
				if(min != i){
					(*clusters)[min]->Cluster_Insert(neigh[j],min_dist);
					(*clusters)[i]->Cluster_Remove_Neigh(j-removed);
					removed++;
				}
				objective_value += min_dist;
				object_info[neigh[j]]->Set_second_neigh(second_best);
				object_info[neigh[j]]->Increase_Flag();
			}
		}
	}
	return objective_value;
}



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