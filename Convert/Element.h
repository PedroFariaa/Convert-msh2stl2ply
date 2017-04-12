#pragma once
#include "Node.h"

using namespace std;

class Element {
	int id, elem_type, nr_tags, label;
	vector<Node> nodes;
	vector<int> nodes_int;
	vector<float> normal;

public:
	Element(int id, int elem_type, int label, vector<Node> nodes);
	Element(int id, int elem_type, Node node1, Node node2, Node node3);
	Element(int id, int node1, int node2, int node3);
	int getType();
	vector<Node> getNodes();
	vector<int> getNodesId();

};

