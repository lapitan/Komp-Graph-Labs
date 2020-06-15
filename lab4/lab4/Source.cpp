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
	string To;
	string From;
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
	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}
	try
	{
		pict->ColorSpace_changes(From, To);
		pict->write(count2, outFileName);
	}
	catch (const std::exception&)
	{

	}
	delete pict;
	return 0;

}