#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../include/Road_Segmentation_Functions.h"

using namespace std;

int main(int argc,char **argv){
	char * input_file = NULL;
	char * output_file = NULL;
	int split_data = 0;
	if(read_args(argc,argv,&input_file,&output_file,&split_data)){
		Usage(argv[0]);
		exit(1);
	}
	read_ways(input_file,output_file);
	return 0;
}