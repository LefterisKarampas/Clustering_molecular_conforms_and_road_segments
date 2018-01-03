#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "../include/Initialization.h"
#include "../include/Object_Info.h"
#include "../include/Distance.h"

using namespace std;

extern Object_Info** object_info;
extern double ** Distance_Table;

int Random_Initialization(std::vector<int> *Cluster_Centers,int num_clusters,int num_objects){
	std::vector<int> centers;
	int new_center;
	int flag = 0;
	for(int i=0;i<num_clusters;i++){
		flag = 0;
		while(!flag){
			new_center = rand() % num_objects;
			unsigned int j;
			for(j=0;j<centers.size();j++){
				if(centers[j] == new_center){
					break;
				}
			}
			if(j != centers.size()){
				continue;
			}
			Cluster_Centers->push_back(new_center);
			centers.push_back(new_center);
			flag = 1;
		}

	}
	return 0;
}

int K_Means_Plusplus(std::vector<int> *Cluster_Centers,int num_clusters,int num_objects,double (*distance)(int,int)){
	std::vector<int> centers;
	std::vector<double> min_distance;
	int new_center;
	new_center = rand() % num_objects;
	for(int i=0;i<num_clusters-1;i++){
		std::vector<double> P;
		(*Cluster_Centers).push_back(new_center);
		centers.push_back(new_center);
		double max_dist = -1; 
		for(int j=0;j<num_objects;j++){
			if(j == 0){
				P.push_back(0.0);
			}
			int flag = 0;
			for(unsigned int k=0;k<centers.size();k++){
				if(j == centers[k]){
					flag = 1;
					break;
				}
			}
			if(flag == 1){
				continue;
			}
			//Get the distance if exists in Distance Table or Calculate and store it
			double dist = Find_Distance(new_center,j,distance,Distance_Table);
			//Distance_Table
			if(max_dist < dist){
				max_dist = dist;
			}
			if(i == 0){
				min_distance.push_back(dist);
			}
			else if(min_distance[j] > dist){
				min_distance[j] = dist;
			}
		}
		for(int j=0;j<num_objects-centers.size();j++){
			P.push_back(P[j]+min_distance[j]/max_dist * min_distance[j]/max_dist);
		}
		int flag = 0;
		float x;
		int upper = P.size();
		int lower = 1;
		int mid = (upper+lower)/2;
		x = rand()/((float)RAND_MAX) * P[P.size()-1];
		while(1){
			if(P[mid] > x){
				if(P[mid] == x || (mid > 0 && P[mid-1] < x)){
					new_center = mid;
					break;
				}
				else{
					upper = mid;
					mid = (upper+lower)/2;
				}
			}
			else if(P[mid] == x){
				new_center = mid;
				break;
			}
			else{
				lower = mid;
				mid = (upper+lower)/2;
				if(mid == lower){
					mid++;
				}
			}
		}
		new_center-=2;
		if(new_center < 0){
			new_center = 0;
		}
		std::sort (centers.begin(),centers.end());
		for(unsigned int k=0;k<centers.size();k++){
			if(new_center >= centers[k]){
				new_center++;
			}
			else{
				break;
			}
		}
	}
	(*Cluster_Centers).push_back(new_center);
	return 0;
}