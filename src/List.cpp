#include <iostream>
#include <vector>
#include "../include/List.h"
#include "../include/Object_Info.h"

using namespace std;
extern Object_Info** object_info;
extern double ** Distance_Table;


bool Compare_points(Point *p1,Point *p2){
	if(p1->size() != p2->size()){
		return false;
	}
	else{
		for(unsigned int i=0;i<p1->size();i++){
			if((*p1)[i] != (*p2)[i]){
				return false;
			}
		}
		return true;
	}
}

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

Point * List::List_Insert(int index,Point *point,int pos){		//Insert a new node to list
	Node *k;
	if(pos >= 0){
		unsigned int i;
		Node *temp = this->head;
		while(temp != NULL){
			Point * t = temp->Get_Point();
			if((t->size() < 0) || (t->size() > 2)){
				cerr << "Failed" << endl;
				temp = temp->GetNext();
				continue;
			}
			for(i=0;i<point->size();i++){
				if((*t)[i] != (*point)[i]){
					break;
				}
			}
			if(i == point->size()){
				temp->Update_Belongs(index,pos);
				return t;
			}
			temp = temp->GetNext();
		}
	}
	k = new Node(index,point,pos);		//Create a node
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




/*void List::List_Search(int center,Object & object,Point * point,std::vector<int> *Closest_Neighbors,
	std::vector<double> *Dist,long double (*distance)( Object&, Object &)){
	Node * temp;							
	if(this->head != NULL){					//If list is empty,return NULL
		temp = this->head;
	}
	else{
		return;
	}
	while(temp){							//For each list's node
		if(!Compare_points(point,temp->Get_Point())){
			temp = temp->GetNext();
			continue;
		}
		Closest_Neighbors->push_back(temp->GetIndex());
		double dist = (*distance)(object,temp->GetValue());
		object_info[temp->GetIndex()]->LSH_Increment(center,dist);
		Dist->push_back(dist);
		temp = temp->GetNext();
	}
}

void List::List_Search(int center,Object & object,Point * point,std::vector<int> *Closest_Neighbors,
	long double (*distance)( Object&, Object &)){
	Node * temp;							
	if(this->head != NULL){					//If list is empty,return NULL
		temp = this->head;
	}
	else{
		return;
	}
	while(temp){							//For each list's node
		if(!Compare_points(point,temp->Get_Point())){
			temp = temp->GetNext();
			continue;
		}
		int neigh = temp->GetIndex();
		int index_b;
		int index_l;
		if(neigh > center){
			index_b = neigh;
			index_l = center;
		}
		else{
			index_b = center;
			index_l = neigh;
		}
		double dist;
		if(Distance_Table[index_b][index_l] == -1){
			dist = (*distance)(object,temp->GetValue());
			Distance_Table[index_b][index_l] = dist;
		}
		else{
			dist = Distance_Table[index_b][index_l];
		}
		Closest_Neighbors->push_back(neigh);
		object_info[neigh]->LSH_Increment(center,dist);
		temp = temp->GetNext();
	}
}
*/


/*						//Search for nearest Curves
List * List::List_Search(Curve * v,bool *flag){
	Node * temp;							
	if(this->head != NULL){					//If list is empty,return NULL
		temp = this->head;
	}
	else{
		return NULL;
	}
	List * result;
	result = new List();
	while(temp){							//For each list's node
		Curve *x = temp->GetValue();
		if(x->Compare_point(v)){		//Check if the objects have the same grid_objects
			if(*flag == false){
				*flag = true;				//If find at least one we will return only
			}								//objects with the same grid_object
			result->List_Insert(x);			
		}
		temp = temp->GetNext();
	}
	if(*flag == false){						//If we don't find any same grid_object 
		result->head = this->head;			//Return all the list
	}
	return result;							//Return the list
}*/


/*

void List::Clear_up(){				
	if(this->head != NULL){				
		Node *temp;	
		temp = this->head;
		while(temp){
			temp->Clear_up();
			temp = temp->GetNext();
		}
	}
}*/




void List::Set_NULL(){
	this->head = NULL;
}



/*					//Remove the list with the result but only the node
void List::Search_Clear(){		//not the info 
	Node *temp;
	Node *k;
	temp = this->head;
	this->head = NULL;
	while(temp){
		temp->Search_Clear();
		k = temp->GetNext();
		temp->Set_next(NULL);
		delete temp;
		temp = k;
	}
	this->head = NULL;
}*/



/*				//Find the LSH nearest neighbor
Curve * List::find_min(Curve object,long double *min_distance,
	long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &)){
	Node *temp;
	temp = this->head;
	Curve *x;
	x = temp->GetValue();
	(*min_distance) = (*distance)(x->GetCurve(),object.GetCurve());		//Compute the distance for first object
	long double temp_min;
	Curve *temp_x;
	temp = temp->GetNext();
	while(temp){														//For each object in list
		temp_x = temp->GetValue();
		temp_min = (*distance)(temp_x->GetCurve(),object.GetCurve());	//compute the distance 
		if(*min_distance > temp_min){									//compare the distances
			x = temp_x;
			*min_distance = temp_min;
		}
		temp = temp->GetNext(); 
	}	
	return x;															//Return the nearest neighbors
}



				//Find the LSH nearest neighbor and the r_near neighbors
Curve * List::find_min(Curve object,long double *min_distance,
	long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &),double R,std::vector<char *> *r_near){
	Node *temp;
	temp = this->head;
	Curve *x;
	x = temp->GetValue();
	(*min_distance) = (*distance)(x->GetCurve(),object.GetCurve());		//Compute the distance for first object
	if(*min_distance < R){
		r_near->push_back(x->GetId());
	} 
	long double temp_min;
	Curve *temp_x;
	temp = temp->GetNext();
	while(temp != NULL){												//For each object in list
		temp_x = temp->GetValue();
		temp_min = (*distance)(temp_x->GetCurve(),object.GetCurve());	//compute the distance 
		if(temp_min < R){												//Check if object is r_near neighbor
			r_near->push_back(temp_x->GetId());
		} 
		if(*min_distance > temp_min){									//compare the distances
			x = temp_x;
			*min_distance = temp_min;
		}
		temp = temp->GetNext(); 
	}
	return x;
}




				//Find true nearest,LSH nearest neighbor and r_near neighbors
Curve * List::find_nearest_min(Curve *object,Curve *neigh,long double *neigh_dist,bool *cond,double R,std::vector<char *> *r_near,Curve *nearest_neigh,long double *nearest_dist,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&)){
	Node *temp;
	temp = this->head;
	bool condition = false;
	while(temp){
		Curve *x;
		x = temp->GetValue();
		long double temp_dist;
		temp_dist = (*distance)(x->GetCurve(),object->GetCurve());
		if(x->Compare_point(object)){
			*cond = true;
			if(neigh == NULL){
				neigh = x;
				*neigh_dist = temp_dist;
			}
			else if(*neigh_dist > temp_dist){
				*neigh_dist = temp_dist;
				neigh = x;
			}
		}
		if(temp_dist < R){
			r_near->push_back(x->GetId());
		}
		if(!condition){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
			condition = true;
		}
		else if(*nearest_dist > temp_dist){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
		}
		temp = temp->GetNext();
	}
	return nearest_neigh;
}



					//Find true nearest
Curve * List::find_nearest(Curve *object,Curve *nearest_neigh,long double *nearest_dist,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&)){
	Node *temp;
	temp = this->head;
	Curve *x;
	while(temp){
		x = temp->GetValue();
		long double temp_dist;
		temp_dist = (*distance)(x->GetCurve(),object->GetCurve());
		if(nearest_neigh == NULL){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
		}
		else if(*nearest_dist > temp_dist){
			nearest_neigh = x;
			*nearest_dist = temp_dist;
		}
		temp = temp->GetNext();
	}
	return nearest_neigh;
}*/