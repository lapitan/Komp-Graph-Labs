#include <iostream>
#include <string>
#include "PGM.h"

int main(int argc, char* argv[]) {


	if (argc != 7) {

		cerr << "Command is invalid";
		return 1;

	}

	string inpFileName=string(argv[1]);
	string outFileName=string(argv[2]);
	bool grad;
	int numb_dith;
	int bit;
	double gamma;

	try {

		grad = (string(argv[3]) == "1");
		if (string(argv[3]) == "1") {
			grad = true;
		}
		else if (string(argv[3]) == "0") {
			grad = false;
		}
		else {
			cerr << "Wrong gradient";
			return 1;
		}
		if (strlen(argv[4]) != 1 || !isdigit(argv[4][0])) {
			cerr << "Algo should be int number from 0 to 7";
			return 1;
		}
		numb_dith = atoi(argv[4]);
		if (numb_dith < 0 || numb_dith>7) {
			cerr << "Wrong algo";
			return 1;
		}
		if (strlen(argv[5]) != 1 || !isdigit(argv[5][0])) {
			cerr << "Bits should be int number from 1 to 8";
			return 1;
		}
		bit = atoi(argv[5]);
		if (bit > 8 || bit < 1) {
			cerr << "Wrong bits";
			return 1;
		}
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
		delete pict;
		cerr << "There is no such algorithm";
		return 1;
	}
	try {
		pict->write(outFileName, gamma, bit);
	}
	catch (exception& ex) {
		delete pict;
		cerr << ex.what();
		return 1;
	}

	delete pict;
	return 0;

}