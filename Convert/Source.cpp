#include "Element.h"
#include "Normal.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<Node> nodes = vector<Node>();
vector<Element> elements;
vector<string> coord;
vector<string> elem;
int gmesh_state = 0;

vector<Node> nodes2;
vector<Element> elements2;
vector<string> parse_elements;
vector<Node> element_nodes;
int stl_state = 0;

vector<string> split(const string& s)
{
	vector<string> ret;
	typedef string::size_type string_size;
	string_size i = 0;

	// invariant: we have processed characters [original value of i, i) 
	while (i != s.size()) {
		// ignore leading blanks
		// invariant: characters in range [original i, current i) are all spaces
		while (i != s.size() && isspace(s[i]))
			++i;

		// find end of next word
		string_size j = i;
		// invariant: none of the characters in range [original j, current j)is a space
		while (j != s.size() && !isspace(s[j]))
			j++;

		// if we found some nonwhitespace characters 
		if (i != j) {
			// copy from s starting at i and taking j - i chars
			ret.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return ret;
}

void parseLine_gmsh(string line) {
	if (strcmp(line.c_str(), "$Nodes") == 0)
		gmesh_state = 1;
	if (strcmp(line.c_str(), "$Elements") == 0)
		gmesh_state = 2;
	if (strcmp(line.c_str(), "$EndNodes") || strcmp(line.c_str(), "$EndElements") == 0)
		//gmesh_state = 0;

	if (gmesh_state == 1 && strcmp(line.c_str(), "$Nodes") != 0) {
		//split in whitespace
		coord = split(line.c_str());
		//create node
		if (coord.size() > 1) {
			Node a = Node(nodes.size() + 1, stof(coord[1]), stof(coord[2]), stof(coord[3]));
			nodes.push_back(a);
		}
	}

	if (gmesh_state == 2 && strcmp(line.c_str(), "$EndNodes")) {
		//split in whitespaces
		elem = split(line.c_str());
		//create Element
		if (elem.size() > 1) {
			if (stoi(elem[1]) == 2) {
				vector<Node> n;
				for each (Node no in nodes) {
					if (no.getId() == stoi(elem[6]))
						n.push_back(no);
				}
				for each (Node no in nodes) {
					if (no.getId() == stoi(elem[7]))
						n.push_back(no);
				}
				for each (Node no in nodes) {
					if (no.getId() == stoi(elem[8]))
						n.push_back(no);
				}
				Element el = Element(elements.size() + 1, stoi(elem[1]), stoi(elem[3]), n);
				elements.push_back(el);
			}
		}
	}
}

void parseLine_stl_vertex(string line) {
	bool exists = false;
	parse_elements = split(line.c_str());

	if (parse_elements[0] == "facet") {
		element_nodes = vector<Node>();
	}

	if (parse_elements[0] == "vertex") {
		//check if exists
		for each (Node n in nodes2)	{
			if (n.getX() == stof(parse_elements[1]) && n.getY() == stof(parse_elements[2]) && n.getZ() == stof(parse_elements[3])){
				element_nodes.push_back(n);
				exists = true;
			}
		}
		//if doesn't
		if (exists == false){
			Node n2 = Node(nodes2.size() + 1, stof(parse_elements[1]), stof(parse_elements[2]), stof(parse_elements[3]));
			nodes2.push_back(n2);
			element_nodes.push_back(n2);
		}else
			exists = false;
	}
	if (parse_elements[0] == "endloop") {
		Element e2 = Element(elements2.size()+1, element_nodes[0].getId(), element_nodes[1].getId(), element_nodes[2].getId());
		elements2.push_back(e2);
	}
}

void parseLine_stl_element(string line) {
	//
}

/*
void parseLine_stl(string line) {
	if (strcmp(line.c_str(), "end_header") == 0)
		stl_state = 1;

	if(stl_state == 1) {
		parse_elements = split(line.c_str());
		if (parse_elements.size() == 3) {
			//create Node
			Node n2 = Node(nodes2.size() + 1, stof(parse_elements[0]), stof(parse_elements[1]), stof(parse_elements[2]));
			nodes2.push_back(n2);
			cout << "created node\n";
		} else {
			//create Element
			Element e2 = Element(elements2.size()+1, stoi(parse_elements[1]), stoi(parse_elements[2]), stoi(parse_elements[3]));
			elements2.push_back(e2);
			cout << "created element\n";
		}
	}
}
*/

		
void write_stl() {
	ofstream outputFile("program3data.stl"); //output name needs to be changed
	outputFile << "solid created\n";
	for each (Element e in elements) {
		if (e.getType() == 2) {
			Normal nr = Normal(e.getNodes()[0], e.getNodes()[1], e.getNodes()[2]);
			outputFile << "facet" << " " << nr.getX() << " " << nr.getY() << " " << nr.getZ() << "\n"; //calculate facet's normal
			outputFile << "outer loop\n";
			for each (Node n in e.getNodes()) {
				string s = "vertex ";
				s.append(n.getX_string());
				s.append(" ");
				s.append(n.getY_string());
				s.append(" ");
				s.append(n.getZ_string());
				outputFile << s << "\n";
			}
			outputFile << "endloop\n" << "endfacet\n";
		}
	}
	outputFile << "endsolid created\n";
	outputFile.close();
}

void write_ply() {
	ofstream outputFile("program3data.ply"); //output name needs to be changed
	//writting header to file
	outputFile << "ply\nformat ascii 1.0\ncomment VCGLIB generated\nelement vertex " << nodes2.size() << "\nproperty float x\nproperty float y\nproperty float z\nelement face " << 
		elements2.size() << "\nproperty list uchar int vertex_indices\nend_header\n";
	//write all nodes
	for each (Node n in nodes2)	{
		string s = "";
		s.append(n.getX_string());
		s.append(" ");
		s.append(n.getY_string());
		s.append(" ");
		s.append(n.getZ_string());
		outputFile << s << "\n";
	}
	//write all elements
	for each (Element e in elements2) {
		outputFile << "3 ";
		for each (int nid in e.getNodesId()) {
			outputFile << to_string(nid-1) << " ";
		}
		outputFile << " \n";
	}
	outputFile.close();
}

void msh2stl() {
	string file_path = "../msh.msh";
	string line;
	ifstream myfile(file_path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			parseLine_gmsh(line);
		}
		myfile.close();
	}
	write_stl();
}

void stl2ply() {
	string file_path = "../stl.stl";
	string line;
	ifstream myfile(file_path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			parseLine_stl_vertex(line);
		}
		myfile.close();
	}
	write_ply();
}

int main() {
	//msh2stl();
	stl2ply();
	return 0;
}