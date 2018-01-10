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
	vector<int> *curvature_length_split;
public:
	Way(char *,std::vector<Point *> *,std::vector<int> *,std::vector<int> *);
	~Way();
	void Insert_junction(int);
	void Insert_curvature(int);
	int print(int,ofstream &);
};


#endif // _WAYS_H_