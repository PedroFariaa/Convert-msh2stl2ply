#include "Node.h"


Node::Node(int id, float x, float y, float z){
	this->id = id;
	this->X_coord = x;
	this->Y_coord = y;
	this->Z_coord = z;
}


string Node::getCoords(){
	//ss << this->X_coord;
	string a = "";
	a.append(to_string(this->X_coord));
	return string();
}

int Node::getId(){
	return this->id;
}

float Node::getX()
{
	return this->X_coord;
}

float Node::getY()
{
	return this->Y_coord;
}

float Node::getZ()
{
	return this->Z_coord;
}

string Node::getID_string() {
	return to_string(this->id);
}

string Node::getX_string(){
	return to_string(this->X_coord);
}

string Node::getY_string() {
	return to_string(this->Y_coord);
}

string Node::getZ_string() {
	return to_string(this->Z_coord);
}
