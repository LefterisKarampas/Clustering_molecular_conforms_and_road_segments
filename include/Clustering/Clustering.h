#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include "../Types.h"
#include "Cluster.h"
#include "Object_Info.h"

using namespace std;

float timedifference_msec(struct timeval , struct timeval );
void print_clustering(Clusters ,ofstream &,double ,int ,int ,float );
void Clustering(int,int,Object_Info **,double (*distance)(int,int),ofstream &,int,int);