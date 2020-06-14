#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

using namespace std;

class PGM {
private:
	vector<char> vers;
	int width;
	int height;
	int colorDepth;
	vector<vector<double>> pict;

	
	double find_new_color(double a_color, int bit) {

		int color = round(a_color);
		if (bit == 8) {
			return color;
		}
		else if (bit == 7) {

			return (color & 1 << 7) + (color & 1 << 6) + (color & 1 << 5) + (color & 1 << 4) + (color & 1 << 3) + (color & 1 << 2) + (color & 1 << 1) + (color & 1 << 7) / 128;

		}
		else if (bit == 6) {

			double new_color = (color & 1 << 7) + (color & 1 << 6) + (color & 1 << 5) + (color & 1 << 4) + (color & 1 << 3) + (color & 1 << 2) + (color & 1 << 7) / 64 + (color & 1 << 6) / 64;
			return new_color;

		}
		else if (bit == 5) {

			double new_color = (color & 1 << 7) + (color & 1 << 6) + (color & 1 << 5) + (color & 1 << 4) + (color & 1 << 3) + (color & 1 << 7) / 32 + (color & 1 << 6) / 32 + (color & 1 << 5) / 32;
			return new_color;

		}
		else if (bit == 4) {

			double new_color = (color & 1 << 7) + (color & 1 << 6) + (color & 1 << 5) + (color & 1 << 4) + (color & 1 << 7) / 16 + (color & 1 << 6) / 16 + (color & 1 << 5) / 16 + (color & 1 << 4) / 16;
			return new_color;

		}
		else if (bit == 3) {

			double new_color = (color & 1 << 7) + (color & 1 << 6) + (color & 1 << 5) + (color & 1 << 7) / 8 + (color & 1 << 6) / 8 + (color & 1 << 5) / 8 + (color & 1 << 7) / 64 + (color & 1 << 6) / 64;
			return new_color;

		}
		else if (bit == 2) {

			double new_color = (color & 1 << 7) + (color & 1 << 6) + (color & 1 << 7) / 4 + (color & 1 << 6) / 4 + (color & 1 << 7) / 16 + (color & 1 << 6) / 16 + (color & 1 << 7) / 64 + (color & 1 << 6) / 64;
			return new_color;

		}
		else if (bit == 1) {

			double new_color = (color & 1 << 7) + (color & 1 << 7) / 2 + (color & 1 << 7) / 4 + (color & 1 << 7) / 8 + (color & 1 << 7) / 16 + (color & 1 << 7) / 32 + (color & 1 << 7) / 64 + (color & 1 << 7) / 128;
			return new_color;

		}
	}
	double sum_between(double a, double b) {
		if (a + b <= 0.0) {
			return 0.0;
		}
		else if (a + b >= 1.0) {
			return 1.0;
		}
		else {
			return a + b;
		}
	}

	double OrderedDitheringMatrix[8][8] = { {0, 48, 12, 60, 3, 51, 15, 63},
									   {32, 16, 44, 28, 35, 19, 47, 31},
									   {8, 56, 4, 52, 11, 59, 7, 55},
									   {40, 24, 36, 20, 43, 27, 39, 23},
									   {2, 50, 14, 62, 1, 49, 13, 61},
									   {34, 18, 46, 30, 33, 17, 45, 29},
									   {10, 58, 6, 54, 9, 57, 5, 53},
									   {42, 26, 38, 22, 41, 25, 37, 21} };

	int HalftoneMatrix[4][4]= { {7, 13, 11, 4},
							    {12, 16, 14, 8},
								{10, 15, 6, 2},
								{5, 9, 3, 1}};

public:


	PGM(string inpFileName,bool gradient,double gamma) {
		ifstream inpFile(inpFileName, ios::binary);
		if (!inpFile.is_open()) {
			throw exception("Cant open input file");
		}
		char vers1,vers2;
		inpFile >> vers1 >> vers2 >> width >> height >> colorDepth;
		if (vers1 != 'P' || vers2 != '5') {
			throw exception("Wrong format");
		}
		vers.push_back(vers1);
		vers.push_back(vers2);
		char read_char[1];
		double new_pixel;
		inpFile.read(read_char, 1);
		pict.assign(height, vector<double>(width));

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				if (!gradient) {
					if (inpFile.eof()) {
						throw exception("Too few pixels");
					}
					inpFile.read(read_char, 1);
					new_pixel = double(unsigned char(read_char[0]))/colorDepth;
					if (gamma != 0) {
						new_pixel = pow(new_pixel, gamma);
						new_pixel *= colorDepth;
						pict[i][j] = new_pixel;
					}
					else {
						if (new_pixel < 0.04045) {
							new_pixel = new_pixel / 12.92;
							pict[i][j] = new_pixel * colorDepth;
						}
						else {
							new_pixel = pow((new_pixel + 0.055) / 1.055, 2.2);
							new_pixel *= colorDepth;
							pict[i][j] = new_pixel;

						}
					}
				}
				else {
					new_pixel = (j * 1.0/(double(width)-1));
					if (gamma != 0) {
						new_pixel = pow(new_pixel, gamma);
						new_pixel *= colorDepth;
						pict[i][j] = new_pixel;
					}
					else {
						if (new_pixel < 0.04045) {
							new_pixel = new_pixel / 12.92;
							pict[i][j] = new_pixel * colorDepth;
						}
						else {
							new_pixel = pow((new_pixel + 0.055) / 1.055, 2.2);
							new_pixel *= colorDepth;
							pict[i][j] = new_pixel;

						}
					}
				}
			}
		}
		inpFile.close();
	}

	void write(string outFileName,double gamma,int bit) {
		ofstream outFile(outFileName, ios::binary);
		if (!outFile.is_open()) {
			throw exception("Cant open output file");
		}
		int NewcolorDepth = int(pow(2, bit) - 1);
		outFile << vers[0] << vers[1] << '\n' << width << ' ' << height << '\n' << colorDepth << '\n';

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (gamma != 0) {
					outFile << unsigned char(int(round(pow(pict[i][j] / colorDepth, 1.0 / gamma) * colorDepth)));
				}
				else {
					if (pict[i][j] / colorDepth <= 0.0031308) {
						outFile << unsigned char(round(pict[i][j] * 12.92*colorDepth));
					}
					else {
						unsigned char kavo = unsigned char(round(((pow(pict[i][j]/colorDepth, 1.0 / 2.2) * 1.055) - 0.055) * colorDepth));
						outFile << kavo;
					}
				}
			}
		}
		outFile.close();
	}
	
	void no_dithering(int bit) {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				try {
					pict[i][j] = find_new_color(pict[i][j], bit);
				}
				catch (exception& ex) {
					throw ex;
				}
			}
		}

	}

	void ordered_dithering(int bit) {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double a = (OrderedDitheringMatrix[i % 8][j % 8] / 64) - 0.5;
				pict[i][j] = find_new_color(sum_between(pict[i][j] / colorDepth, a)*colorDepth, bit);
			}
		}

	}

	void random_dithering(int bit) {

		srand(time(NULL));

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pict[i][j] = find_new_color(sum_between(pict[i][j]/colorDepth,rand()*1.0/(RAND_MAX-1)-0.5) * colorDepth, bit);
			}
		}

	}

	void floyd_dithering(int bit) {

		vector<vector<double>> errors(height, vector<double>(width, 0));
		double error;
		double dErr;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				
				pict[i][j] = sum_between(pict[i][j] / colorDepth, errors[i][j]/colorDepth)*colorDepth;
				dErr = find_new_color(pict[i][j], bit);
				error = (pict[i][j] - dErr) / 16.0;
				pict[i][j] = dErr;
				if (j +1< width) {
					errors[i][j + 1] += (error * 7);
				}
				if (i + 1<height) {
					errors[i + 1][j] += (error * 5);
					if (j - 1 >= 0) {
						errors[i + 1][j - 1] += (error * 3);
					}
					if (j + 1 < width) {
						errors[i + 1][j + 1] += (error);
					}
				}
			}
		}

	}

	void Jarvis_dithering(int bit) {

		vector<vector<double>> errors(height, vector<double>(width, 0));
		double error;
		double dErr;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				pict[i][j] = sum_between(pict[i][j] * 1.0 / colorDepth, errors[i][j]/colorDepth) * colorDepth;
				dErr = find_new_color(pict[i][j], bit);
				error = (pict[i][j] - dErr) / 48.0;
				pict[i][j] = dErr;
				if (j + 1 < width) errors[i][j + 1] += error * 7;
				if (j + 2 < width) errors[i][j + 2] += error * 5;
				if (i + 1 < height)
				{
					if (j - 2 >= 0) errors[i + 1][j - 2] += error * 3;
					if (j - 1 >= 0) errors[i + 1][j - 1] += error * 5;
					errors[i + 1][j] += error * 7;
					if (j + 1 < width) errors[i + 1][j + 1] += (error * 5);
					if (j + 2 < width) errors[i + 1][j + 2] += (error * 3);
				}
				if (i + 2 < height)
				{
					if (j - 2 >= 0) errors[i + 2][j - 2] += (error * 1);
					if (j - 1 >= 0) errors[i + 2][j - 1] += (error * 3);
					errors[i + 2][j] += (error * 5);
					if (j + 1 < width) errors[i + 2][j + 1] += (error * 3);
					if (j + 2 < width) errors[i + 2][j + 2] += (error * 1);
				}
			}
		}

	}

	void Sierra3_dithering(int bit) {

		vector<vector<double>> errors(height, vector<double>(width, 0));
		double error;
		double dErr;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				pict[i][j] = sum_between(pict[i][j] * 1.0 / colorDepth, errors[i][j]/colorDepth) * colorDepth;
				dErr = find_new_color(pict[i][j], bit);
				error = (pict[i][j] - dErr) / 32.0;
				pict[i][j] = dErr;
				if (j + 1 < width) errors[i][j + 1] += (error * 5);
				if (j + 2 < width) errors[i][j + 2] += (error * 3);
				if (i + 1 < height)
				{
					if (j - 2 >= 0) errors[i + 1][j - 2] += (error * 2);
					if (j - 1 >= 0) errors[i + 1][j - 1] += (error * 4);
					errors[i + 1][j] += (error * 5);
					if (j + 1 < width) errors[i + 1][j + 1] += (error * 4);
					if (j + 2 < width) errors[i + 1][j + 2] += (error * 2);
				}
				if (i + 2 < height)
				{
					if (j - 1 >= 0) errors[i + 2][j - 1] += (error * 2);
					errors[i + 2][j] += (error * 3);
					if (j + 1 < width) errors[i + 2][j + 1] += (error * 2);
				}
			}
		}
	}

	void Atkinson_dithering(int bit) {

		vector<vector<double>> errors(height, vector<double>(width, 0));
		double error;
		double dErr;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				pict[i][j] = sum_between(pict[i][j] * 1.0 / colorDepth, errors[i][j]/colorDepth) * colorDepth;
				dErr = find_new_color(pict[i][j], bit);
				error = (pict[i][j] - dErr) / 8.0;
				pict[i][j] = dErr;
				if (j + 1 < width) errors[i][j + 1] += error;
				if (j + 2 < width) errors[i][j + 2] += error;
				if (i + 1 < height)
				{
					if (j - 1 >= 0) errors[i + 1][j - 1] += error;
					errors[i + 1][j] += error;
					if (j + 1 < width) errors[i + 1][j + 1] += error;
				}
				if (i + 2 < height)
				{
					errors[i + 2][j] += error;
				}
			}
		}

	}

	void Halftone(int bit) {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				double a = HalftoneMatrix[i % 4][j % 4] / 17.0 - 0.5;
				pict[i][j] = find_new_color(sum_between(pict[i][j]/colorDepth, a)*colorDepth,bit);
			}
		}
	}
};