#include "Element.h"
#include "Normal.h"
#include <fstream>
#include <iostream>

#include <Windows.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <algorithm>

using namespace std;

//file structure variables
string file_path;
string line;

vector<Node> nodes;
vector<Element> elements;
vector<string> coord;
vector<string> parse_elem;
vector<Node> element_nodes;
vector<Node> nodes2;
vector<Element> elements2;
int state = 0;


void resetVectors() {
	nodes = vector<Node>();
	elements = vector<Element>();
	nodes2 = vector<Node>();
	elements2 = vector<Element>();
}

//split line by whitespaces
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


// retrieves filename without extension from a path file
string getFilename(string file_path) {
	string we = file_path.substr( file_path.find_last_of("/") + 1 );
	size_t lastindex = we.find_last_of(".");
	return we.substr(0, lastindex);
}

void parseLine_gmsh(string line) {
	if (strcmp(line.c_str(), "$Nodes") == 0)
		state = 1;
	if (strcmp(line.c_str(), "$Elements") == 0)
		state = 2;
	if (strcmp(line.c_str(), "$EndNodes") || strcmp(line.c_str(), "$EndElements") == 0)
		//gmesh_state = 0;

	if (state == 1 && strcmp(line.c_str(), "$Nodes") != 0) {
		//split in whitespace
		coord = split(line.c_str());
		//create node
		if (coord.size() > 1) {
			Node a = Node(nodes.size() + 1, stof(coord[1]), stof(coord[2]), stof(coord[3]));
			nodes.push_back(a);
		}
	}

	if (state == 2 && strcmp(line.c_str(), "$EndNodes")) {
		//split in whitespaces
		parse_elem = split(line.c_str());
		//create Element
		if (parse_elem.size() > 1) {
			if (stoi(parse_elem[1]) == 2) {
				vector<Node> n;
				for each (Node no in nodes) {
					if (no.getId() == stoi(parse_elem[6]))
						n.push_back(no);
				}
				for each (Node no in nodes) {
					if (no.getId() == stoi(parse_elem[7]))
						n.push_back(no);
				}
				for each (Node no in nodes) {
					if (no.getId() == stoi(parse_elem[8]))
						n.push_back(no);
				}
				Element e1 = Element(elements.size() + 1, n[0].getId(), n[1].getId(), n[2].getId());
				elements.push_back(e1);
			}
		}
	}
}

void parseLine_gmsh2stl(string line) {
	if (strcmp(line.c_str(), "$Nodes") == 0)
		state = 1;
	if (strcmp(line.c_str(), "$Elements") == 0)
		state = 2;
	if (strcmp(line.c_str(), "$EndNodes") || strcmp(line.c_str(), "$EndElements") == 0)
		//gmesh_state = 0;

		if (state == 1 && strcmp(line.c_str(), "$Nodes") != 0) {
			//split in whitespace
			coord = split(line.c_str());
			//create node
			if (coord.size() > 1) {
				Node a = Node(nodes.size() + 1, stof(coord[1]), stof(coord[2]), stof(coord[3]));
				nodes.push_back(a);
			}
		}

	if (state == 2 && strcmp(line.c_str(), "$EndNodes")) {
		//split in whitespaces
		parse_elem = split(line.c_str());
		//create Element
		if (parse_elem.size() > 1) {
			if (stoi(parse_elem[1]) == 2) {
				vector<Node> n;
				for each (Node no in nodes) {
					if (no.getId() == stoi(parse_elem[6]))
						n.push_back(no);
				}
				for each (Node no in nodes) {
					if (no.getId() == stoi(parse_elem[7]))
						n.push_back(no);
				}
				for each (Node no in nodes) {
					if (no.getId() == stoi(parse_elem[8]))
						n.push_back(no);
				}
				Element e2 = Element(elements.size() + 1, stoi(parse_elem[1]), stoi(parse_elem[3]), n);
				elements2.push_back(e2);
			}
		}
	}
}

void parseLine_stl(string line) {
	bool exists = false;
	parse_elem = split(line.c_str());

	if (parse_elem[0] == "facet") {
		element_nodes = vector<Node>();
	}

	if (parse_elem[0] == "vertex") {
		//check if exists
		for each (Node n in nodes)	{
			if (n.getX() == stof(parse_elem[1]) && n.getY() == stof(parse_elem[2]) && n.getZ() == stof(parse_elem[3])){
				element_nodes.push_back(n);
				exists = true;
			}
		}
		//if doesn't
		if (exists == false){
			Node n2 = Node(nodes.size() + 1, stof(parse_elem[1]), stof(parse_elem[2]), stof(parse_elem[3]));
			nodes.push_back(n2);
			element_nodes.push_back(n2);
		}else
			exists = false;
	}
	if (parse_elem[0] == "endloop") {
		Element e2 = Element(elements.size()+1, element_nodes[0].getId(), element_nodes[1].getId(), element_nodes[2].getId());
		elements.push_back(e2);
	}
}
		
void write_stl(string out_folder, string file_path, vector<Node> nodes, vector<Element> elements) {
	if (CreateDirectory(out_folder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		ofstream outputFile(out_folder + "/" + getFilename(file_path) + ".stl");
		outputFile << "solid created\n";

		for each (Element e in elements2) {
				Normal nr = Normal(e.getNodes()[0], e.getNodes()[1], e.getNodes()[2]);
				outputFile << "facet" << " " << nr.getX() << " " << nr.getY() << " " << nr.getZ() << "\n"; //calculate facet's normal
				//outputFile << "facet" << " " << 0.0 << " " << 0.0 << " " << 0.0 << "\n"; //calculate facet's normal
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
		outputFile << "endsolid created\n";
		outputFile.close();
	}
}

void write_ply(string out_folder, string file_path, vector<Node> nodes, vector<Element> elements) {
	if (CreateDirectory(out_folder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		ofstream outputFile(out_folder + "/" +  getFilename(file_path) + ".ply");
		
		outputFile << "ply\nformat ascii 1.0\ncomment VCGLIB generated\nelement vertex " << nodes.size() << "\nproperty float x\nproperty float y\nproperty float z\nelement face " <<
			elements.size() << "\nproperty list uchar int vertex_indices\nend_header\n";
		
		//write all nodes
		for each (Node n in nodes) {
			string s = "";
			s.append(n.getX_string());
			s.append(" ");
			s.append(n.getY_string());
			s.append(" ");
			s.append(n.getZ_string());
			outputFile << s << "\n";
		}

		//write all elements
		for each (Element e in elements) {
			outputFile << "3 ";
			for each (int nid in e.getNodesId()) {
				outputFile << to_string(nid - 1) << " ";
			}
			outputFile << " \n";
		}
		outputFile.close();
	}
}


void msh2stl(string out_folder, string file_path) {
	resetVectors();
	ifstream myfile(file_path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			parseLine_gmsh2stl(line);
		}
		myfile.close();
	}
	write_stl(out_folder, file_path, nodes, elements);
}

void stl2ply(string out_folder, string file_path) {
	resetVectors();
	ifstream myfile(file_path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			parseLine_stl(line);
		}
		myfile.close();
	}
	write_ply(out_folder, file_path, nodes, elements);
}

void msh2ply(string out_folder, string file_path) {
	resetVectors();
	ifstream myfile(file_path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			parseLine_gmsh(line);
		}
		myfile.close();
	}
	write_ply(out_folder, file_path, nodes, elements);
}


void get_all_files_names_within_folder(string folder, string out_folder, int convertion_type) {
	string search_path = folder + "/*.msh";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);

	if (convertion_type == 1) {
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					//string filename_str(fd.cFileName);
					msh2ply(out_folder, folder + "/" + fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
	}
	else if(convertion_type == 2){
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					//string filename_str(fd.cFileName);
					msh2stl(out_folder, folder + "/" + fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
	}
	else {
		search_path = folder + "/*.stl";
		hFind = ::FindFirstFile(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					//string filename_str(fd.cFileName);
					stl2ply(out_folder, folder + "/" + fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
	}
}


int main() {

	//msh2ply(); --- 1
	//msh2stl(); --- 2
	//stl2ply(); --- 3

	
	//get_all_files_names_within_folder("C:/Users/Pedro/Desktop/convert", "C:/Users/Pedro/Desktop/convert/generated", 1);

	//get_all_files_names_within_folder("C:/Users/Pedro/Desktop", "C:/Users/Pedro/Desktop/generated_files", 2);
		
	//get_all_files_names_within_folder("C:/Users/Pedro/Desktop/generated_files", "C:/Users/Pedro/Desktop/generated_files/using_generated_stl", 3);

	system("cls");

	string input_dir, output_dir;
	int conv_type;


	cout << "Input the path for the directory with the files to be converted" << endl;
	cin >> input_dir;
	cout << "Choose the path for the converted files be placed" << endl;
	cin >> output_dir;

	cout << endl << "To convert msh files to ply type 1" << endl;
	cout << "To convert msh files to stl type 2" << endl;
	cout << "To convert stl files to ply type 3" << endl;
	cin >> conv_type;

	replace(input_dir.begin(), input_dir.end(), '\\', '/');
	replace(output_dir.begin(), output_dir.end(), '\\', '/');


	get_all_files_names_within_folder(input_dir, output_dir, conv_type);

	cout << "\n\n All the files are converted" << endl;
	system("pause");

	main();
	


	/*
	To all patients according with our folder structure
	*//*
	string patient[] = { "003", "007", "008", "013", "014", "017" };
	string lat[] = { "right", "right", "left", "left", "right", "left" };
	string birad[] = { "I", "II", "III", "IV" };
	vector<string> patients(patient, patient + sizeof(patient) / sizeof(string));
	vector<string> laterality(lat, lat + sizeof(lat) / sizeof(string));
	vector<string> BIRAD(birad, birad + sizeof(birad) / sizeof(string));
	string dir_root = "C:/Users/Pedro/Dropbox/";

	for (int a = 0; a < patients.size(); a++)
		for (int b = 0; b < BIRAD.size(); b++)
			for (int r = 1; r<6; r++)
				for (int s = 1; s < 4; s++) {
					cout << "CONVERTING Mesh of patient: " + patient[a] + "   " + laterality[a] + " breast" + "     BIRAD " + BIRAD[b] + "    Region" + to_string(r) + "    Size " + to_string(s) << endl;

					dir = dir_root + "ToServer/Patients/patient_" + patients[a] + "/" + laterality[a] + "_breast/Simulations/" + BIRAD[b]
						+ "/p_"+patients[a] + "_" + laterality[a] + "_" + BIRAD[b] + "-upright";
					get_all_files_names_within_folder(dir, dir, 1);

					dir = dir_root + "ToServer/Patients/patient_" + patients[a] + "/" + laterality[a] + "_breast/Simulations/" + BIRAD[b] + "/R_" + to_string(r) + "/S_" + to_string(s) + "/"
						+ "p_"+patients[a] + "_" + laterality[a] + "_" + BIRAD[b] + "_R" + to_string(r) + "_S" + to_string(s) + "-wound_healing";
					get_all_files_names_within_folder(dir, dir, 1);
				}

	*/
	return 0;

}
