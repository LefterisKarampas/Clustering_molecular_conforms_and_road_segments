#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../include/Segments_main_functions.h"
#include "../../include/Types.h"
#include "../../include/Object_Info.h"
#include "../../include/Distance_Metric.h"
#include "../../include/Initialization.h"
#include "../../include/Assignment.h"
#include "../../include/Update.h"
#include "../../include/Cluster.h"
#include "../../include/Silhouette.h"
#include "../../include/Road_Segmentation_Functions.h"


Object_Info ** object_info;
double ** Distance_Table = NULL;

float max_r = 0.05;
float length = 0.005;
int dim = 2;

using namespace std;

int main(int argc,char **argv){
	char * input_file = NULL;
	char * output_file = NULL;
	char *segmentation_input = NULL;
	char *metric = NULL;
	ofstream output;
	int split_data = 0;
	int k = -1;
	int n;
	int segmentation = 0;
	if(read_args(argc,argv,&input_file,&output_file,&segmentation_input,&k,&metric,&max_r,&length,&dim)){
		Usage(argv[0]);
		exit(1);
	}
	
	//Read the ways and split into segments
	if(input_file == NULL){
		if(segmentation_input != NULL){
			read_ways(segmentation_input,output_file,dim,max_r,length);
			input_file = output_file;
			output_file = NULL;
		}
		else{
			cerr << "We can have input_file as arg or segmentation_file with output_file!" << endl;
			exit(1);
		}
	}
	cout << "Input_File: " << input_file << endl;
	n = read_dataset(input_file,&object_info,dim);

	int silhouette_flag = 0;
	if(k == -1){
		silhouette_flag = 1;
		k = n/2;
	}
	
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
	for(int i=n;i<n;i++){
		Distance_Table[i] = (double *)malloc(sizeof(double)*n);
		for(int j=0;j<n;j++){
			Distance_Table[i][j] = -1;
		}
	}
	srand(time(NULL));
	
	double (*distance)(int,int);
	if(!strcmp(metric,"DFT")){
		distance = &Frechet;
		output.open ("./results/kmeans_ways_frechet.dat");
	}
	else{
		distance = &DTW;
		output.open("./results/kmeans_ways_dtw.dat");
	}

	int prev_k;
	int best_k;
	double best_value;
	double silhouette_value;

	std::vector<int> out;
	int sil_loop=0;
	std::vector<int> cluster_centers;
	
	K_Means_Plusplus(&cluster_centers, k, n, distance);
	do{
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

	for(int i=0;i<n;i++){
		if(object_info[i] != NULL){
			delete object_info[i];
		}
		free(Distance_Table[i]);
	}
	delete[] object_info;
	free(Distance_Table);
	//delete clusters;
	if(segmentation_input){
		free(segmentation_input);
	}
	free(input_file);
	free(metric);
	return 0;
}

