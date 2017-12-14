#pragma once

#include<vector>

#include"FFT.h"
#include"Grid.h"

class Func {
public:
	static void readFile(char*);
	static void writeFile(char*);
	static void redo();
	static void undo();
	static void continuation(double);
	static void derivation();
	static void clean();
	static void draw(HWND hWnd);
	static WCHAR message[100];
	struct Status {
		int length;
		std::vector<FFT::Complex*> logs;
	};
	static Status record;
	static Grid st,cur;
};

