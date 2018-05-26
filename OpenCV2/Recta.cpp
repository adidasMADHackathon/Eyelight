#include "Recta.h"

Recta::Recta()
{
	setM(0);
}

Recta::Recta(double m)
{
	setM(m);
}


Recta::~Recta()
{
}

double Recta::getM() {

	return Recta::m;

}

void Recta::setM(double m) {

	Recta::m = m;

}

Coordenada Recta::getCoor1() {

	return Recta::c1;

}

void Recta::setCoor1(int x, int y) {

	Recta::c1 = Coordenada(x, y);

}

Coordenada Recta::getCoor2() {

	return Recta::c2;

}

void Recta::setCoor2(int x, int y) {

	Recta::c2 = Coordenada(x, y);

}

Coordenada Recta::getVector() {

	return Recta::vector;

}

void Recta::setVector(int x, int y) {

	Recta::vector = Coordenada(x, y);

}