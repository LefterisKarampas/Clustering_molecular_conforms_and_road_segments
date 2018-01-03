#include <iostream>
#include <vector>
#include "../include/Curve_Info.h"
#include "../include/Node.h"

using namespace std;

//---------------------------Node Functions------------------------------

extern Curve_Info** curve_info;

Node::Node(int index,Point *GridCurve){
	this->index = index;
	/*cout << index << ". " << curve_info[index]->GetId() << endl;
	Object temp(curve_info[index]->Get_Curve());
	for(int i=0;i<temp.size();i++){
		cout << "\t (";
		int j;
		for(j=0;j<temp[i].size()-1;j++){
			cout << temp[i][j] <<",";
		}
		cout << temp[i][j] << ")" << endl;
	}*/
	this->GridCurve = GridCurve;
	this->next = NULL;
}




Node::~Node(){
	delete this->GridCurve;
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
	return curve_info[this->index]->Get_Curve();
}


Point * Node::Get_GridCurve(){
	return this->GridCurve;
}