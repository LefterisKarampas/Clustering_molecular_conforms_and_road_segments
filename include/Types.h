#ifndef _TYPES_H_
#define _TYPES_H_

#include <iostream>
#include "Cluster.h"
#include <vector>

typedef std::vector<double> Point;
typedef std::vector< Point > Conform;
typedef Conform Object;
typedef std::vector<Cluster *> Clusters; 

#endif