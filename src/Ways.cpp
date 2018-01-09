#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/Ways.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "../include/Distance.h"

using namespace std;

#define MAX_R 0.05
#define LENGHT 0.005


Way::Way(char *id,vector<Point *> *points,vector<int> *junction_split,vector<int> *curvature_length_split){
	this->id = id;
	this->points = points;
	this->junction_split = junction_split;
	this->curvature_length_split = curvature_length_split;
}


Way::~Way(){
	free(this->id);
	delete this->points;
	delete this->junction_split;
	delete this->curvature_length_split;
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


/*int Way::print(int count){
	std::vector<int> v(this->junction_split->size()+this->curvature_length_split->size());
	std::vector<int>::iterator it;
	sort(this->junction_split->begin(),this->junction_split->end());
	sort(this->curvature_length_split->begin(),this->curvature_length_split->end());
	it=std::set_union (this->junction_split->begin(),this->junction_split->end(),
		this->curvature_length_split->begin(), this->curvature_length_split->end(), v.begin());
	v.resize(it-v.begin());
	int j=0;
	for(unsigned int i=0;i<v.size();i++){
		if(i == 0 && v[i] == 0){
			continue;
		}
		if(j>v[i]){
			continue
		}
		cout << count++ << ", " << this->id << ", " << v[i]-j+1;
		for(;j<=v[i];j++){
			for(unsigned int k=0;k<points->at(j)->size();k++){
				cout << ", " << points->at(j)->at(k);
			}
		}
		cout << endl;
	}
	if(j == points->size()){
		return count;
	}
	cout << count++ << ", " << this->id << ", " << points->size()-j;
	for(;j<points->size();j++){
		for(unsigned int k=0;k<points->at(j)->size();k++){
			cout << ", " << points->at(j)->at(k);
		}
	}
	cout << endl;
	return count;
}*/

int Way::print(int count){
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
			if(r > MAX_R){
				prev = 0;
				vec.push_back(i-1);
				cout << "Curvature: " << current << endl;
				current = 0;
			}
			else if(prev > LENGHT){
				vec.push_back(i-1);
				cout << "Length " << current <<": " << prev << endl;
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
			if(prev > LENGHT){
				vec.push_back(i-1);
				cout << "Length " << current <<": " << prev << endl;
				prev = 0;
				current = 0;
			}
		}
		else{
			current++;
		}
	}
	int k = 0;
	for(unsigned int i=0;i<vec.size();i++){
		cout << count++ << ", " << this->id << ", " << vec[i]-k+1;
		for(;j<=vec[i];j++){
			for(unsigned int k=0;k<points->at(j)->size();k++){
				cout << ", " << points->at(j)->at(k);
			}
		}
		cout << endl;
		k = vec[i]+1;
	}
	if(k < points->size())
		cout << count++ << ", " << this->id << ", " << points->size()-k;
	for(;k<points->size();k++){
		for(unsigned int j=0;j<points->at(k)->size();j++){
			cout << ", " << points->at(k)->at(j);
		}
	}
	cout << endl;
	cout << "------------" <<endl;
	return count;
}