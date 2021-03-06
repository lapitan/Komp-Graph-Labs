#pragma once
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

struct Pixel
{
	unsigned char r, g, b;
};

class PPM:public PNM {
private:
	char vers[2];
	int width;
	int height;
	int colorDepth;
	vector<vector<Pixel>> pict;
public:
	PPM(string inpFileName) {
		ifstream inpFile(inpFileName, ios::binary);
		inpFile >> vers[0] >> vers[1] >> width >> height >> colorDepth;
		pict.assign(height, vector<Pixel>(width));
		char pixel_read[3];
		char read_char[1];
		inpFile.read(read_char, 1);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (inpFile.eof()) {
					inpFile.close();
					cout << i << " " << j << "\n";
					throw exception("Too few pixels");
				}
				inpFile.read(pixel_read, 3);
				pict[i][j].r = pixel_read[0];
				pict[i][j].g = pixel_read[1];
				pict[i][j].b = pixel_read[2];
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
				outFile << pict[i][j].r<<pict[i][j].g<<pict[i][j].b;
			}
		}
		outFile.close();
	}

	void ColorInversion() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pict[i][j].r = colorDepth - pict[i][j].r;
				pict[i][j].g = colorDepth - pict[i][j].g;
				pict[i][j].b = colorDepth - pict[i][j].b;
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
		vector<vector<Pixel>> new_pict(width, vector<Pixel>(height));
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_pict[j][height - i - 1] = pict[i][j];
			}
		}
		swap(width, height);
		pict = new_pict;
	}

	void RotateLeft() {

		vector<vector<Pixel>> new_pict(width, vector<Pixel>(height));
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_pict[width - j - 1][i] = pict[i][j];
			}
		}
		swap(width, height);
		pict = new_pict;

	}
};