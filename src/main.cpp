#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "../include/main_functions.h"



using namespace std;

Curve_Info** curve_info = NULL;								//Structure for storing the curve's info
double ** Distance_Table = NULL;

int main(int argc,char **argv){
	char * input_file = NULL;
	char * config_file = NULL;
	char * output_file = NULL;
	char * metric = NULL;
	int num_clusters,num_grid,num_HT;
	//Read command line args
	if(get_args(argc,argv,&input_file,&config_file,&output_file,&metric)){
		cerr << "Fail read args" << endl;
		exit(1);
	}
	//Read configuration file
	if(read_config(config_file,&num_clusters,&num_grid,&num_HT)){
		cerr << "Fail read configuration file" << endl;
		exit(2);
	}
	//Read curves from file
	LSH_Curve ** LSH;											//LSH Structure
	int n;
	//Read curves and construct Curve's info structure and LSH
	if((LSH = read_curves(input_file,num_HT,num_grid,&curve_info,&n)) == NULL){
		cerr << "Failed read input file" << endl;
		exit(3);
	}
	Distance_Table = (double **)malloc(sizeof(double *)*n);

	long double (*distance)(Object &,Object &);
	distance = &(DFT);
	srand(time(NULL));
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
	Clustering(output_file,num_clusters,n,LSH,num_HT,distance,1);
	
	//Destroy the curves and LSH structure
	for(int i=0;i<n;i++){
		if(curve_info[i] != NULL){
			delete curve_info[i];
		}
	}
	delete[] curve_info;

	for(int i =0;i<num_HT;i++){
		delete LSH[i];
	}
	delete[] LSH;

	//Free the allocate space for arguments
	free(input_file);
	free(config_file);
	free(output_file);
	free(metric);
	return 0;
}