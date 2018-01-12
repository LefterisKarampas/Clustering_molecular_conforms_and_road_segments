#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "../../include/Ways.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iomanip>

using namespace std;


template <typename T>
double Euclidean_Distance(T & v1,T &v2){
 	long double sum = 0;
 	for(unsigned int i = 0;i<v1.size();i++){
 		sum += pow((v1[i] - v2[i]),2);
 	}
 	return sqrt(sum);
 };




Way::Way(char *id,vector<Point *> *points,vector<int> *junction_split){
	this->id = id;
	this->points = points;
	this->junction_split = junction_split;
}


Way::~Way(){
	free(this->id);
	delete this->points;
	delete this->junction_split;
}

void Way::Insert_junction(int index){
	int size = this->junction_split->size();
	if(size > 0){
		if(this->junction_split->at(size -1) == (index-1)){
			this->junction_split->at(size-1) = index;
		}
		else if(this->junction_split->at(size -1) == index){
			;
		}
		else{
			this->junction_split->push_back(index);
		}
	}
	else{
		this->junction_split->push_back(index);
	}
}



int Way::print(int count,std::ofstream & out,float max_r,float length){
	sort(this->junction_split->begin(),this->junction_split->end());
	int current = 0;
	int flag = 0;
	vector<int> vec;
	int j = 0;
	double a;
	double b;
	double c;
	double r;
	double prev = 0;
	int last = -2;
	int size = this->junction_split->size();

	//Cut the roads
	for(unsigned int i=0;i<this->points->size();i++){
		if(size > j && this->junction_split->at(j) == i){
			if(this->junction_split->at(j) != 0){
				if(last == (i-1)){
					vec.at(vec.size()-1) = i;
				}
				else{
					vec.push_back(i);
				}
				last = i;
				current = 0;
				prev = 0;
				j++;
			}
			else{
				current++;
			}
		}
		else if(current >=2){
			double sum = 0;
			if(current == 2){
				a = Euclidean_Distance(*(this->points->at(i-2)),*(this->points->at(i-1)));
			}
			else{
				a = b;
			}
			b = Euclidean_Distance(*(this->points->at(i-1)),*(this->points->at(i)));
			c = Euclidean_Distance(*(this->points->at(i-2)),*(this->points->at(i)));
			sum = (a+b+c)*(b+c-a)*(c+a-b)*(a+b-c);
			if(sum <= 0){
				r = 0;
			}
			else{
				r = (a*b*c) / (double)sqrt(sum);
			}
			prev += b;
			if(r > max_r){
				prev = 0;
				vec.push_back(i-1);
				current = 0;
			}
			else if(prev > length){
				vec.push_back(i-1);
				prev = 0;
				current = 0;
			}
			else{
				current++;
			}
		}
		else if(current == 1){
			a = Euclidean_Distance(*(this->points->at(i-1)),*(this->points->at(i)));
			prev += a;
			current++;
			if(prev > length){
				vec.push_back(i-1);
				prev = 0;
				current = 0;
			}
		}
		else{
			current++;
		}
	}

	//Print into file
	int k = 0;
	j = 0;
	for(unsigned int i=0;i<vec.size();i++){
		out << count++ << ", " << this->id << ", " << vec[i]-j+1;
		for(;j<=vec[i];j++){
			for(unsigned int k=0;k<points->at(j)->size();k++){
				out << ", " << setprecision(10) << points->at(j)->at(k);
			}
		}
		out << endl;
		j = vec[i]+1;
	}
	flag = 0;
	if(j < points->size()){
		out << count++ << ", " << this->id << ", " << points->size()-j;
		flag = 1;
	}
	for(;j<points->size();j++){
		for(unsigned int k=0;k<points->at(j)->size();k++){
			out << ", " << setprecision(10) << points->at(j)->at(k);
		}
	}
	if(flag)
		out << endl;
	return count;
}