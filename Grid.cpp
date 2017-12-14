#include"stdafx.h"
#include"Grid.h"

Grid::Grid() {
	coorX = coorY = points = 0;
}
Grid::Grid(const Grid& x) {
	if (this == &x) return;
	if (x.points != 0) {
		height = x.height;
		width = x.width;
		size = x.size;
		points = new double[size];
		coorX = new double[size];
		coorY = new double[size];
		memcpy(points, x.points, sizeof(double)*size);
		memcpy(coorX, x.coorX, sizeof(double)*size);
		memcpy(coorY, x.coorY, sizeof(double)*size);
	}
}
Grid::Grid(const Grid& x, double* p) {
	if (this != &x) {
		if (x.points != 0) {
			height = x.height;
			width = x.width;
			size = x.size;
			points = new double[size];
			coorX = new double[size];
			coorY = new double[size];
			memcpy(coorX, x.coorX, sizeof(double)*size);
			memcpy(coorY, x.coorY, sizeof(double)*size);
		}
	}
	if (x.points != 0) memcpy(points, p, sizeof(double)*size);
}
Grid::~Grid() {
	if (points != 0) {
		delete[] points;
		delete[] coorX;
		delete[] coorY;
	}
}
Grid& Grid::operator=(const Grid& x) {
	if (this == &x) return *this;
	if (points != 0) {
		delete[] points;
		delete[] coorX;
		delete[] coorY;
	}
	if (x.points != 0) {
		height = x.height;
		width = x.width;
		size = x.size;
		points = new double[size];
		coorX = new double[size];
		coorY = new double[size];
		memcpy(points, x.points, sizeof(double)*size);
		memcpy(coorX, x.coorX, sizeof(double)*size);
		memcpy(coorY, x.coorY, sizeof(double)*size);
	}
	return *this;
}
void Grid::readFromFile(char* fileName) {
	struct p {
		double x, y, v;
		bool operator < (const p &r) const {
			return x < r.x || x == r.x&&y < r.y;
		}
	};
	std::vector<p> s;
	double a;
	FILE* fp = fopen(fileName, "r");
	if (!fp) return;
	while (fscanf(fp, "%lf", &a) != EOF) {
		p x;
		x.x = a;
		fscanf(fp, "%lf%lf", &x.y, &x.v);
		s.push_back(x);
	}
	fclose(fp);
	size = s.size();
	std::sort(s.begin(), s.end());
	points = new double[size];
	coorX = new double[size];
	coorY = new double[size];
	for (int i = 0; i < size; i++) {
		coorX[i] = s[i].x;
		coorY[i] = s[i].y;
		points[i] = s[i].v;
	}
	for (int i = 0; i < size; i++) {
		if (coorX[i] != coorX[0]) {
			width = i;
			break;
		}else if (i==size-1) width = size;
	}
	height = size / width;
}
void Grid::writeToFile(char* fileName) {
	FILE* fp = fopen(fileName, "w");
	for (int i = 0; i < size; i++) fprintf(fp, "%.16lf %.16lf %.16lf\r\n", coorX[i],coorY[i],points[i]);
	fclose(fp);
}
bool Grid::isValid()const {
	return points != 0;
}
int Grid::getHeight()const {
	return height;
}
int Grid::getWidth()const {
	return width;
}
int Grid::getSize()const {
	return size;
}
double Grid::getS()const {
	return (coorX[size - 1] - coorX[0]) / height * 111;
}
double Grid::getT()const {
	return(coorY[size-1] - coorY[0]) / width*111;
}
double Grid::getPoint(int x, int y)const {
	return points[x*width + y];
}