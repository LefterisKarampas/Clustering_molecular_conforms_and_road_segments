#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../include/Types.h"
#include "../../include/HashTable.h"
#include "../../include/Ways.h"

#define BUCKETS 200

using namespace std;



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



void read_ways(char *input_file,char *output_file,int dim,float max_r,float length){
	string line;
	ifstream myfile(input_file);
	if (!myfile.is_open())
	{
		cerr << "OPEN" << endl;
		exit(1);
	}
	int m = count_lines(input_file);

	Way ** ways = NULL;
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
		while(temp != NULL){
			if(k == 0){
				id = (char *)malloc(sizeof(char)*(strlen(temp)+1));
				strcpy(id,temp);
			}
			else if(k == 1){
				;
			}
			else{
				Point *p = new Point();
				for(int i=0;i<dim;i++){
					double x;
					sscanf(temp,"%lf",&x);
					p->push_back(x);
					if(i != (dim -1)){
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
					for(i=0;i<dim;i++){
						if(points->at(size-1)->at(i) != p->at(i)){
							break;
						}
					}
				}
				//Try insert it to Hash
				if(i != dim){
					Point *t = HT->Hash_Insert(loop,p,size,ways); //Insert Node to Hash and check if there is junction
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
					}
				}
				//Stay in the same Node
				else{
					delete p;
				}
			}
			k++;
			temp = strtok(NULL,", ");
		}
		ways[loop] = new Way(id,points,junction_split);
		loop++;
	}
	int count = 0;
	ofstream out(output_file);
	if(!out.is_open()){
		out.open("./roads/segments.csv");
	}
	for(int i=0;i<loop;i++){
		count = ways[i]->print(count,out,max_r,length);
	}
	out.close();
	int n;
	n = loop;
	myfile.close();
	delete HT;
	for(int i=0;i<n;i++){
		delete ways[i];
	}
	delete[] ways;
}