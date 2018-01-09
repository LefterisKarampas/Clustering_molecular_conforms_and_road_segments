#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "../include/Types.h"
#include "../include/HashTable.h"
#include "../include/Ways.h"
#include "../include/Object_Info.h"
#include "../include/Distance.h"


using namespace std;
#define DIM 2
#define BUCKETS 200
#define MAX_R 0.05
#define LENGHT 0.005

Way ** ways = NULL;
Object_Info ** object_info = NULL;

void Usage(char * exec){
	fprintf(stderr,"Usage: %s -i <input_file>\n",exec);
}

int count_lines(char *path){
	char * command;
	int n;
	char buff[32];
	command = (char *) malloc(strlen(path)+strlen("wc -l  | cut -d ' ' -f1")+2);
	sprintf(command, "wc -l %s | cut -d ' ' -f1",path);
	FILE *in;
	if(!(in = popen(command, "r"))){
		exit(1);
	}
	n = 100;
	while(fgets(buff, sizeof(buff), in)!=NULL){
		n = atoi(buff);
	}
	pclose(in);
	free(command);
	return n;
}

int read_args(int argc,char **argv,char **input_file,int *split_data){
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
		if(!strcmp(argv[i],"-s")){
			*split_data = 1;
			i--;
		}
		else if(!strcmp(argv[i],"-h")){
			Usage(argv[0]);
			exit(0);
		}
		i+=2;
	}
	return 0;
}

void read_ways(char *input_file,int *n){
	string line;
	ifstream myfile(input_file);
	if (!myfile.is_open())
	{
		cerr << "OPEN" << endl;
		exit(1);
	}
	int m = count_lines(input_file);
	ways = new Way*[m];
	cerr << "We have " << m << " ways" << endl;
	int loop = 0;
	HashTable *HT;
	if(m > 10000){
		HT = new HashTable(BUCKETS);
	}
	else{
		HT = new HashTable(m/40);
	}

	while ( getline (myfile,line) ){
		char *dup = strdup(line.c_str());
		char *temp;
		int k=0;
		temp = strtok(dup,", ");
		char *id;
		std::vector<Point *> * points = new std::vector<Point *>();
		std::vector<int> * junction_split = new vector<int>();
		std::vector<int> * curvature_length_split = new vector<int>();
		double a;
		double b;
		double c;
		double prev = 0;
		int flag;
		while(temp != NULL){
			if(k == 0){
				id = (char *)malloc(sizeof(char)*(strlen(temp)+1));
				strcpy(id,temp);
			}
			else if(k == 1){
				flag = 2;
			}
			else{
				Point *p = new Point();
				for(int i=0;i<DIM;i++){
					double x;
					sscanf(temp,"%lf",&x);
					p->push_back(x);
					if(i != (DIM -1)){
						temp = strtok(NULL,", ");
						if(temp == NULL){
							cerr << "Failed data" << endl;
							exit(1);
						}
					}
				}
				int i = 0;
				int size = points->size();
				if(size > 0){
					for(i=0;i<DIM;i++){
						if(points->at(size-1)->at(i) != p->at(i)){
							break;
						}
					}
				}
				//Try insert it to Hash
				if(i != DIM){
					flag--;
					Point *t = HT->Hash_Insert(loop,p,size); //Insert Node to Hash and check if there is junction
					if(t == NULL){
						points->push_back(p);
					}
					else{
						//Junction find, update split pos
						int s = junction_split->size();
						if(s > 0){
							if(junction_split->at(s-1) == (size-1)){
								junction_split->at(s-1) = size;
							}
							else{
								junction_split->push_back(size);
							}
						}
						else{
							junction_split->push_back(size);
						}
						points->push_back(t);
						delete p;
						prev = 0;
						flag = 3;
					}
					//Curvature and Length
					/*double r;
					int index = points->size();
					if(index>=3 && flag <= 0){
						double sum = 0;
						int i = index -1;
						if(index == 3){
							a = Euclidean_Distance(*(points->at(i-2)),*(points->at(i-1)));
						}
						else{
							a = b;
						}
						b = Euclidean_Distance(*(points->at(i-1)),*(points->at(i)));
						c = Euclidean_Distance(*(points->at(i-2)),*(points->at(i)));
						sum = (a+b+c)*(b+c-a)*(c+a-b)*(a+b-c);
						if(sum <= 0){
							r = 0;
						}
						else{
							r = (a*b*c) / (double)sqrt(sum);
						}
						prev += b;
						if(r > MAX_R){
							curvature_length_split->push_back(i-1);
							flag = 2;
							prev = 0;
						}
						else if(prev > LENGHT){
							curvature_length_split->push_back(index-1);
							prev = 0;
							flag = 2;
						}
					}
					else if(index == 1){
						prev = 0;
					}
					else{
						a = Euclidean_Distance(*(points->at(index-1)),*(points->at(index-2)));
						prev += a;
						if(prev > LENGHT){
							curvature_length_split->push_back(index-1);
							prev = 0;
						}
					}*/
				}
				//Stay in the same Node
				else{
					delete p;
				}
			}
			k++;
			temp = strtok(NULL,", ");
		}
		ways[loop] = new Way(id,points,junction_split,curvature_length_split);
		loop++;
	}
	int count = 0;
	for(int i=0;i<loop;i++){
		count = ways[i]->print(count);
	}
	*n = loop;
	myfile.close();
	delete HT;
}

int main(int argc,char **argv){
	char * input_file = NULL;
	int split_data = 0;
	if(read_args(argc,argv,&input_file,&split_data)){
		Usage(argv[0]);
		exit(1);
	}
	int n;
	read_ways(input_file,&n);
	for(int i=0;i<n;i++){
		delete ways[i];
	}
	delete[] ways;
}