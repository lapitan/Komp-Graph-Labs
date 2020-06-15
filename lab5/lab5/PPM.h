#pragma once
#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <set>
#include <iterator>

using namespace std;

struct Pixel
{
	unsigned char r, g, b;
};

class PPM {
private:
	char vers[2];
	int width;
	int height;
	int colorDepth;
	vector<vector<Pixel>> pict;

	int rround(int a) {
		if (a > colorDepth) {
			return colorDepth;
		}
		else if (a < 0) {
			return 0;
		}
		else {
			return a;
		}
	}

public:
	PPM(string inpFileName) {
			ifstream inpFile(inpFileName, ios::binary);
			if (!inpFile.is_open()) {
				throw exception("Cant open File");
			}
			inpFile >> vers[0] >> vers[1] >> width >> height >> colorDepth;
			if (vers[0] != 'P' || (vers[1] != '6'&&vers[1]!='5')) {
				throw exception("Wrong Fromat");
			}
			pict.assign(height, vector<Pixel>(width));
			char pixel_read[3];
			char read_char[1];
			inpFile.read(read_char, 1);

			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if (vers[1] == '6'){
						inpFile.read(pixel_read, 3);
						pict[i][j].r = pixel_read[0];
						pict[i][j].g = pixel_read[1];
						pict[i][j].b = pixel_read[2];
					}
					else {
						inpFile.read(pixel_read, 1);
						pict[i][j].r = pixel_read[0];
						pict[i][j].g = pixel_read[0];
						pict[i][j].b = pixel_read[0];
					}
				}
			}
			inpFile.close();
	}

	void write(string outFileName) {

			ofstream outFile(outFileName, ios::binary);
			if (!outFile.is_open()) {
				throw exception("Cant open output file");
			}
			outFile << vers[0]<<vers[1] << '\n' << width << ' ' << height << '\n' << colorDepth << '\n';

			if (vers[1] == '6') {
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						outFile << pict[i][j].r << pict[i][j].g << pict[i][j].b;
					}
				}
				outFile.close();
			}
			else {
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						outFile << pict[i][j].r;
					}
				}
				outFile.close();
			}
	}

	void addandmult_RGB(int i, int j, int add, double mult) {
		pict[i][j].r = unsigned char(rround((pict[i][j].r - add) * mult));
		pict[i][j].g = unsigned char(rround((pict[i][j].g - add) * mult));
		pict[i][j].b = unsigned char(rround((pict[i][j].b - add) * mult));
	}

	void addandmult_YCC(int i, int j, int add, double mult) {
		pict[i][j].r = unsigned char(rround((pict[i][j].r - add) * mult));
	}

	int rround(double a) {
		
		if (a < 0) {
			return 0;
		}
		else if (a > 255) {
			return 255;
		}
		else {
			return int(a);
		}

	}

	void act0(int add, double mult) {
		
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				addandmult_RGB(i, j, add, mult);
			}
		}

	}

	void act1(int add, double mult) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				addandmult_YCC(i, j, add, mult);
			}
		}
		//From_YCbCr601_to_RGB();
	}

	void act2() {

		int maxi=-1;
		int mini=1000;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (pict[i][j].r > maxi) {
					maxi = pict[i][j].r;
				}
				if (pict[i][j].g > maxi) {
					maxi = pict[i][j].g;
				}
				if (pict[i][j].b > maxi) {
					maxi = pict[i][j].b;
				}
				if (pict[i][j].r < mini) {
					mini = pict[i][j].r;
				}
				if (pict[i][j].g < mini) {
					mini = pict[i][j].g;
				}
				if (pict[i][j].b < mini) {
					mini = pict[i][j].b;
				}
			}
		}
		if (maxi == mini) {
			return;
		}
		int add = mini;
		double mult = 255.0 / (maxi - mini);
		cout << add << " " << mult << "\n";
		act0(add, mult);

	}

	void act3() {

		int maxi = -1;
		int mini = 1000;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (pict[i][j].r > maxi) {
					maxi = pict[i][j].r;
				}
				if (pict[i][j].r < mini) {
					mini = pict[i][j].r;
				}
			}
		}
		if (maxi == mini) {
			return;
		}
		int add = mini;
		double mult = 255.0 / (maxi - mini);
		cout << add << " " << mult << "\n";
		act1(add, mult);

	}

	void act4() {

		int all_pixels = width * height;
		int needed_max = all_pixels * 0.01 * 0.39;
		vector<int> pixels;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pixels.push_back(pict[i][j].r);
				pixels.push_back(pict[i][j].g);
				pixels.push_back(pict[i][j].b);
			}
		}
		/*auto it1 = pixels.begin();
		auto it2 = pixels.end();
		it2--;
		for (int i = 0; i < needed_max; i++) {
			it1++;
			it2--;
		}*/
		sort(pixels.begin(), pixels.end());
		int maxi = pixels[all_pixels*3- needed_max];
		int mini = pixels[needed_max];
		if (maxi == mini) {
			return;
		}
		int add = mini;
		double mult = 255.0 / (maxi - mini);
		cout << add << " " << mult<<"\n";
		act0(add, mult);
	}

	void act5() {

		int all_pixels = width * height;
		int needed_max = all_pixels * 0.01 * 0.39;
		set<int> pixels;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pixels.insert(pict[i][j].r);
			}
		}
		auto it1 = pixels.begin();
		auto it2 = pixels.end();
		it2--;
		for (int i = 0; i < needed_max; i++) {
			it1++;
			it2--;
		}

		int maxi = (*it2);
		int mini = (*it1);
		if (maxi == mini) {
			return;
		}
		int add = mini;
		double mult = 255.0 / (maxi - mini);
		cout << add << " " << mult << "\n";
		act1(add, mult);
	}

	void From_RGB_to_YCbCr601() {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double Kr = 0.299;
				double Kg = 0.576;
				double Kb = 0.114;

				double R = pict[i][j].r / double(colorDepth);
				double G = pict[i][j].g / double(colorDepth);
				double B = pict[i][j].b / double(colorDepth);

				double Y = Kr * R + Kg * G + Kb * B;
				double Cb = (B / 2) - (Kr * R / (2 * (1 - Kb))) - (Kg * G / (2 * (1 - Kb)));
				double Cr = (R / 2) - (G * Kg / (2 * (1 - Kr))) - (B * Kb / (2 * (1 - Kr)));

				pict[i][j] = { unsigned char(rround(int(round(Y * colorDepth)))),unsigned char(rround(int(round((Cb + 0.5) * colorDepth)))),unsigned char(rround(int(round((Cr + 0.5) * colorDepth)))) };
			}
		}
	}

	void From_YCbCr601_to_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double Kr = 0.299;
				double Kg = 0.576;
				double Kb = 0.114;

				double Y = pict[i][j].r / double(colorDepth);
				double Cb = (pict[i][j].g / double(colorDepth)) - 0.5;
				double Cr = (pict[i][j].b / double(colorDepth)) - 0.5;

				double R = Y + Cr * (2 - 2 * Kr);
				double G = Y - (Kb * (2 - 2 * Kb) / Kg) - (Kr * (202 * Kr) / Kg);
				double B = Y - Cb * (2 - 2 * Kb);

				pict[i][j] = { unsigned char(rround(int(round(R * colorDepth)))),unsigned char(rround(int(round(G * colorDepth)))),unsigned char(rround(int(round(B * colorDepth)))) };
			}
		}
	}
};