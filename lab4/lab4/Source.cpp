#include "PPM.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

	if (argc != 11) {
		
		cerr << "Too few arguments";
		return 1;

	}


	string inpFileName;
	string outFileName;
	int count1;
	int count2;
	string From;
	string To;
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "-f" && i < argc - 1) {
			From = string(argv[i + 1]);
		}
		if (string(argv[i]) == "-t" && i < argc - 1) {
			To = string(argv[i + 1]);
		}
		if (string(argv[i]) == "-i" && i < argc - 2) {
			count1 = atoi(argv[i + 1]);
			inpFileName = string(argv[i + 2]);
		}
		if (string(argv[i]) == "-o" && i < argc - 2) {
			count2 = atoi(argv[i + 1]);
			outFileName = string(argv[i + 2]);
		}
	}


	PPM* pict;
	try {
		pict = new PPM(count1, inpFileName);
		if (From != string("RGB")) {
			cout << "TRAXALSA JOSKA";
					if (From == "HSL") {
						pict->From_HSL_to_RGB();
					}
					else if (From == "HSV") {
						pict->From_HSV_to_RGB();
					}
					else if (From == "YCbCr.601") {
						pict->From_YCbCr601_to_RGB();
					}
					else if (From == "YCbCr.709") {
						pict->From_YCbCr709_to_RGB();
					}
					else if (From == "YCoCg") {
						pict->From_YCoCg_to_RGB();
					}
					else if (From == "CMY") {
						pict->From_CMY_to_RGB();
					}
					else {
						throw exception("There is no such Color Space");
					}
		}
		if (To != "RGB") {
					if (To == "HSL") {
						pict->From_RGB_to_HSL();
					}
					else if (To == "HSV") {
						pict->From_RGB_to_HSV();
					}
					else if (To == "YCbCr.601") {
						pict->From_RGB_to_YCbCr601();
					}
					else if (To == "YCbCr.709") {
						pict->From_YCbCr709_to_RGB();
					}
					else if (To == "YCoCg") {
						pict->From_RGB_to_YCoCg();
					}
					else if (To == "CMY") {
						pict->From_RGB_to_CMY();
					}
					else {
						throw exception("There is no such Color Space");
					}
		}
		pict->write(count2, outFileName);
	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}
	delete pict;
	return 0;

}