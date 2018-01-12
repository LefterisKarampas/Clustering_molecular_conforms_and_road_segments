#ifndef _WAYS_H_
#define _WAYS_H_

#include "Types.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


class Way{
	char *id;
	vector<Point *> *points;
	vector<int> *junction_split;
public:
	Way(char *,std::vector<Point *> *,std::vector<int> *);
	~Way();
	void Insert_junction(int);
	int print(int,ofstream &,float,float);
};


#endif // _WAYS_H_