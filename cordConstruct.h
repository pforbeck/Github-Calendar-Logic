#pragma once
#include <iostream>
#include <string>


class cordConstruct {
public:
	cordConstruct(int x, int y, int r, int g, int b);
	void setX(int x);
	void setY(int y);
	void setR(int r);
	void setG(int g);
	void setB(int b);
	int getX();
	int getY();
	int getR();
	int getG();
	int getB();
	std::string data();
private:
	int x;
	int y;
	int r;
	int g;
	int b;
};