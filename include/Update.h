#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "Types.h"
#include "Cluster.h"

void PAM_Improved(Clusters *clusters,double (*distance)(int,int));

double Mean_Discrete_Frechet(Clusters *clusters,int,double (*distance)( int,int));


#endif