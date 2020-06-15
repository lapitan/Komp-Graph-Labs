#include "PGM.h"
#include <iostream>

using namespace std;

int main(int argc,char* argv[]) {

	if (argc != 10) {
		cerr << "Wrong command";
		return 1;
	}

	string inpFileName = string(argv[1]);
	string outFileName = string(argv[2]);
	double x1=atoi(argv[5]);
	double y1 = atoi(argv[6]);
	double x2=atoi(argv[7]);
	double y2=atoi(argv[8]);
	int bright=atoi(argv[3]);
	PGM* pict;
	try {
		pict = new PGM(inpFileName);
		pict->wu_algo(x1, y1, x2, y2, bright);
		pict->write(outFileName);
	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}
	delete pict;
	return 0;

}