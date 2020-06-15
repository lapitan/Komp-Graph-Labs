#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

class PGM {
private:
	char vers[2];
	int width;
	int height;
	int colorDepth;
	vector<vector<unsigned char>> pict;
public:
	PGM(string inpFileName) {
		ifstream inpFile(inpFileName, ios::binary);
		if (!inpFile.is_open()) {
			throw exception("Cant open input File");
		}
		inpFile >> vers[0] >> vers[1] >> width >> height >> colorDepth;
		char read_char[1];
		inpFile.read(read_char, 1);
		pict.assign(height, vector<unsigned char>(width, unsigned char(0)));
		inpFile.close();
	}

	void write(string outFileName) {
		ofstream outFile(outFileName, ios::binary);
		if (!outFile.is_open()) {
			throw exception("Cant open output file");
		}
		outFile << vers[0] << vers[1] << '\n' << width << ' ' << height << '\n' << colorDepth << '\n';

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				outFile << pict[i][j];
			}
		}
		outFile.close();
	}

	void plot(int x, int y, double bright,int color) {

		if (x < 0 || x >= this->width || y < 0 || y >= this->height || bright < 0) {
			return;
		}

		double gamma = 2.0;
		double ColorCorrect = double(color) / colorDepth;
		ColorCorrect = pow(ColorCorrect, gamma);
		ColorCorrect *= bright;
		ColorCorrect = pow(ColorCorrect, 1.0 / gamma);
		pict[x][y] = unsigned char(colorDepth * ColorCorrect);

	}

	double fpart(double x) {
		return x - floor(x);
	}

	double rfpart(double x) {
		return 1 - fpart(x);
	}

	void draw_vertical(double x0, double y0, double x1, double y1, int bright) {
		if (y0 > y1) {
			swap(y0, y1);
		}
		for (int i = y0; i <= y1; i++) {
			plot(x0, i, 1,bright);
		}
	}

	void draw_horizontal(double x0, double y0, double x1, double y1,int bright) {
		if (x0 > x1) {
			swap(x0, x1);
		}
		for (int i = x0; i <= x1; i++) {
			plot(i, y0, 1,bright);
		}
	}

	void wu_algo(double x0, double y0, double x1, double y1,int color) {

		if (y0 == y1) {
			draw_horizontal(x0, y0, x1, y1,color);
			return;
		}
		if (x0 == x1) {
			draw_vertical(x0, y0, x1, y1, color);
			return;
		}
		bool steep = abs(y1 - y0) > abs(x1 - x0);

		if (steep) {
			swap(x0, y0);
			swap(x1, y1);
		}
		if (x0 > x1) {
			swap(x0, x1);
			swap(y0, y1);
		}

		double dx = x1 - x0;
		double dy = y1 - y0;
		double grad;
		if (dx == 0) {
			grad = 1;
		}
		else {
			grad = dy / dx;
		}
		int xend = round(x0);
		int yend = y0 + grad * (xend - x0);
		double xgap = rfpart(x0 + 0.5);
		int xpxl1 = xend;
		int ypxl1 = floor(yend);
		if (steep) {
			plot(ypxl1, xpxl1, rfpart(yend) * xgap,color);
			plot(ypxl1 + 1, xpxl1, fpart(yend) * xgap,color);
		}
		else {
			plot(xpxl1, ypxl1, rfpart(yend) * xgap,color);
			plot(xpxl1, ypxl1 + 1, fpart(yend) * xgap,color);
		}
		double intery = yend + grad;
		xend = round(x1);
		yend = y1 + grad * (xend - x1);
		xgap = fpart(x1 + 0.5);
		int xpxl2 = xend;
		int ypxl2 = floor(yend);
		if (steep) {
			plot(ypxl2, xpxl2, rfpart(yend) * xgap,color);
			plot(ypxl2 + 1, xpxl2, fpart(yend) * xgap,color);
		}
		else {
			plot(xpxl2, ypxl2, rfpart(yend) * xgap,color);
			plot(xpxl2, ypxl2 + 1, fpart(yend) * xgap,color);
		}
		if (steep) {
			for (int x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
				plot(floor(intery), x, rfpart(intery),color);
				plot(floor(intery) + 1, x, fpart(intery),color);
				intery += grad;
			}
		}
		else {
			for (int x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
				plot(x, floor(intery), rfpart(intery),color);
				plot(x, floor(intery) + 1, fpart(intery),color);
				intery += grad;
			}
		}

	}

};