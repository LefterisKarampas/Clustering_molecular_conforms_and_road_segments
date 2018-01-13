#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include "../../include/Clustering/Object_Info.h"
#include "../../include/Types.h"

using namespace std;


//Get the command line args
int get_args(int argc,char ** argv, char ** input_file,char **metric,int *k,int *dim,int *time_flag){
	int i;
	int count = 0;
	for(i=1;i<argc;i+=2){
		//Help manual
		if(!strcmp(argv[i],"-h")){
			fprintf(stderr,"Usage: %s -i <input_file> -d <metric> -k <num_clusters> -dim <dimension> -t\n",argv[0]);
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
		else if(!strcmp(argv[i],"-dim")){
			*dim = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-t")){
			*time_flag = 1;
			i--;
		}
		else{
			return 1;
		}
	}
	//If all arguments were not given, print error message and exit
	if(count < 2){
		fprintf(stderr,"Usage: %s -i <input_file> -d <metric> -k <num_clusters> -dim <dimension> -t\n",argv[0]);
		return 1;
	}
	return 0;
}

void read_dataset(char *input_file,Object_Info *** object_info,int *n,int *m,int dim){
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
			*object_info = new Object_Info*[numConform];
		}
		else if(loop == 1){
			N = atoi(line.c_str());
		}
		else{
			double x[dim];
			if(k >= N){
				(*object_info)[num] = new Object_Info(object);
				k = 0;
				num++;
			}
			if(k == 0){
				object = new Object;
			}
			if(dim == 1){
				sscanf(line.c_str(),"%lf",x);
			}
			else if(dim == 2){
				sscanf(line.c_str(),"%lf\t%lf",x,x+1);
			}
			else if(dim == 3){
				sscanf(line.c_str(),"%lf\t%lf\t%lf",x,x+1,x+2);
			}
			Point p;
			for(int i=0;i<dim;i++){
				p.push_back(x[i]);
			}
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
	myfile.close();
}
