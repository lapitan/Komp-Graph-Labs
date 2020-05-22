#pragma once
#include "PNM.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class PGM:public PNM{
private:
	char vers[2];
	int width;
	int height;
	int colorDepth;
	vector<vector<unsigned char>> pict;
public:
	PGM(string inpFileName) {
		ifstream inpFile(inpFileName,ios::binary);
		inpFile >> vers[0] >> vers[1]>>width>>height>>colorDepth;
		pict.assign(height, vector<unsigned char>(width));

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (inpFile.eof()) {
					throw exception("Too few pixels");
				}
				inpFile >> pict[i][j];
			}
		}
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

	void ColorInversion() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pict[i][j] = colorDepth - pict[i][j];
			}
		}
	}

	void HorizontalReflect() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width / 2; j++) {
				swap(pict[i][j], pict[i][width - j - 1]);
			}
		}
	}

	void VerticalReflect() {
		for (int i = 0; i < height / 2; i++) {
			for (int j = 0; j < width; j++) {
				swap(pict[i][j], pict[height - i - 1][j]);
			}
		}
	}

	void RotateRight() {
		vector<vector<unsigned char>> new_pict(width, vector<unsigned char>(height));
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_pict[j][height - i - 1] = pict[i][j];
			}
		}
		swap(width, height);
		pict = new_pict;
	}

	void RotateLeft() {

		vector<vector<unsigned char>> new_pict(width, vector<unsigned char>(height));
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_pict[width-j-1][i] = pict[i][j];
			}
		}
		swap(width, height);
		pict = new_pict;

	}
};