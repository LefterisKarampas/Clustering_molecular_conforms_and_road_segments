#include <iostream>
#include <vector>
#include "Ways.h"
#include "Node.h"

using namespace std;

typedef struct {
	std::vector<int> *index;
	std::vector<int> *pos;
}Mapping;


class NodeWay: public Node{
	Mapping *map;
public:
	NodeWay(int,Point *,int);
	~NodeWay();
	void Update_Belongs(int,int,Way **);	
};