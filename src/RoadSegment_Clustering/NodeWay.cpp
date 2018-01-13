#include <iostream>
#include <vector>
#include "../../include/RoadSegment_Clustering/Ways.h"
#include "../../include/LSH/Node.h"
#include "../../include/RoadSegment_Clustering/NodeWay.h"

#include <cstdlib>
using namespace std;


NodeWay::NodeWay(int index,Point *point,int pos):Node(index,point){
	this->map = (Mapping *)malloc(sizeof(Mapping));
	this->map->index = new vector<int>();
	this->map->index->push_back(index);
	this->map->pos = new vector<int>();
	this->map->pos->push_back(pos);
}


NodeWay::~NodeWay(){
	delete this->map->index;
	delete this->map->pos;
	free(map);
}

void NodeWay::Update_Belongs(int index,int pos,Way **ways){
	unsigned int size = this->map->index->size();
	for(unsigned int i=0;i<size;i++){
		int k = this->map->index->at(i);
		int p = this->map->pos->at(i);
		if(k >= index){
			continue;
		}
		ways[k]->Insert_junction(p);
	}
	this->map->index->push_back(index);
	this->map->pos->push_back(pos);
}