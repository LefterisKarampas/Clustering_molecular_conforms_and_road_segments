#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <vector>
#include "Node.h"
#include "Types.h"

using namespace std;

bool Compare_GridCurves(Point *,Point *);

class List{
private:
	Node * head;
	int count;
public:
	List();
	~List();
	Point * List_Insert(int,Point *,int);
	int List_Insert(int,Point *);
	void Set_NULL();
	void Size();
	/*void List_Search(int ,Object & ,Point *,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void List_Search(int ,Object & ,Point *,std::vector<int> *,
		long double (*distance)( Object&, Object &));*/

};



#endif