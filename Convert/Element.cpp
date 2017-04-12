#include "Element.h"


Element::Element(int id, int elem_type, int label, vector<Node> nodes){
	this->id = id;
	this->elem_type = elem_type;
	this->label = label;
	for each (Node n in nodes){
		this->nodes.push_back(n);
	}
}

Element::Element(int id, int elem_type, Node node1, Node node2, Node node3) {
	this->id = id;
	this->elem_type = elem_type;
	this->nodes.push_back(node1);
	this->nodes.push_back(node2);
	this->nodes.push_back(node3);
}

Element::Element(int id, int node1, int node2, int node3){
	this->id = id;
	this->nodes_int.push_back(node1);
	this->nodes_int.push_back(node2);
	this->nodes_int.push_back(node3);
}

int Element::getType(){
	return this->elem_type;
}

vector<Node> Element::getNodes(){
	return this->nodes;
}

vector<int> Element::getNodesId() {
	return this->nodes_int;
}

