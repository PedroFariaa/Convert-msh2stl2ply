#pragma once
#include "Node.h"

using namespace std;

class Element {
	int id, elem_type, nr_tags, label;
	vector<Node> nodes;
	vector<float> normal;

public:
	Element(int id, int elem_type, int label, vector<Node> nodes);
	int getType();
	vector<Node> getNodes();

};

