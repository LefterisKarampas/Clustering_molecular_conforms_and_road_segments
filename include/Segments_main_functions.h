#ifndef _SEGMENTS_FUNCTIONS_H_
#define _SEGMENTS_FUNCTIONS_H_

#include "Object_Info.h"
#include <fstream>
#include <iostream>

using namespace std;

void Usage(char *);
int read_args(int ,char **,char **,char ** ,char **,int *,char **,float *,float *,int *,int *);
int read_dataset(char *,Object_Info *** ,int,int);


#endif