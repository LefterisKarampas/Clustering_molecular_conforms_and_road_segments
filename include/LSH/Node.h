#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include "../Types.h"

using namespace std;


class Node{
private:
	int index;
	Point *point;
	Node * next;
public:
	Node(int,Point *);
	~Node();
	int Set_next(Node * next);
	Node * GetNext();
	int GetIndex();
	Point *Get_Point();
};


#endif