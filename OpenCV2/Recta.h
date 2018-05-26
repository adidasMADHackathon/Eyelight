#pragma once
#include "Coordenada.h"
class Recta
{
public:
	Recta();
	Recta(double);
	~Recta();
	double getM();
	void setM(double m);
	Coordenada getCoor1();
	void setCoor1(int x, int y);
	Coordenada getCoor2();
	void setCoor2(int x, int y);
	Coordenada getVector();
	void setVector(int x, int y);

private:
	double m;
	Coordenada c1;
	Coordenada c2;
	Coordenada vector;
};

