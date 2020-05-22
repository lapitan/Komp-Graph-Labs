#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "PGM.h";
#include "PPM.h";

using namespace std;

struct pixel
{

	unsigned char red;
	unsigned char green;
	unsigned char blue;

};

int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "command is invalid\n";
		exit(0);
	}
	string finName = string(argv[1]);
	string foutName = string(argv[2]);
	string command = string(argv[3]);
	ifstream fin;
	try
	{
		fin.open(finName,ios::binary);
	}
	catch (const std::exception& ex)
	{
		cout << ex.what();
	}

	char vers[2];
	try
	{
		fin >> vers[0] >> vers[1];
	}
	catch (const std::exception& ex)
	{
		cout << ex.what();
	}
	if (!(vers[0] == 'P' && (vers[1] == '5' || vers[1] == '6'))) {
		cout << "Version is invalid\n";
		exit(0);
	}

	PNM* picture;
	try
	{
		if (vers[1] == '5') {
			picture = new PGM(finName);
		}
		else {
			picture = new PPM(finName);
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what();
		exit(0);
	}

	try
	{
		for (int i = 0; i < command.length(); i++) {
			if (command[i] == '0') {
				picture->ColorInversion();
			}
			else if (command[i] == '1') {
				picture->HorizontalReflect();
			}
			else if (command[i] == '2') {
				picture->VerticalReflect();
			}
			else if (command[i] == '3') {
				picture->RotateRight();
			}
			else if (command[i] == '4') {
				picture->RotateLeft();
			}
			else {
				throw exception("Invalid command");
			}
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what();
	}

	try
	{
		picture->write(foutName);
	}
	catch (const std::exception& ex)
	{
		ex.what();
	}
	fin.close();
	return 0;

}