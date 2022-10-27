#pragma once
#include <iostream>
#include <string>
#include <vector>

class jsonConstruct {
public:
	jsonConstruct(std::vector<std::string>& vec);
	std::string data();
	std::string clear();
	std::string timer();
private:
	std::vector<std::string> vec;
};