#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <vector>
#include "Node.h"
#include "Types.h"
#include "Ways.h"

using namespace std;


class List{
private:
	Node * head;
	int count;
public:
	List();
	~List();
	Point * List_Insert(int,Point *,int,Way **);
	int List_Insert(int,Point *);
	void Set_NULL();
	void Size();

};



#endif