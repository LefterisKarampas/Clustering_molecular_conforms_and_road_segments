#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "../../include/Clustering/Object_Info.h"
#include "../../include/RoadSegment_Clustering/Road_Segmentation_Functions.h"
#include <fstream>
#include <vector>

using namespace std;


void Usage(char * exec){
	fprintf(stderr,"Usage: %s -i <input_file> -o <output_file> -s <segmentation_file> -k <clusters> -r <radius> -l <length> -dim <dimension> -d <metric> -lsh -t\n",exec);
}

int read_args(int argc,char **argv,char **input_file,char ** output_file,char **segmentation,int *k,
	char **metric,float *max_r,float *length,int *dim,int *lsh,int *time_flag){
	if(argc < 3){
		Usage(argv[0]);
		exit(1);
	}
	int i = 1;
	while(i<argc){
		if(!strcmp(argv[i],"-i")){
			*input_file = (char *)malloc(sizeof(char)*(strlen(argv[i+1])+1));
			strcpy(*input_file,argv[i+1]);
		}
		else if(!strcmp(argv[i],"-s")){
			*segmentation = (char *)malloc(sizeof(char)*(strlen(argv[i+1])+1));
			strcpy(*segmentation,argv[i+1]);
		}
		else if(!strcmp(argv[i],"-o")){
			*output_file = (char *)malloc(sizeof(char)*(strlen(argv[i+1])+1));
			strcpy(*output_file,argv[i+1]);
		}
		else if(!strcmp(argv[i],"-r")){
			*max_r = atof(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-l")){
			*length = atof(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-dim")){
			*dim = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-k")){
			*k = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-d")){
			if((!strcmp(argv[i+1],"DFT")) || (!strcmp(argv[i+1],"DTW"))){
				(*metric) = (char *)malloc(sizeof(char) * (strlen(argv[i+1])+1));
				strcpy(*metric,argv[i+1]);
			}
			else{
				cerr << "Metric parameter in {DFT,DTW}" << endl;
				return 1;
			}
		}
		else if(!strcmp(argv[i],"-lsh")){
			*lsh = 1;
			i--;
		}
		else if(!strcmp(argv[i],"-t")){
			*time_flag = 1;
			i--;
		}
		else if(!strcmp(argv[i],"-h")){
			Usage(argv[0]);
			exit(0);
		}
		i+=2;
	}
	if(*input_file == NULL){
		if(((*segmentation) == NULL) || ((*output_file) == NULL)){
			cerr << "Give at least input_file or segmentation_file and output_file" << endl;
			Usage(argv[0]);
			exit(1);
		}
	}
	if((*metric) == NULL){
		cerr << "Give metric distance" << endl;
		Usage(argv[0]);
		exit(1);
	}
	return 0;
}


int read_dataset(char *input_file,Object_Info *** object_info,int dim){
	string line;
	ifstream myfile(input_file);
	if (!myfile.is_open())
	{
		cerr << "OPEN" << endl;
		exit(1);
	}
	int segments;
	segments = count_lines(input_file);
	segments--;
	int k = 0;
	int num = 0;
	Object * object;
	*object_info = new Object_Info*[segments];
	while (getline (myfile,line) && (num < segments)){
		k = 0;
		istringstream ss(line);
		string token;
		int m = 1;
		Point p;
		int nodes;
		while(getline(ss,token,',')){
			if(k == 0){
				object = new Object;
			}
			else if(k == 1){
				;
			}
			else if(k == 2){
				nodes = atoi(&(token.c_str()[1]));
			}
			else{
				double x;
				x = atof(&(token.c_str()[1]));
				p.push_back(x);
				if(m % dim == 0){
					object->push_back(p);
					m = 1;
					p.clear();
				}
				else{
					m++;
				}
			}
			k++;
		}
		(*object_info)[num] = new Object_Info(object);
		num++;
	}
	if(num != segments){
		cerr << num << " " << segments<< " FAILED!" << endl;
		exit(1);
	}
	myfile.close();
	return segments;
}

