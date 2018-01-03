#include "../include/Object_Info.h"
#include "../include/Cluster.h"
#include "../include/Types.h"
#include "../include/Distance.h"
#include <vector>
#include <iostream>

#define THRESHOLD -0.5
extern Object_Info ** object_info;
extern double ** Distance_Table;

using namespace std;


int Silhouette(Clusters clusters,double (*distance)(int,int),vector<int> *out,double *silhouette_value){
	std::vector<double> silhouette;
	int num_clusters = clusters.size();
	double all = 0;
	int count = 0;
	double overall = 0;
	for(unsigned int i=0;i<clusters.size();i++){
		Neighbors neigh = clusters[i]->Cluster_Get_Neighbors();
		silhouette.push_back(0);
		for(unsigned int j=0;j<neigh.size();j++){
			double sum = 0;
			for(unsigned int k=0;k<neigh.size();k++){
				sum += Find_Distance(neigh[j],neigh[k],distance,Distance_Table);
			}
			Neighbors second_neigh  = clusters[object_info[neigh[j]]->Get_second_neigh()]->Cluster_Get_Neighbors();
			double second_sum = 0;
			for(unsigned int k=0;k<second_neigh.size();k++){
				second_sum += Find_Distance(neigh[j],second_neigh[k],distance,Distance_Table);
			}
			if(second_sum == 0 && sum == 0){
				continue;
			}
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
			silhouette[i] = silhouette[i]/(double) neigh.size();
			overall += silhouette[i];
			if(silhouette[i] <= THRESHOLD){
				num_clusters--;
				out->push_back(i);
			}
		}
	}
	*silhouette_value = overall/(double)clusters.size();
	//cout << "Overall: " << *silhouette_value << endl;
	//cout << "For k: " << clusters.size() << " All: " << all/(double)count << endl;
	return num_clusters;
}