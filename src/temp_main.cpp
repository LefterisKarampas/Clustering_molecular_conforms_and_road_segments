#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "../include/main_functions.h"
#include "../include/Types.h"
#include "../include/Object_Info.h"
#include "../include/Distance.h"
#include "../include/Initialization.h"
#include "../include/Assignment.h"
#include "../include/Update.h"
#include "../include/Cluster.h"
#include "../include/Silhouette.h"


Object_Info ** object_info;
double ** Distance_Table = NULL;

using namespace std;


int main(int argc,char **argv){
	int n;
	char * input_file = NULL;
	char * metric = NULL;
	int k = -1;
	int first_k;
	ofstream output;
	if(get_args(argc,argv,&input_file,&metric,&k)){
		return 1;
	}
	int silhouette_flag = 0;
	if(k == -1){
		silhouette_flag = 1;
	}
	read_dataset(input_file,&object_info,&n,&k);
	first_k = k;
	Distance_Table = (double **)malloc(sizeof(double *)*(n+k));
	for(int i=0;i<n;i++){
		Distance_Table[i] = (double *)malloc(sizeof(double)*(i+1));
		for(int j=0;j<i+1;j++){
			if(i == j){
				Distance_Table[i][j] = 0;
			}
			else{
				Distance_Table[i][j] = -1;
			}
		
		}
	}
	for(int i=n;i<n+k;i++){
		Distance_Table[i] = (double *)malloc(sizeof(double)*n);
		for(int j=0;j<n;j++){
			Distance_Table[i][j] = -1;
		}
	}
	srand(time(NULL));
	
	double (*distance)(int,int);
	if(!strcmp(metric,"DFT")){
		distance = &Frechet;
		output.open ("./results/frechet.dat");
	}
	else{
		distance = &c_RMSD;
		output.open("./results/crmsd.dat");
	}

	int prev_k;
	int best_k;
	double best_value;
	double silhouette_value;

	//Random_Initialization(&cluster_centers,k,n);
	std::vector<int> out;
	int sil_loop=0;
	std::vector<int> cluster_centers;
	if(!strcmp(metric,"DFT")){
		Random_Initialization(&cluster_centers,k,n);
	}
	else{
		K_Means_Plusplus(&cluster_centers, k, n, distance);
	}
	do{
		/*std::vector<int> cluster_centers;
		if(!strcmp(metric,"DFT")){
			Random_Initialization(&cluster_centers,k,n);
		}
		else{
			K_Means_Plusplus(&cluster_centers, k, n, distance);
		}*/
		Clusters clusters;
		int rem =0;
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

		print_clustering(clusters,output,silhouette_value,prev_k);
		for(int i=0;i<prev_k;i++){
			delete clusters[i];
		}
		if(prev_k == k){
			k--;
		}
		if(prev_k > 200){
			prev_k = k/3;
		}
	}while(k>1 && silhouette_flag);
	if(silhouette_flag)
		cout << "Best silhouette_value " << best_value << " with " << best_k << " clusters" << endl;

	//Free allocated space
	for(int i=0;i<n+first_k;i++){
		if(object_info[i] != NULL){
			delete object_info[i];
		}
		free(Distance_Table[i]);
	}
	delete[] object_info;
	free(Distance_Table);
	//delete clusters;
	free(input_file);
	free(metric);

}