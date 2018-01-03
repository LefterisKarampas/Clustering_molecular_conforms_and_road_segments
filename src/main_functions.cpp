#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include "../include/Object_Info.h"
#include "../include/Types.h"

using namespace std;

extern Object_Info** object_info;								//Structure for storing the curve's info
extern double ** Distance_Table; 


//Get the command line args
int get_args(int argc,char ** argv, char ** input_file,char **metric,int *k){
	int i;
	int count = 0;
	for(i=1;i<argc;i+=2){
		//Help manual
		if(!strcmp(argv[i],"-h")){
			fprintf(stderr,"Usage: %s -i <input_file> -d <metric> -k <num_clusters>\n",argv[0]);
			exit(1);
		}
		//Input file argument
		else if(!strcmp(argv[i],"-i")){
			if((i+1) < argc && (*input_file) == NULL){
				(*input_file) = (char *)malloc(sizeof(char) * (strlen(argv[i+1])+1));
				strcpy(*input_file,argv[i+1]);
				count++;
			}
			else{
				return 1;
			}
		}
		//Metric argument
		else if(!strcmp(argv[i],"-d")){
			if((!strcmp(argv[i+1],"DFT")) || (!strcmp(argv[i+1],"CRMSD"))){
				(*metric) = (char *)malloc(sizeof(char) * (strlen(argv[i+1])+1));
				strcpy(*metric,argv[i+1]);
				count++;
			}
			else{
				cerr << "Metric parameter in {DFT,CRMSD}" << endl;
				return 1;
			}
		}
		else if(!strcmp(argv[i],"-k")){
			*k = atoi(argv[i+1]);
			count++;
		}
		else{
			return 1;
		}
	}
	//If all arguments were not given, print error message and exit
	if(count < 2){
		fprintf(stderr,"Usage: %s -i <input_file> -d <metric> -k <num_clusters>\n",argv[0]);
		return 1;
	}
	return 0;
}

void read_dataset(char *input_file,Object_Info *** object_info,int *n,int *m){
	string line;
	ifstream myfile(input_file);
	if (!myfile.is_open())
	{
		cerr << "OPEN" << endl;
		return;
	}
	int numConform;
	int N = 0;
	int loop = 0;
	int k = 0;
	int num = 0;
	Object * object;
	while ( getline (myfile,line) ){
		if(loop == 0){
			numConform = atoi(line.c_str());
			if(*m == -1){
				*m = numConform/2;
			}
			*object_info = new Object_Info*[numConform+*m];
		}
		else if(loop == 1){
			N = atoi(line.c_str());
		}
		else{
			double x[3];
			if(k >= N){
				(*object_info)[num] = new Object_Info(object);
				k = 0;
				num++;
			}
			if(k == 0){
				object = new Object;
			}
			sscanf(line.c_str(),"%lf\t%lf\t%lf",x,x+1,x+2);
			Point p;
			p.push_back(x[0]);
			p.push_back(x[1]);
			p.push_back(x[2]);
			object->push_back(p);
			k++;
		}
		loop++;
	}
	(*object_info)[num] = new Object_Info(object);
	num++;
	if(num != numConform){
		cerr << num << " " << numConform<< " FAILED!" << endl;
		exit(1);
	}
	*n = numConform;
	for(int i=numConform;i<numConform+*m;i++){
		(*object_info)[i] = NULL;
	}
	myfile.close();
}

void print_clustering(Clusters clusters,ofstream &output,double silhouette,int k){
	output << k << endl;
	output << silhouette << endl;
	for(int i=0;i<k;i++){
		Neighbors neigh = clusters[i]->Cluster_Get_Neighbors();
		if(neigh.size() == 0){
			output << "Failed! " << clusters[i]->Cluster_Get_Center();
		}
		//output << "Cluster with center: "<< clusters[i]->Cluster_Get_Center() << endl;
		std::sort(neigh.begin(),neigh.end());
		for(unsigned int j=0;j<neigh.size();j++){
			output << neigh[j] << "\t";
		}
		output << endl;
	}
	output << endl;
}