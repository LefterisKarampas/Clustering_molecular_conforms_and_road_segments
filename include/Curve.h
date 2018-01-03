#ifndef CURVE_H_
#define CURVE_H_

#include <vector>
#include <iostream>

typedef std::vector<double> Point;
typedef std::vector< Point > Object;

class Curve{
	Object * curve;
	Point *grid_curve;
	char * id;
public:
	Curve(Object *, Point *,char *);
	~Curve();
	const Point & Get_GridCurve();
	bool Compare_GridCurve(Curve *);
	char * GetId();
	Object & GetCurve();
	void Clear_up();
};




#endif