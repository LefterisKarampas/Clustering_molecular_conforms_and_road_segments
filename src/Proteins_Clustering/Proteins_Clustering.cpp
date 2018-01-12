#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../include/protein_main_functions.h"
#include "../../include/Types.h"
#include "../../include/Object_Info.h"
#include "../../include/Distance_Metric.h"
#include "../../include/Initialization.h"
#include "../../include/Assignment.h"
#include "../../include/Update.h"
#include "../../include/Cluster.h"
#include "../../include/Silhouette.h"
#include "../../include/Clustering.h"


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

	Clustering(k,n,object_info,distance,output,silhouette_flag);

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
	output.close();
	free(input_file);
	free(metric);

}