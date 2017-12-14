#include "stdafx.h"
#include"FFT.h"

FFT::Complex* FFT::continuation(const Grid& g, Complex* inp,double z) {
	int h = extendLength(g.getHeight()), w = extendLength(g.getWidth());
	double lx=g.getS()*h, ly = g.getT()*w;
	Complex *freq = new Complex[w*h];
	memcpy(freq, inp, sizeof(Complex)*w*h);
	if (z < 0) for (int i = -h / 2; i < h / 2; i++) for (int j = -w / 2; j < w / 2;j++) {
		freq[(i + h / 2)*w + j + w / 2].real *= exp(2 * acos(-1.0)*z*sqrt(i*i / lx / lx + j*j / ly / ly));
		freq[(i + h / 2)*w + j + w / 2].imag *= exp(2 * acos(-1.0)*z*sqrt(i*i / lx / lx + j*j / ly / ly));
	}
	else if (z > 0) for (int i = -h / 2; i < h / 2; i++){
		for (int j = -w / 2; j < w / 2; j++) {
			freq[(i + h / 2)*w + j + w / 2].real *= exp(acos(-1.0)*z*sqrt(i*i / lx / lx + j*j / ly / ly)*(1 + cos(acos(-1.0) / 2 * sqrt((i*i*lx*lx / h / h + j*j*ly*ly / w / w) / (lx*lx + ly*ly)))));
			freq[(i + h / 2)*w + j + w / 2].imag *= exp(acos(-1.0)*z*sqrt(i*i / lx / lx + j*j / ly / ly)*(1 + cos(acos(-1.0)*2* sqrt((i*i*lx*lx / h / h + j*j*ly*ly / w / w) / (lx*lx + ly*ly)))));
		}
	}
	return freq;
}
FFT::Complex* FFT::derivation(const Grid& g, Complex* inp) {
	int h = extendLength(g.getHeight()), w = extendLength(g.getWidth());
	double lx = g.getS()*h, ly = g.getT()*w;
	Complex *freq = new Complex[w*h];
	memcpy(freq, inp, sizeof(Complex)*w*h);
	for (int i = -h / 2; i < h / 2; i++) for (int j = -w / 2; j < w / 2; j++) {
		freq[(i + h / 2)*w + j + w / 2].real *= 2 * acos(-1.0)*sqrt(i*i / lx / lx + j*j / ly / ly);
		freq[(i + h / 2)*w + j + w / 2].imag *= 2 * acos(-1.0)*sqrt(i*i / lx / lx + j*j / ly / ly);
	}
	return freq;
}
FFT::Complex::Complex(double r, double i) {
	real = r;
	imag = i;
}
FFT::Complex FFT::Complex::operator+(const Complex& x)const {
	return Complex(real + x.real, imag + x.imag);
}
FFT::Complex FFT::Complex::operator-(const Complex& x)const {
	return Complex(real - x.real, imag - x.imag);
}
FFT::Complex FFT::Complex::operator*(const Complex& x)const {
	return Complex(real*x.real - imag*x.imag, real*x.imag + imag*x.real);
}
FFT::Complex* FFT::initialize(const Grid& g) {
	int h = extendLength(g.getHeight()), w = extendLength(g.getWidth());
	Complex* f = new Complex[h*w];
	int td = (h - g.getHeight()) / 2,bd=h-g.getHeight()-td, ld = (w - g.getWidth()) / 2,rd= w - g.getWidth() - ld;
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) {
		int x, y;
		x = max(i, td);
		x = min(x, h-1-bd);
		y = max(j, ld);
		y = min(y, w - 1 - rd);
		f[i*w + j].real = g.getPoint(x-td,y-ld);
		f[i*w + j].imag = 0;
	}
	
	for (int i = 0; i < td; i++) for (int j = 0; j < w ; j++) {
		f[i*w + j].real *= (1 - cos(acos(-1.0) / td*i)) / 2;
	}
	for (int i = h - bd; i < h; i++) for (int j = 0; j < w; j++) {
		f[i*w + j].real *= (1 - cos(acos(-1.0) / bd*(h-1-i))) / 2;
	}
	for (int i = 0; i < h; i++) for (int j = 0; j < ld; j++) {
		f[i*w + j].real *= (1 - cos(acos(-1.0) / ld*j)) / 2;
	}
	for (int i = 0; i < h; i++) for (int j = w - rd; j < w; j++) {
		f[i*w + j].real *= (1 - cos(acos(-1.0) / rd*(w-1-j))) / 2;
	}
	Complex *c= new Complex[h*w];
	for (int i = 0; i < h; i++) fft(f+i*w, w, 1);
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) c[j*h + i] = f[i*w + j];
	for (int i = 0; i < w; i++) fft(c+i*h, h, 1);
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) f[i*w + j] = c[j*h + i];
	delete[] c;
	for (int i = 0; i < h / 2; i++) for (int j = 0; j < w / 2; j++) {
		std::swap(f[i*w + j], f[(i + h / 2)*w + j + w / 2]);
		std::swap(f[i*w + j + w / 2], f[(i + h / 2)*w + j]);
	}
	return f;
}
void FFT::retrieve(Grid& g, Complex* inp) {
	int h = extendLength(g.getHeight()), w = extendLength(g.getWidth());
	Complex* f = new Complex[h*w];
	memcpy(f, inp, sizeof(Complex)*w*h);
	for (int i = 0; i < h / 2; i++) for (int j = 0; j < w / 2; j++) {
		std::swap(f[i*w + j], f[(i + h / 2)*w + j + w / 2]);
		std::swap(f[i*w + j + w / 2], f[(i + h / 2)*w + j]);
	}
	Complex *c = new Complex[h*w];
	for (int i = 0; i < h; i++) fft(f + i*w, w, -1);
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) c[j*h + i] = f[i*w + j];
	for (int i = 0; i < w; i++) fft(c + i*h, h, -1);
	for (int i = 0; i < h; i++) for (int j = 0; j < w; j++) f[i*w + j] = c[j*h + i];
	delete[] c;
	double *ret = new double[g.getSize()];
	int td=(h-g.getHeight())/2, ld= (w - g.getWidth()) / 2;
	for (int i = 0; i < g.getHeight(); i++) for (int j = 0; j < g.getWidth(); j++) {
		ret[i*g.getWidth() + j] = f[(i+td)*w+j+ld].real;
	}
	g = Grid(g, ret);
	delete[] ret;
	delete[] f;
}
int FFT::extendLength(int x){
	int ret = 1;
	while (ret < x) ret <<= 1;
	return ret<<1;
}
void FFT::change(Complex *y, int l){
	register int i, j, k;
	for (i = 1, j = l / 2; i<l - 1; i++){
		if (i<j)  std::swap(y[i], y[j]);  
		k = l / 2;
		while (j >= k){
			j -= k;
			k /= 2;
		}
		if (j<k)  j += k;
	}
}
void FFT::fft(Complex* y, int l, int on){
	register int h, i, j, k;
	Complex u,t;
	double pi = acos(-1.0);
	change(y, l);
	for (h = 2; h <= l; h <<= 1){ 
		Complex wn(cos(-on * 2 * pi / h), sin(-on * 2 * pi / h));
		for (j = 0; j<l; j += h) {
			Complex w(1, 0); 
			for (k = j; k<j + h / 2; k++){
				u = y[k];
				t = w*y[k + h / 2];
				y[k] = u + t;
				y[k + h / 2] = u - t;
				w = w*wn; 
			}
		}
	}
	if (on == -1)  for (i = 0; i < l; i++) {
		y[i].real /= l;
		y[i].imag /= l;
	}
}