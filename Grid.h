#pragma once

#include<cstring>
#include<cstdio>
#include<vector>
#include<algorithm>

class Grid{
public:
	Grid();
	Grid(const Grid&);
	Grid(const Grid&, double*);
	~Grid();
	Grid& operator=(const Grid&);
	void readFromFile(char*);
	void writeToFile(char*);
	bool isValid()const;
	int getHeight()const;
	int getWidth()const;
	int getSize()const;
	double getS()const;
	double getT()const;
	double getPoint(int, int)const;
private:
	int height, width, size;
	double *points, *coorX, *coorY;
};