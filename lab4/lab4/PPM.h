#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <string>
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

	PPM(int count, string inpFileName) {
		if (count == 1) {
			ifstream inpFile(inpFileName, ios::binary);
			if (!inpFile.is_open())
				throw runtime_error("Cant open File");

			inpFile >> vers[0] >> vers[1];
			if (vers[0] != 'P' || vers[1] != '6')
				throw runtime_error("Wrong Format");
			inpFile >> width >> height >> colorDepth;
			pict.assign(height, vector<Pixel>(width));
			char p;
			inpFile.read(&p, 1);
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					Pixel cur;
					inpFile.read(&p, sizeof(unsigned char));
					cur.r = p;
					inpFile.read(&p, sizeof(unsigned char));
					cur.g = p;
					inpFile.read(&p, sizeof(unsigned char));
					cur.b = p;
					pict[i][j] = cur;
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
				throw runtime_error("Cant Open File");
			}
			string beforeDot = inpFileName.substr(0, dotNumb);
			string AfterDot = inpFileName.substr(dotNumb);
			vector<string> files;
			files.resize(3);
			files[0] = beforeDot + "_1"+AfterDot;
			files[1] = beforeDot + "_2" + AfterDot;
			files[2] = beforeDot + "_3" + AfterDot;
			for (int k = 0; k < 3; k++)
			{
				ifstream inpFile(files[k], ios::binary);
				if (!inpFile.is_open()) {
					throw runtime_error("Cant Open File ");
				}

				inpFile >> vers[0] >> vers[1];
				if (vers[0] != 'P' || vers[1] != '5') {
					throw runtime_error("Wrong Format");
				}
				inpFile >> width >> height >> colorDepth;
				if (pict.size() == 0)
					pict.assign(height, vector<Pixel>(width));
				char p;
				inpFile.read(&p, 1);
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
					{
						inpFile.read(&p, sizeof(unsigned char));
						if (k == 0) {
							pict[i][j].r = p;
						}
						else if  (k == 1) {
							pict[i][j].g = p;
						}
						else {
							pict[i][j].b = p;
						}
					}
				inpFile.close();
			}
		}
	}

	void write(int count, string outFileName) {
		if (count == 1) {
			ofstream outFile(outFileName, ios::binary);
			if (!outFile.is_open()) {
				throw runtime_error("cannot open output file");
			}
			outFile << "P6\n" << width << ' ' << height << '\n' << colorDepth << '\n';
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					outFile << pict[i][j].r;
					outFile << pict[i][j].g;
					outFile << pict[i][j].b;
				}
			outFile.close();
		}
		else {

			int dotNumb = -1;
			for (int i = outFileName.size()-1; i >= 0; i--) {
				if (outFileName[i] == '.') {
					dotNumb = i;
					break;
				}
			}
			if (dotNumb == -1)
				throw runtime_error("Cant open File");
			string beforeDot = outFileName.substr(0, dotNumb);
			string AfterDot = outFileName.substr(dotNumb);
			vector<string> files;
			files.resize(3);
			files[0] = beforeDot + "_1" + AfterDot;
			files[1] = beforeDot + "_2" + AfterDot;
			files[2] = beforeDot + "_3" + AfterDot;
			for (int k = 0; k < 3; k++)
			{
				ofstream outFile(files[k], ios::binary);
				if (!outFile.is_open()) {
					throw runtime_error("Cant open output file ");
				}
				outFile << "P5\n" << width << ' ' << height << '\n' << colorDepth << '\n';
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
					{
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

				double R = pict[i][j].r * 1.0 / double(colorDepth);
				double G = pict[i][j].g * 1.0 / double(colorDepth);
				double B = pict[i][j].b * 1.0 / double(colorDepth);

				double maxi = max(R, max(G, B));
				double mini = min(R, min(G, B));
				double delta = maxi - mini;

				double H, S, V;
				if (delta == 0) {
					H = 0;
				}
				else if (R == maxi) {
					H = 60 * ((G - B) / delta);
				}
				else if (G == maxi) {
					H = 60 * (((B - R) / delta) + 2);
				}
				else {
					H = 60 * (((R - G) / delta) + 4);
				}

				if (maxi == 0) {
					S = 0;
				}
				else {
					S = delta / maxi;
				}

				V = maxi;
				if (H < 0) {
					H += 360;
				}
				pict[i][j] = { unsigned char(round(H * double(colorDepth) / 360.0)),unsigned char(rround(round(S * double(colorDepth)))),unsigned char(rround(round(V * double(colorDepth)))) };
			}
		}
	}

	void From_HSV_to_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				double H = pict[i][j].r / 255.0 * 360.0;
				double S = pict[i][j].g / 255.0;
				double V = pict[i][j].b / 255.0;
				double C = V * S;
				double Hi = H / 60;
				double X = C * (1 - abs(((int)Hi) % 2 + (Hi - (int)Hi) - 1));
				double m = V - C;
				double R, G, B;
				if (H >= 0 && H <= 60)
					R = C, G = X, B = 0;
				else if (H >= 60 && H <= 120)
					R = X, G = C, B = 0;
				else if (H >= 120 && H <= 180)
					R = 0, G = C, B = X;
				else if (H >= 180 && H <= 240)
					R = 0, G = X, B = C;
				else if (H >= 240 && H <= 300)
					R = X, G = 0, B = C;
				else
					R = C, G = 0, B = X;

				int Rr = (int)(round((R + m) * 255));
				int Gg = (int)(round((G + m) * 255));
				int Bb = (int)(round((B + m) * 255));

				if (Rr < 0) Rr = 0;
				if (Rr > 255) Rr = 255;
				if (Bb < 0) Bb = 0;
				if (Bb > 255) Bb = 255;
				if (Gg < 0) Gg = 0;
				if (Gg > 255) Gg = 255;
				pict[i][j]={ (unsigned char)Rr, (unsigned char)Gg, (unsigned char)Bb };
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
				double G = Y - (Cb*(Kb * (2 - 2 * Kb) / Kg)) - (Cr*(Kr * (2-2 * Kr) / Kg));
				double B = Y + Cb * (2 - 2 * Kb);

				pict[i][j] = { unsigned char(rround(int(round(R * colorDepth)))),unsigned char(rround(int(round(G * colorDepth)))),unsigned char(rround(int(round(B * colorDepth)))) };
			}
		}
	}

	void From_RGB_to_YCbCr709(){
	
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				double Kr = 0.2126;
				double Kg = 0.7152;
				double Kb = 0.0722;

				double R = double(pict[i][j].r) / double(colorDepth);
				double G = double(pict[i][j].g) / double(colorDepth);
				double B = double(pict[i][j].b) / double(colorDepth);

				//cout << Kb << " " << Kr << " " << Kg << " "<<R<<" "<<G<<" "<<B<<"\n";

				double Y = Kr * R + Kg * G + Kb * B;
				double Cb = (B - Y) / (2.0 * (1.0 - Kb));
				double Cr = (R - Y) / (2.0 * (1.0 - Kr));

				pict[i][j] = { unsigned char(rround(int(round(Y * colorDepth)))),unsigned char(rround(int(round((Cb + 0.5) * colorDepth)))),unsigned char(rround(int(round((Cr + 0.5) * colorDepth)))) };
			}
		}
	}

	void From_YCbCr709_to_RGB() {

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				double Kb = 0.0722;
				double Kr = 0.2126;
				double Kg = 0.7152;

				double Y = double(pict[i][j].r) / double(colorDepth);
				double Cb = (double(pict[i][j].g )/ double(colorDepth)) - 0.5;
				double Cr = (double(pict[i][j].b) / double(colorDepth)) - 0.5;

				double R = Y + Cr * (2.0 - 2.0* Kr);
				double G = Y - (Kb * (2.0 - 2.0 * Kb) / Kg)*Cb - Cr*(Kr * (2.0-2.0 * Kr) / Kg);
				double B = Y + Cb * (2.0 - 2.0 * Kb);

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

	void ColorSpace_changes(string From,string To) {

		if (From != "RGB") {
			if (From == "HSL") {
				From_HSL_to_RGB();
			}
			else if (From == "HSV") {
				From_HSV_to_RGB();
			}
			else if (From == "YCbCr.601") {
				From_YCbCr601_to_RGB();
			}
			else if (From == "YCbCr.709") {
				From_YCbCr709_to_RGB();
			}
			else if (From == "YCoCg") {
				From_YCoCg_to_RGB();
			}
			else if (From == "CMY") {
				From_CMY_to_RGB();
			}
			else {
				throw exception("There is no such Color Space");
			}
		}
		if (To != "RGB") {
			if (To == "HSL") {
				From_RGB_to_HSL();
			}
			else if (To == "HSV") {
				From_RGB_to_HSV();
			}
			else if (To == "YCbCr.601") {
				From_RGB_to_YCbCr601();
			}
			else if (To == "YCbCr.709") {
				From_RGB_to_YCbCr709();
			}
			else if (To == "YCoCg") {
				From_RGB_to_YCoCg();
			}
			else if (To == "CMY") {
				From_RGB_to_CMY();
			}
			else {
				throw exception("There is no such Color Space");
			}
		}

	}
};