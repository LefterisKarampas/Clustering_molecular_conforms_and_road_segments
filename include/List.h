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
	int List_Insert(int,Point *);
	//List * List_Search(Curve *,bool *);
	//void Clear_up();
	void Set_NULL();
	void List_Search(int ,Object & ,Point *,std::vector<int> *,
	std::vector<double> *,long double (*distance)( Object&, Object &));
	void List_Search(int ,Object & ,Point *,std::vector<int> *,
		long double (*distance)( Object&, Object &));
	//void Search_Clear();
	/*Curve * find_min(Curve ,long double *,
			long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &),double,std::vector<char *> *);
	Curve * find_min(Curve ,long double *,
			long double (*distance)(const  std::vector< std::vector<double> > &,const std::vector< std::vector<double> > &));
	Curve * find_nearest_min(Curve *,Curve *,long double *,bool *,double ,std::vector<char *> *,Curve *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
	Curve * find_nearest(Curve *,Curve *,long double *,long double(*distance)(const std::vector< std::vector<double> >&,const std::vector< std::vector<double> >&));
	*/
};



#endif