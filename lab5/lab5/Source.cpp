#include <iostream>
#include <set>
#include <iterator>
#include "PPM.h"

using namespace std;

int main(int argc,char* argv[]) {

	if (argc < 4) {
		cerr << "Too few arguments";
		return 1;
	}

	string inpFileName=string(argv[1]);
	string outFileName=string(argv[2]);
	int action_numb=atoi(argv[3]);
	int add=0;
	double mult=0;
	if(action_numb<=1&&argc!=6){
		cerr << "Wrong Arguments";
		return 1;
	}
	else if (action_numb >= 2 && argc != 4) {
		cerr << "Wrong Arguments";
		return 1;
	}
	if (action_numb <= 1) {
		add = atoi(argv[4]);
		mult = atoi(argv[5]);
	}
	PPM* pict;
	try {
		pict = new PPM(inpFileName);
		if (action_numb == 0) {
			pict->act0(add, mult);
		}
		else if (action_numb == 1) {
			pict->act1(add, mult);
		}
		else if (action_numb == 2) {
			pict->act2();
		}
		else if (action_numb == 3) {
			pict->act3();
		}
		else if (action_numb == 4) {
			pict->act4();
		}
		else if (action_numb == 5) {
			pict->act5();
		}
		else {
			throw exception("Wrong Algo");
		}
		pict->write(outFileName);
	}
	catch (exception& ex) {
		cerr << ex.what();
		return 1;
	}
	delete pict;
	return 0;

}