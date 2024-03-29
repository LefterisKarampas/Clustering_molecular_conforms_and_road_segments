#include "../../include/Clustering/Object_Info.h"
#include "../../include/Clustering/Cluster.h"
#include "../../include/Types.h"
#include "../../include/Distance/Distance_Metric.h"
#include <vector>
#include <iostream>

#define THRESHOLD -0.5
extern Object_Info ** object_info;
extern double ** Distance_Table;

using namespace std;

static double best = 0;


int Silhouette(Clusters clusters,double (*distance)(int,int),vector<int> *out,double *silhouette_value){
	std::vector<double> silhouette;
	int num_clusters = clusters.size();
	double all = 0;
	int count = 0;
	double overall = 0;
	//For each cluster
	for(unsigned int i=0;i<clusters.size();i++){
		Neighbors neigh = clusters[i]->Cluster_Get_Neighbors();
		silhouette.push_back(0);
		//For each object in cluster
		for(unsigned int j=0;j<neigh.size();j++){
			double sum = 0;
			//Find sum distance from this object to all other objects in cluster
			for(unsigned int k=0;k<neigh.size();k++){
				sum += Find_Distance(neigh[j],neigh[k],distance,Distance_Table);
			}
			//Find sum distance from this object to all other objects in the second best_cluster
			Neighbors second_neigh  = clusters[object_info[neigh[j]]->Get_second_neigh()]->Cluster_Get_Neighbors();
			double second_sum = 0;
			for(unsigned int k=0;k<second_neigh.size();k++){
				second_sum += Find_Distance(neigh[j],second_neigh[k],distance,Distance_Table);
			}
			if(second_sum == 0 && sum == 0){
				continue;
			}
			//Silhouette value
			if(second_sum > sum){
				all += (second_sum-sum)/(double)second_sum;
				silhouette[i] += (second_sum-sum)/(double)second_sum;
			}
			else{
				all += (second_sum-sum)/(double)sum;
				silhouette[i] += (second_sum-sum)/(double)sum;
			}
		}
		if(neigh.size() > 0){
			count += neigh.size();
			//Silhouette value for each cluster
			silhouette[i] = silhouette[i]/(double) neigh.size();
			//Silhouete value for all objects
			overall += silhouette[i];
			if(silhouette[i] <= THRESHOLD){
				num_clusters--;
				out->push_back(i);
			}
		}
	}
	*silhouette_value = overall/(double)clusters.size();
	//cout << "Overall: " << *silhouette_value << endl;
	/*if(best < all/(double)count){
		best = all/(double)count;
		cout << "For k: " << clusters.size() << " All: " << all/(double)count << endl;
	}*/
	return num_clusters;
}