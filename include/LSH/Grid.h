#ifndef _GRID_H_
#define _GRID_H_

#include <vector>
#include "../Types.h"


using namespace std;


class Grid{
	std::vector<double> t;
	double delta;
	//double Generator(double,double);
public:
	Grid(int);
	~Grid();
	Point * Create_Point(const Object &);
};



#endif
