#ifndef _MAIN_FUNC_H_
#define _MAIN_FUNC_H_

#include "Object_Info.h"
#include "Types.h"
#include <iostream>
#include <fstream>

int get_args(int,char ** , char **,char **,int *);

void read_dataset(char *,Object_Info *** ,int *,int *);

void print_clustering(Clusters,std::ofstream &,double,int);

#endif //_MAIN_FUNC_H_
