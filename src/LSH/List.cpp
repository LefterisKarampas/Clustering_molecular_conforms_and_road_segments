#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../../include/LSH/List.h"
#include "../../include/RoadSegment_Clustering/NodeWay.h"
#include "../../include/RoadSegment_Clustering/Ways.h"

using namespace std;



//---------------------------List Functions------------------------------

List::List(){							//Initialize the list
	this->head = NULL;
	this->count = 0;
}



List::~List(){						//Destroy the list
	if(this->head != NULL){
		delete this->head;					//Delete the head
	}
}

void List::Size(){
	cout << this-> count << endl;
}

Point * List::List_Insert(int index,Point *point,int pos,Way **ways){		//Insert a new node to list
	NodeWay *k;
	if(pos >= 0){
		unsigned int i;
		NodeWay *temp = (NodeWay *)this->head;
		while(temp != NULL){
			Point * t = temp->Get_Point();
			if((t->size() < 0) || (t->size() > 2)){
				cerr << "Failed" << endl;
				temp = (NodeWay *)temp->GetNext();
				continue;
			}
			for(i=0;i<point->size();i++){
				if((*t)[i] != (*point)[i]){
					break;
				}
			}
			if(i == point->size()){
				temp->Update_Belongs(index,pos,ways);
				return t;
			}
			temp = (NodeWay *)temp->GetNext();
		}
	}
	k = new NodeWay(index,point,pos);		//Create a node
	if(this->head != NULL){					//If list is not empty,set head as next to the new node
		k->Set_next(this->head);
	}
	this->head = k;							//Set the new node as head
	this->count++;
	return NULL;
}

					
int List::List_Insert(int index,Point *point){		//Insert a new node to list	
	Node *k;
	k = new Node(index,point);				//Create a node
	if(this->head != NULL){					//If list is not empty,set head as next to the new node
		k->Set_next(this->head);
	}
	this->head = k;							//Set the new node as head
	this->count++;
	return 0;
}


void List::Set_NULL(){
	this->head = NULL;
}


void List::Print(std::ofstream &output){
	Node *temp = this->head;
	vector<int> index;
	while(temp != NULL){
		index.push_back(temp->GetIndex());
		temp = temp->GetNext();
	}
	std::sort(index.begin(),index.end());
	for(unsigned int i =0;i<index.size();i++){
		output << index[i] << "\t";
	}
	output << endl;
}

vector<int> * List::Get_Values(){
	Node *temp = this->head;
	vector<int> * index = new vector<int>;
	while(temp != NULL){
		index->push_back(temp->GetIndex());
		temp = temp->GetNext();
	}
	return index;
}