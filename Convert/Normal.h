#pragma once
#include "Node.h"
#include <math.h> 

class Normal{
	vector<float> vertex1, vertex2, vertex3;
	vector<float> vec1, vec2;
	vector<float> normal;
	vector<float> normal_normalized;
public:
	Normal(Node a, Node b, Node c);
	string getX();
	string getY();
	string getZ();

};

