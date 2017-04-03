#pragma once
#include <string>
#include <vector>

using namespace std;

class Node {
	float X_coord, Y_coord, Z_coord;
	int id;
public:
	Node(int id, float x, float y, float z);
	string getCoords();
	int getId();
	float getX();
	float getY();
	float getZ();
	string getX_string();
	string getY_string();
	string getZ_string();
};
