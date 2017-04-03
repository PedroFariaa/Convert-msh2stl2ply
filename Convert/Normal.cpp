#include "Normal.h"


Normal::Normal(Node a, Node b, Node c){
	vertex1.push_back(a.getX()); vertex1.push_back(a.getY()); vertex1.push_back(a.getZ());
	vertex2.push_back(b.getX()); vertex2.push_back(b.getY()); vertex2.push_back(b.getZ());
	vertex3.push_back(c.getX()); vertex3.push_back(c.getY()); vertex3.push_back(c.getZ());

	vec1.push_back(vertex2[0] - vertex1[0]); vec1.push_back(vertex2[1] - vertex1[1]); vec1.push_back(vertex2[2] - vertex1[2]);
	vec2.push_back(vertex3[0] - vertex1[0]); vec2.push_back(vertex3[1] - vertex1[1]); vec2.push_back(vertex3[2] - vertex1[2]);

	normal.push_back(vec1[1] * vec2[2] - vec2[1] * vec1[2]); normal.push_back(vec1[2] * vec2[0] - vec2[2] * vec1[0]); normal.push_back(vec1[0] * vec2[1] - vec2[0] * vec1[1]);
	float normal_factor = sqrt (normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	normal_normalized.push_back(normal[0] / normal_factor); normal_normalized.push_back(normal[1] / normal_factor); normal_normalized.push_back(normal[2] / normal_factor);
}

string Normal::getX(){
	return to_string(this->normal_normalized[0]);
}

string Normal::getY() {
	return to_string(this->normal_normalized[1]);
}

string Normal::getZ() {
	return to_string(this->normal_normalized[2]);
}
