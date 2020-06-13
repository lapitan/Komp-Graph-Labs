#include <iostream>
#include <string>
#include "PGM.h"

int main(int argc, char* argv[]) {
	
	if (argc != 7) {

		cerr << "Command is invalid";
		return 1;

	}

	string inpFileName = string(argv[1]);
	string outFileName = string(argv[2]);
	bool grad=true;
	int numb_dith;
	int bit=5;
	double gamma=2.4;

	try {

		grad = (string(argv[3]) == "1");
		numb_dith = atoi(argv[4]);
		bit = atoi(argv[5]);
		gamma = stold(argv[6]);

	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}

	PGM* pict;
	try {
		pict = new PGM(inpFileName, grad, gamma);
	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}

	if (numb_dith == 0) {
		pict->no_dithering(bit);
	}
	else if (numb_dith == 1) {
		pict->ordered_dithering(bit);
	}
	else if (numb_dith == 2) {
		pict->random_dithering(bit);
	}
	else if (numb_dith == 3) {
		pict->floyd_dithering(bit);
	}
	else if (numb_dith == 4) {
		pict->Jarvis_dithering(bit);
	}
	else if (numb_dith == 5) {
		pict->Sierra3_dithering(bit);
	}
	else if (numb_dith == 6) {
		pict->Atkinson_dithering(bit);
	}
	else if (numb_dith == 7) {
		pict->Halftone(bit);
	}
	else {
		cerr << "There is no such algorithm";
		return 1;
	}
	try {
		pict->write(outFileName, gamma, bit);
	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}
	return 0;

}