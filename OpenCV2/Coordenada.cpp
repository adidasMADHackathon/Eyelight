#include "Coordenada.h"


Coordenada::Coordenada()
{
	setX(0);
	setY(0);
}

Coordenada::Coordenada(int x, int y)
{
	setX(x);
	setY(y);
}

Coordenada::~Coordenada()
{

}

int Coordenada::getX() {

	return Coordenada::x;

}

void Coordenada::setX(int x) {

	Coordenada::x = x;

}

int Coordenada::getY() {

	return Coordenada::y;

}

void Coordenada::setY(int y) {

	Coordenada::y = y;

}
