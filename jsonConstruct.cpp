#include <iostream>
#include <string>
#include <vector>
#include "jsonConstruct.h"

jsonConstruct::jsonConstruct(std::vector<std::string>& vec) {
	this->vec = vec;
}

std::string jsonConstruct::data() {
	
	std::string data = "{";
	data.append("\"count\":");
	data.append(std::to_string(vec.size() - 1));
	data.append(", \"clear\" : false, \"github\" : true, \"timer\" : false, \"cords\" : [");
	for (int i = 0; i < vec.size(); i++) {
		data.append(vec[i]);
		if (i != vec.size() - 1) {
			data.append(", ");
		}
	}
	data.append("]}");
	return data;
}

std::string jsonConstruct::clear() {
	std::string data = "{";
	data.append("\"count\":");
	data.append(std::to_string(0));
	data.append(", \"clear\" : true, \"github\" : false, \"timer\" : false, \"cords\" : []}");
	return data;
}

std::string jsonConstruct::timer() {
	std::string data = "{";
	data.append("\"count\":");
	data.append(std::to_string(vec.size() - 1));
	data.append(", \"clear\" : false, \"github\" : false, \"timer\" : true, \"cords\" : [");
	for (int i = 0; i < vec.size(); i++) {
		data.append(vec[i]);
		if (i != vec.size() - 1) {
			data.append(", ");
		}
	}
	data.append("]}");
	return data;
}

