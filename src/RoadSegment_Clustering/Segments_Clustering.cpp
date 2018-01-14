#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../include/RoadSegment_Clustering/Segments_main_functions.h"
#include "../../include/Types.h"
#include "../../include/Clustering/Object_Info.h"
#include "../../include/Distance/Distance_Metric.h"
#include "../../include/RoadSegment_Clustering/Road_Segmentation_Functions.h"
#include "../../include/Clustering/Clustering.h"
#include "../../include/Clustering/LSH_Clustering.h"

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
	int lsh_flag = 0;
	int time_flag = 0;
	if(read_args(argc,argv,&input_file,&output_file,&segmentation_input,&k,&metric,&max_r,&length,&dim,&lsh_flag,&time_flag)){
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
	//Read dataset and store them in Object Info Table
	n = read_dataset(input_file,&object_info,dim);

	//Create the Distance Table Structure
	Distance_Table = (double **)malloc(sizeof(double *)*n);
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

	//Clustering
	if(!lsh_flag){
		int silhouette_flag = 0;
		if(k == -1){
			silhouette_flag = 1;
			k = n/2;
		}
		else if(k > n){
			k = n;
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
		Clustering(k,n,object_info,distance,output,silhouette_flag,time_flag);
	}
	//LSH Clustering
	else{
		if(k == -1){
			k = 2;
		}
		double (*distance)(int,int);
		distance = &Frechet;
		output.open("./results/lsh_ways_clustering.dat");
		LSH_Clustering(n,k,object_info,distance,dim,output);
	}


	for(int i=0;i<n;i++){
		if(object_info[i] != NULL){
			delete object_info[i];
		}
		free(Distance_Table[i]);
	}
	delete[] object_info;
	free(Distance_Table);

	if(segmentation_input){
		free(segmentation_input);
	}
	if(metric)
		free(metric);
	output.close();
	free(input_file);
	return 0;
}

