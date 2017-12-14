#include "stdafx.h"
#include "Func.h"
Func::Status Func::record;
Grid Func::st;
Grid Func::cur;
WCHAR Func::message[100];
void Func::readFile(char* fileName) {
	clean();
	st.readFromFile(fileName);
	cur = st;
	record.logs.push_back(FFT::initialize(st));
	record.length++;
	wsprintfW(message, L"\n    Import the data successfully!");
}
void Func::writeFile(char* fileName) {
	if (record.length > 0) {
		Grid g(st);
		FFT::retrieve(g, record.logs[record.length - 1]);
		g.writeToFile(fileName);
		wsprintfW(message, L"\n    Export the data successfully!");
	}else wsprintfW(message, L"\n    No data in workspace! Fail to export the data!");
	
}
void Func::undo() {
	if (record.length > 1) {
		record.length--;
		cur = Grid(st);
		FFT::retrieve(cur, record.logs[record.length - 1]);
		wsprintfW(message, L"\n    Undo the previous operation.");
	}
}
void Func::redo() {
	if (record.length < record.logs.size()) {
		record.length++;
		cur = Grid(st);
		FFT::retrieve(cur, record.logs[record.length - 1]);
		wsprintfW(message, L"\n    Redo the cancelled operation.");
	}
}
void Func::continuation(double z) {
	while (record.length < record.logs.size()) {
		delete[] record.logs[record.logs.size() - 1];
		record.logs.pop_back();
	}
	if (record.logs.size()) {
		record.logs.push_back(FFT::continuation(st,record.logs[record.logs.size() - 1], z));
		record.length++;
		cur = Grid(st);
		FFT::retrieve(cur, record.logs[record.length - 1]);
		wsprintfW(message, L"\n    Continuation has been done successfully!");
	}else wsprintfW(message, L"\n    No data in workspace! Fail to do the continuation!");
}
void Func::derivation() {
	while (record.length < record.logs.size()) {
		delete[] record.logs[record.logs.size() - 1];
		record.logs.pop_back();
	}
	if (record.logs.size()) {
		record.logs.push_back(FFT::derivation(st,record.logs[record.logs.size() - 1]));
		record.length++;
		cur = Grid(st);
		FFT::retrieve(cur, record.logs[record.length - 1]);
		wsprintfW(message, L"\n    Derivation has been done successfully!");
	}
	else wsprintfW(message, L"\n    No data in workspace! Fail to do the derivation!");
}
void Func::clean() {
	record.length = 0;
	for (int i = 0; i < record.logs.size(); i++) delete[] record.logs[i];
	record.logs.clear();
	wsprintfW(message, L"\n    No data in workspace!");
}
void Func::draw(HWND hWnd) {
	if (record.length > 0) {
		COLORREF clrs[20] = {
			RGB(0,0,0),RGB(0, 0, 122),RGB(0,0,244),RGB(0, 81, 255),
			RGB(0,171,255),RGB(0, 254, 251),RGB(0,230,193),RGB(0, 207, 135),
			RGB(0,184,76),RGB(0, 160, 18),RGB(94,190,0),RGB(227, 244, 0),
			RGB(255,205,0),RGB(255, 141, 0),RGB(255,77,0),RGB(255, 13, 0),
			RGB(255,67,67),RGB(255, 151, 151),RGB(255,235,235),RGB(255, 255, 255)
		};
		HDC hDC = GetDC(hWnd);
		double h = cur.getS()*cur.getHeight(),w=cur.getT()*cur.getWidth();
		if (h / w <= 1.5) {
			h = 600 * h / w;
			w = 600;
		}
		else {
			w = 900 * w / h;
			h = 900;
		}
		double ul = -1e11, dl = 1e11;
		for (int i = 0; i < cur.getHeight(); i++) for (int j = 0; j < cur.getWidth(); j++) {
			ul = max(ul, cur.getPoint(i, j));
			dl = min(dl, cur.getPoint(i, j));
		}
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
			double sum = 0, cnt = 0;
			int ks = (double)i / h*cur.getHeight(), kf, ls = (double)j / w*cur.getWidth(), lf;
			if (h >= cur.getHeight()) kf = ks + 1;
			else kf= (double)(i+1) / h*cur.getHeight();
			if (w >= cur.getWidth()) lf = ls + 1;
			else lf = (double)(j + 1) / w*cur.getWidth();
			for (int k = ks; k < kf; k++) for (int l = ls; l < lf;l++) {
				sum++;
				cnt += cur.getPoint(k, l);
			}
			int v=min(20*(cnt/sum - dl) / (ul - dl),19);
			if (v == 20) v = 19;
			SetPixel(hDC, i+15, 650-j, clrs[v]);
		}
		ReleaseDC(hWnd, hDC);
	}
}