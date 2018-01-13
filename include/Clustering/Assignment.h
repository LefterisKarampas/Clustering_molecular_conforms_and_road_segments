#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include "Cluster.h"

double Lloyd_Assignment(Clusters *,int,double (*distance)(int,int));


//double LSH_RangeSearch_Assignment(Clusters *,int ,LSH_Curve ** ,int ,long double (*distance)(int,int));

#endif