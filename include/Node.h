#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include "Types.h"

using namespace std;


typedef struct {
	std::vector<int> *index;
	std::vector<int> *pos;
}Mapping;

class Node{
private:
	int index;
	Mapping *map;
	Point *point;
	Node * next;
public:
	Node(int,Point *);
	Node(int,Point *,int);
	~Node();
	int Set_next(Node * next);
	Node * GetNext();
	int GetIndex();
	Object & GetValue();
	Point *Get_Point();
	void Update_Belongs(int,int);	
};


#endif