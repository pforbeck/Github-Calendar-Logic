#include <iostream>
#include <string>
#include "cordConstruct.h"

cordConstruct::cordConstruct(int x, int y, int r, int g, int b) {
	this->x = x;
	this->y = y;
	this->r = r;
	this->g = g;
	this->b = b;
}

void cordConstruct::setX(int x) {
	this->x = x;
}

void cordConstruct::setY(int y) {
	this->y = y;
}

void cordConstruct::setR(int r) {
	this->r = r;
}

void cordConstruct::setG(int g) {
	this->g = g;
}

void cordConstruct::setB(int b) {
	this->b = b;
}

int cordConstruct::getX() {
	return this->x;
}

int cordConstruct::getY() {
	return this->y;
}

int cordConstruct::getR() {
	return this->r;
}

int cordConstruct::getG() {
	return this->g;
}

int cordConstruct::getB() {
	return this->b;
}

std::string cordConstruct::data() {
	std::string data = "[";
	data.append(std::to_string(x));
	data.append(", ");
	data.append(std::to_string(y));
	data.append(", ");
	data.append(std::to_string(r));
	data.append(", ");
	data.append(std::to_string(g));
	data.append(", ");
	data.append(std::to_string(b));
	data.append("]");
	return data;
}
