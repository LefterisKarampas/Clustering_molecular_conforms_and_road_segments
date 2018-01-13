#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include "../../include/Clustering/Object_Info.h"
#include "../../include/Types.h"
#include "../../include/LSH/LSH.h"
#include "../../include/LSH/HashFunctions.h"
#include "../../include/Clustering/Cluster.h"
#include "../../include/Clustering/Clustering.h"
#include "../../include/Clustering/Silhouette.h"
#include "../../include/Distance/Distance_Metric.h"


using namespace std;

void Find_SecondBest(Clusters clusters,Object_Info **object_info){
	for(unsigned int i=0;i<clusters.size();i++){
		Neighbors neigh = clusters[i]->Cluster_Get_Neighbors();
		for(unsigned int j=0;j<neigh.size();j++){
			int second_best;
			double min_dist = -1;
			for(unsigned int k=0;k<clusters.size();k++){
				double dist;
				if(i == k){
					continue;
				}
				dist = Frechet(neigh[j],clusters[k]->Get_Centroid());
				if(min_dist == -1){
					min_dist = dist;
					second_best = k;
				}
				else if(min_dist > dist){
					min_dist = dist;
					second_best = k;
				}
			}
			object_info[neigh[j]]->Set_second_neigh(second_best);
		}
	}
}


void LSH_Clustering(int n,int k,Object_Info **object_info,double (*distance)(int,int),int dim,ofstream &output){
	LSH * HT;
	double silhouette;
	double best_silhouette;
	int best_k;
	int loop = 0;
	int k_vec = 10;
	struct timeval start, stop;
	float secs = 0;
	std::vector<int> out;
	do{
		gettimeofday(&start, NULL);
		HT = new LSH(k,dim,k_vec,n/4,&probabilistic);
		for(int i=0;i<n;i++){
			HT->LSH_Insert(i,object_info[i]->Get_Object());
		}

		vector<vector<int> *> neigh = HT->Get_Clusters();
		Clusters clusters;
		for(unsigned int i=0;i<neigh.size();i++){
			Cluster *temp = new Cluster(neigh[i]);
			clusters.push_back(temp);
		}
		Find_SecondBest(clusters,object_info);
		Silhouette(clusters,distance,&out,&silhouette);
		//Silhouette
		if(loop == 0){
			best_silhouette = silhouette;
			best_k = k;
		}
		else if(best_silhouette < silhouette){
			best_silhouette = silhouette;
			best_k = k;
		}
		out.clear();
		gettimeofday(&stop, NULL);
		secs = timedifference_msec(start,stop);
		print_clustering(clusters,output,silhouette,k,1,secs);
		//delete HT
		for(int i=0;i<clusters.size();i++){
			delete clusters[i];
		}
		k++;
		loop++;
	}while(k < 10);
	cout << "Best silhouette_value " << best_silhouette << " with " << best_k << " LSH Grids" << endl;
}