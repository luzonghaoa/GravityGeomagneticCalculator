#pragma once

#include<cmath>
#include<algorithm>

#include"Grid.h"

class FFT {
public:
	struct Complex {
		double real, imag;
		Complex(double = 0.0, double = 0.0);
		Complex operator+(const Complex&) const;
		Complex operator-(const Complex&) const;
		Complex operator*(const Complex&) const;
	};
	static Complex* continuation(const Grid&, Complex*, double);
	static Complex* derivation(const Grid&, Complex*);
	static Complex* initialize(const Grid&);
	static void retrieve(Grid&, Complex*);
	static int extendLength(int);
private:
	static void change(Complex*, int);
	static void fft(Complex*, int, int);
};