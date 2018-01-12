#include <iostream>
#include <vector>
#include "../../include/Object_Info.h"
#include "../../include/Node.h"
#include "../../include/Types.h"

using namespace std;

//---------------------------Node Functions------------------------------

Node::Node(int index,Point *point){
	this->index = index;
	this->point = point;
	this->next = NULL;
}


Node::~Node(){
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



Point * Node::Get_Point(){
	return this->point;
}
