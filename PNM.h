#pragma once
#include <iostream>
#include <string>
using namespace std;

class PNM {
public:

	virtual void write(string outFileName) = 0;
	virtual void ColorInversion() = 0;
	virtual void HorizontalReflect() = 0;
	virtual void VerticalReflect() = 0;
	virtual void RotateRight() = 0;
	virtual void RotateLeft() = 0;

};