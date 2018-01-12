#include <iostream>
#include <fstream>
#include "../../include/Types.h"
#include "../../include/Object_Info.h"

using namespace std;


void Clustering(int,int,Object_Info **,double (*distance)(int,int),ofstream &,int);