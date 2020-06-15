#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
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
	PPM(int ccount,string inpFileName) {
		if (ccount == 1) {
			ifstream inpFile(inpFileName, ios::binary);
			if (!inpFile.is_open()) {
				throw exception("Cant open File");
			}
			inpFile >> vers[0] >> vers[1] >> width >> height >> colorDepth;
			if (vers[0] != 'P' || vers[1] != '6') {
				throw exception("Wrong Fromat");
			}
			pict.assign(height, vector<Pixel>(width));
			char pixel_read[3];
			char read_char[1];
			inpFile.read(read_char, 1);

			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					inpFile.read(pixel_read, 3);
					pict[i][j].r = pixel_read[0];
					pict[i][j].g = pixel_read[1];
					pict[i][j].b = pixel_read[2];
				}
			}
			inpFile.close();
		}
		else {
			
			int dotNumb = -1;
			for (int i = inpFileName.size() - 1; i >= 0; i--) {
				if (inpFileName[i] == '.') {
					dotNumb = i;
					break;
				}
			}
			if (dotNumb == -1) {
				throw exception("Cant open File");
			}

			string BeforeDot = inpFileName.substr(0, dotNumb);
			string AfterDot = inpFileName.substr(dotNumb);
			vector<string> inpFileNames;
			inpFileNames[0] = BeforeDot + "_1" + AfterDot;
			inpFileNames[1] = BeforeDot + "_2" + AfterDot;
			inpFileNames[2] = BeforeDot + "_3" + AfterDot;

			for (int k = 0; k < 3; k++) {

				ifstream inpFile(inpFileNames[k], ios::binary);
				if (!inpFile.is_open()) {
					throw exception("Cant open File");
				}
				inpFile >> vers[0] >> vers[1] >> width >> height >> colorDepth;
				if (vers[0] != 'P' || vers[1] != '5') {
					throw exception("Wrong Fromat");
				}
				pict.assign(height, vector<Pixel>(width));
				char pixel_read[1];
				char read_char[1];
				inpFile.read(read_char, 1);

				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						
						inpFile.read(pixel_read, 1);
						if (k == 0) {
							pict[i][j].r = pixel_read[0];
						}
						else if (k == 1) {
							pict[i][j].g = pixel_read[0];
						}
						else {
							pict[i][j].b = pixel_read[0];
						}
						
					}
				}
				inpFile.close();

			}

		}
	}

	void write(int count,string outFileName) {

		if (count == 1) {
			ofstream outFile(outFileName, ios::binary);
			if (!outFile.is_open()) {
				throw exception("Cant open output file");
			}
			outFile << "P6" << '\n' << width << ' ' << height << '\n' << colorDepth << '\n';

			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					outFile << pict[i][j].r << pict[i][j].g << pict[i][j].b;
				}
			}
			outFile.close();
		}
		else {
			int dotNumb = -1;
			for (int i = outFileName.size() - 1; i >= 0; i--) {
				if (outFileName[i] == '.') {
					dotNumb = i;
					break;
				}
			}
			if (dotNumb == -1) {
				throw exception("Cant Open File");
			}

			string BeforeDot = outFileName.substr(0, dotNumb);
			string AfterDot = outFileName.substr(dotNumb);
			vector<string> outFileNames;
			outFileNames.resize(3);
			outFileNames[0] = BeforeDot + "_1" + AfterDot;
			outFileNames[1] = BeforeDot + "_2" + AfterDot;
			outFileNames[2] = BeforeDot + "_3" + AfterDot;

			for (int k = 0; k < 3; k++) {
				ofstream outFile(outFileNames[k], ios::binary);
				if (!outFile.is_open()) {
					throw exception("Cant open output file");
				}
				outFile << "P5"  << '\n' << width << ' ' << height << '\n' << colorDepth << '\n';

				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						if (k == 0) {
							outFile << pict[i][j].r;
						}
						else if (k == 1) {
							outFile << pict[i][j].g;
						}
						else {
							outFile << pict[i][j].b;
						}
					}
				}
				outFile.close();
			}

		}
	}

	void From_RGB_to_HSL() {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Pixel curr = pict[i][j];
				double r = curr.r * 1.0 / double(colorDepth);
				double g = curr.g * 1.0 / double(colorDepth);
				double b = curr.b * 1.0 / double(colorDepth);

				double maxi = max(r, max(g, b));
				double mini = min(r, min(g, b));
				double H;
				double L = (maxi + mini) / 2;
				double S = (maxi - mini) / (1 - abs(1 - maxi - mini));
				if (maxi == mini) {
					H = 0;
				}
				else if (r == maxi && g >= b) {
					H = ((g - b) / (maxi - mini)) * 60;
				}
				else if (r == maxi && g < b) {
					H = (60 * ((b - r) / (maxi - mini))) + 360;
				}
				else if (maxi == g) {
					H = (60 * (b - r) / (maxi - mini)) + 120;
				}
				else {
					H = (60 * ((r - g) / (maxi - mini))) + 240;
				}
				pict[i][j] = { unsigned char(round(H * colorDepth / 360)),unsigned char(round(S * colorDepth)),unsigned char(round(L * colorDepth)) };
			}
		}
	}

	void From_HSL_to_RGB() {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double H = pict[i][j].r / double(colorDepth);
				double S = pict[i][j].g / double(colorDepth);
				double L = pict[i][j].b / double(colorDepth);
				double Q, P;
				if (L < 0.5) {
					Q = L * (1.0 + S);
				}
				else {
					Q = L + S - (L * S);
				}
				P = 2 * L - Q;
				double Tr = H + (1.0 / 3);
				double Tg = H;
				double Tb = H - (1.0 / 3);
				if (Tr < 0)Tr += 1.0;
				if (Tr > 1.0)Tr -= 1.0;
				if (Tg < 0)Tg += 1.0;
				if (Tg > 1.0)Tg -= 1.0;
				if (Tb < 0)Tb += 1.0;
				if (Tb > 1.0)Tb -= 1.0;
				double R, G, B;

				if (Tr < 1.0 / 6) {
					R = P + ((Q - P) * 6 * Tr);
				}
				else if (Tr >= 1.0 / 6 && Tr < 1.0 / 2) {
					R = Q;
				}
				else if (Tr >= 1.0 / 2 && Tr < 2.0/ 3) {
					R = P + ((Q - P) * ((2.0/ 3) - Tr) * 6);
				}
				else {
					R = P;
				}

				if (Tg < 1.0 / 6) {
					G = P + ((Q - P) * 6 * Tg);
				}
				else if (Tg >= 1.0 / 6 && Tg < 1.0 / 2) {
					G = Q;
				}
				else if (Tg >= 1.0 / 2 && Tg < 2.0/ 3) {
					G = P + ((Q - P) * ((2.0/ 3) - Tg) * 6);
				}
				else {
					G = P;
				}

				if (Tb < 1.0 / 6) {
					B = P + ((Q - P) * 6 * Tb);
				}
				else if (Tb >= 1.0 / 6 && Tb < 1.0 / 2) {
					B = Q;
				}
				else if (Tb >= 1.0 / 2 && Tb < 2.0/ 3) {
					B = P + ((Q - P) * ((2.0/ 3) - Tb) * 6);
				}
				else {
					B = P;
				}
				R = R * colorDepth;
				G = G * colorDepth;
				B = B * colorDepth;
				pict[i][j] = { unsigned char(rround(round(R))), unsigned char(rround(round(G))),unsigned char(rround(round(B))) };
			}
		}
	}

	void From_RGB_to_HSV() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Pixel curr = pict[i][j];
				double R = curr.r * 1.0 / double(colorDepth);
				double G = curr.g * 1.0 / double(colorDepth);
				double B = curr.b * 1.0 / double(colorDepth);

				double maxi = max(R, max(G, B));
				double mini = min(R, min(G, B));

				double H, S, V;
				if (R == maxi && G >= B) {
					H = 60 * (G - B) / (maxi - mini);
				}
				else if (R == maxi && G < B) {
					H = (60 * (G - B) / (maxi - mini)) + 360;
				}
				else if (maxi == G) {
					H = (60 * (B - R) / (maxi - mini)) + 120;
				}
				else if (maxi == B) {
					H = (60 * (R - G) / (maxi - mini)) + 240;
				}
				if (maxi == 0) {
					S = 0;
				}
				else {
					S = 1 - (mini / maxi);
				}
				V = maxi;

				pict[i][j] = { unsigned char(round(H * colorDepth / 360)),unsigned char(round(S * colorDepth)),unsigned char(round(V * colorDepth)) };
			}
		}
	}

	void From_HSV_to_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double H = pict[i][j].r * 360.0 / double(colorDepth);
				double S = pict[i][j].g * 1.0 / double(colorDepth);
				double V = pict[i][j].b * 1.0 / double(colorDepth);

				int Hi = floor(H / 60);
				Hi = Hi % 6;
				double Vmin = (100 - S) * V / 100;
				double a = (V - Vmin) * (int(H) % 60) / 60;
				double Vinc = Vmin + a;
				double Vdec = V - a;
				double R, G, B;
				if (Hi == 0) {
					R = V;
					G = Vinc;
					B = Vmin;
				}
				else if (Hi == 1) {
					R = Vdec;
					G = V;
					B = Vmin;
				}
				else if (Hi == 2) {
					R = Vmin;
					G = V;
					B = Vinc;
				}
				else if (Hi == 3) {
					R = Vmin;
					G = Vdec;
					B = V;
				}
				else if (Hi == 4) {
					R = Vinc;
					G = Vmin;
					B = V;
				}
				else {
					R = V;
					G = Vmin;
					B = Vdec;
				}

				pict[i][j] = { unsigned char(round(R * colorDepth / 100)),unsigned char(round(G * colorDepth / 100)),unsigned char(round(B * colorDepth / 100)) };
			}
		}

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

	void From_RGB_to_YCbCr709(){
	
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double Kr = 0.0722;
				double Kg = 0.2126;
				double Kb = 0.7152;

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

	void From_YCbCr709_to_RGB() {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				double Kr = 0.0722;
				double Kg = 0.2126;
				double Kb = 0.7152;

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

	void From_RGB_to_YCoCg() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double R = pict[i][j].r * 1.0;
				double G = pict[i][j].g * 1.0;
				double B = pict[i][j].b * 1.0;

				double Y = (R / 4) + (G / 2) + (B / 4);
				double Co = (R / 2) - (B / 2) + 0.5 * colorDepth;
				double Cg = (G / 2) - (R / 4) - (B / 4) + 0.5 * colorDepth;

				pict[i][j] = { unsigned char(rround(int(Y))),unsigned char(rround(int(Co))),unsigned char(rround(int(Cg))) };
			}
		}
	}

	void From_YCoCg_to_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double Y = pict[i][j].r;
				double Co = pict[i][j].g * 1.0 - 0.5 * colorDepth;
				double Cg = pict[i][j].b * 1.0 - 0.5 * colorDepth;

				double R = Y + Co - Cg;
				double G = Y + Cg;
				double B = Y - Co - Cg;

				pict[i][j] = { unsigned char(rround(int(R))),unsigned char(rround(int(G))),unsigned char(rround(int(B))) };
			}
		}

	}

	void From_RGB_to_CMY() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double R = pict[i][j].r * 1.0;
				double G = pict[i][j].g * 1.0;
				double B = pict[i][j].b * 1.0;
				pict[i][j] = { unsigned char(colorDepth - R),unsigned char(colorDepth - G),unsigned char(colorDepth - B) };
			}
		}
	}

	void From_CMY_to_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double C = pict[i][j].r * 1.0;
				double M = pict[i][j].g * 1.0;
				double Y = pict[i][j].b * 1.0;
				pict[i][j] = { unsigned char(colorDepth - C),unsigned char(colorDepth - M),unsigned char(colorDepth - Y) };
			}
		}
	}
};