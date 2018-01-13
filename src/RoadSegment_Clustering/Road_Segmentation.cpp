#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../include/RoadSegment_Clustering/Road_Segmentation_Functions.h"


using namespace std;

float max_r = 0.05;
float length = 0.005;
int dim = 2;

void Usage(char * exec){
	fprintf(stderr,"Usage: %s -i <input_file> -o <output_file> -r <radius> -l <length> -d <dimension>\n",exec);
}


int read_args(int argc,char **argv,char **input_file,char ** output_file){
	if(argc < 5){
		Usage(argv[0]);
		exit(1);
	}
	int i = 1;
	while(i<argc){
		if(!strcmp(argv[i],"-i")){
			*input_file = (char *)malloc(sizeof(char)*(strlen(argv[i+1])+1));
			strcpy(*input_file,argv[i+1]);
		}
		else if(!strcmp(argv[i],"-o")){
			*output_file = (char *)malloc(sizeof(char)*(strlen(argv[i+1])+1));
			strcpy(*output_file,argv[i+1]);
		}
		else if(!strcmp(argv[i],"-r")){
			max_r = atof(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-l")){
			length = atof(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-d")){
			dim = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-h")){
			Usage(argv[0]);
			exit(0);
		}
		i+=2;
	}
	return 0;
}

int main(int argc,char **argv){
	char * input_file = NULL;
	char * output_file = NULL;
	if(read_args(argc,argv,&input_file,&output_file)){
		Usage(argv[0]);
		exit(1);
	}
	cout << "Input_file: " << input_file << endl;
	cout << "Output_file: " << output_file << endl;
	cout << "Dimension: " << dim << endl;
	cout << "Radius for curvature segment: " << max_r << endl;
	cout << "Length for segment: " << length << endl;
	read_ways(input_file,output_file,dim,max_r,length);
	return 0;
}