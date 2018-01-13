#include <iostream>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "../../include/Types.h"
#include "../../include/Clustering/Object_Info.h"
#include "../../include/Distance/Distance_Metric.h"
#include "../../include/Clustering/Initialization.h"
#include "../../include/Clustering/Assignment.h"
#include "../../include/Clustering/Update.h"
#include "../../include/Clustering/Cluster.h"
#include "../../include/Clustering/Silhouette.h"


using namespace std;

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


void print_clustering(Clusters clusters,ofstream &output,double silhouette,int k,int time_flag,float secs){
	output <<"k: " << k << endl;
	output << "s: "<< silhouette << endl;
	if(time_flag)
		output << "clustering_time: " << secs << endl;
	for(int i=0;i<clusters.size();i++){
		Neighbors neigh = clusters[i]->Cluster_Get_Neighbors();
		if(neigh.size() == 0){
			output << "Failed! " << clusters[i]->Cluster_Get_Center();
		}
		std::sort(neigh.begin(),neigh.end());
		for(unsigned int j=0;j<neigh.size();j++){
			output << neigh[j] << "\t";
		}
		output << endl;
	}
	output << "-----------------------" << endl;
	output << endl;
}


void Clustering(int k,int n,Object_Info **object_info,double (*distance)(int,int),ofstream &output,int silhouette_flag,int time_flag){
	int prev_k;
	int best_k;
	double best_value;
	double silhouette_value;

	std::vector<int> out;
	int sil_loop=0;
	std::vector<int> cluster_centers;
	struct timeval start, stop;
	float secs = 0;

	gettimeofday(&start, NULL);
	K_Means_Plusplus(&cluster_centers, k, n, distance);
	//Random_Initialization(&cluster_centers,k,n);
	do{
		Clusters clusters;
		int rem=0;
		for(unsigned int p=0;p<out.size();p++){
			cluster_centers.erase(cluster_centers.begin()+(out[p]-rem));
			rem++;
		}
		for(int i=0;i<k;i++){
			int new_center = cluster_centers[i];
			Cluster *temp = new Cluster(new_center);
			clusters.push_back(temp);
		}
		
		double objective_value,prev;
		int loop = 0;
		
		objective_value = Lloyd_Assignment(&clusters,n,distance);
		do{
		
			PAM_Improved(&clusters,distance);
			prev = objective_value;
			loop++;
			objective_value = Lloyd_Assignment(&clusters,n,distance);
	
		}while(loop < 20 && (abs(prev - objective_value) > 0.005));

		prev_k = k;
		out.clear();
		
		k = Silhouette(clusters,distance,&out,&silhouette_value);
		
		for(int i=0;i<n;i++){
			object_info[i]->Clear_Flag();
		}
		if(sil_loop == 0){
			best_value = silhouette_value;
			best_k  = prev_k;
 		}
 		else if(best_value < silhouette_value){
 			best_value = silhouette_value;
 			best_k = prev_k;
 		}
 		sil_loop++;
 		gettimeofday(&stop, NULL);
		secs = timedifference_msec(start,stop);
		print_clustering(clusters,output,silhouette_value,prev_k,time_flag,secs);

		for(int i=0;i<prev_k;i++){
			delete clusters[i];
		}
		if(prev_k == k){
			k--;
		}
		if(prev_k > 200){
			prev_k = k/3;
		}
		gettimeofday(&start, NULL);
	}while(k>1 && silhouette_flag);
	if(silhouette_flag)
		cout << "Best silhouette_value " << best_value << " with " << best_k << " clusters" << endl;
}