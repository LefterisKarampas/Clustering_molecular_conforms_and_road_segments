#ifndef _SILHOUETTE_H_
#define _SILHOUETTE_H_

#include "Types.h"
#include <iostream>
#include <vector>
int Silhouette(Clusters,double (*distance)(int,int),std::vector<int> *,double *);

#endif