#include "Element.h"


Element::Element(int id, int elem_type, int label, vector<Node> nodes){
	this->id = id;
	this->elem_type = elem_type;
	this->label = label;
	for each (Node n in nodes){
		this->nodes.push_back(n);
	}
}

int Element::getType(){
	return this->elem_type;
}

vector<Node> Element::getNodes(){
	return this->nodes;
}

