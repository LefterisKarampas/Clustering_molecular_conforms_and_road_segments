#include <iostream>
#include <vector>
#include "../include/Object_Info.h"
#include "../include/Node.h"
#include "../include/Types.h"
#include "../include/Ways.h"

using namespace std;

//---------------------------Node Functions------------------------------

extern Object_Info** object_info;
extern Way ** ways;

Node::Node(int index,Point *point){
	this->index = index;
	this->point = point;
	this->next = NULL;
	this->map = NULL;
}

Node::Node(int index,Point *point,int pos){
	this->map = (Mapping *)malloc(sizeof(Mapping));
	this->map->index = new vector<int>();
	this->map->index->push_back(index);
	this->map->pos = new vector<int>();
	this->map->pos->push_back(pos);
	this->point = point;
	this->next = NULL;
}


Node::~Node(){
	if(this->map){
		delete this->map->index;
		delete this->map->pos;
		free(map);
	}
	delete this->point;
	if(this->next != NULL){
		delete this->next;
	}
}


int Node::Set_next(Node * next){
	this->next = next;
	return 0;
}



Node * Node::GetNext(){
	return this->next;
}


int Node::GetIndex(){
	return this->index;
}



Object & Node::GetValue(){
	#ifdef _GLOBAL_STRUCTURES_
		return object_info[this->index]->Get_Object();
	#endif
}


Point * Node::Get_Point(){
	return this->point;
}



void Node::Update_Belongs(int index,int pos){
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
