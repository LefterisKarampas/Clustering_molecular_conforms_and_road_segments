#include "../include/Update.h"
#include "../include/Object_Info.h"
#include "../include/Cluster.h"
#include "../include/Distance.h"
#include "../include/Types.h"
#include <iostream>
#include <vector>


using namespace std;

extern Object_Info ** object_info;
extern double ** Distance_Table;


void PAM_Improved(Clusters *clusters,double (*distance)(int,int)){
	//For each clsuter, find the new medoid
	for(unsigned int i=0;i<clusters->size();i++){
		//Get cluster's neighbors
		Neighbors neigh = (*clusters)[i]->Cluster_Get_Neighbors();
		unsigned int num_neigh = neigh.size();
		double temp_dist;
		double min;
		double min_index;
		//For each neighbor
		for(int j=0;j<num_neigh;j++){
			temp_dist = 0;
			//For each unvisited neighbors find the distance with the current neighbor
			for(unsigned int k=0;k<num_neigh;k++){
				temp_dist += Find_Distance(neigh[j],neigh[k],distance,Distance_Table);
			}
			if(j==0){
				min = temp_dist;
				min_index = neigh[j];
			}
			else if(temp_dist < min ){
				min = temp_dist;
				min_index = neigh[j];
			}
		}
		(*clusters)[i]->Cluster_Update(min_index,min);
	}
}



Object_Info * Create_MeanFrechet(Neighbors neigh){
	Object_Info *new_center;
	int loop = 0;
	std::vector<Object *> *Mean  = new std::vector<Object *>;
	int m = 0;
	double dist;
	do{
		cout << loop << endl;
		cout << Mean->size() << endl;
		if(loop == 0){
			unsigned int size = neigh.size();
			for(unsigned int i=0;i<size;i+=2){
				if(i == size-1){
					Mean->push_back(MeanFrechet(object_info[neigh[i]]->Get_Object_P(),NULL,&dist));
				}
				else{
					Mean->push_back(MeanFrechet(object_info[neigh[i]]->Get_Object_P(),object_info[neigh[i+1]]->Get_Object_P(),&dist));
					int index_b;
					int index_l;
					if(neigh[i] > neigh[i+1]){
						index_b = neigh[i];
						index_l = neigh[i+1];
					}
					else{
						index_b = neigh[i+1];
						index_l = neigh[i];
					}
					if(Distance_Table[index_b][index_l] == -1){
						Distance_Table[index_b][index_l] = dist;
					}
				}
			}
			if(size == 0){
				delete Mean;
				return NULL;
			}
		}
		else{
			unsigned int size = Mean->size();
			int n = 0;
			std::vector<Object *> *temp  = new std::vector<Object *>;
			for(unsigned int i=0;i<size;i+=2){
				cout << "i: " << i << endl;
				cout << (*Mean)[i]->size() << endl;
				cout << (*(*Mean)[i])[0].size() << endl;
				if(i == size-1){
					temp->push_back(MeanFrechet((*Mean)[i],NULL,&dist));
					delete (*Mean)[i];
				}
				else{
					temp->push_back(MeanFrechet((*Mean)[i],(*Mean)[i+1],&dist));
					delete (*Mean)[i];
					delete (*Mean)[i+1];
				}
			}
			m = n;
			delete Mean;
			Mean = temp;
		}
		loop++;
	}while(Mean->size() != 1);
	new_center = new Object_Info((*Mean)[0]);
	delete Mean;
	return new_center;
}


double Mean_Discrete_Frechet(Clusters *clusters,int n,double (*distance)(int,int)){
	for(unsigned int i=0;i<clusters->size();i++){
		Neighbors neigh = (*clusters)[i]->Cluster_Get_Neighbors();
		Object_Info * object = Create_MeanFrechet(neigh);
		if(object != NULL){
			if(object_info[n+i] != NULL){
				delete object_info[n+i];
				for(int j=0;j<n;j++){
					Distance_Table[n+i][j] = -1;
				}
			}
			object_info[n+i] = object;
			(*clusters)[i]->Cluster_Update(n+i,0);
		}
	}
}