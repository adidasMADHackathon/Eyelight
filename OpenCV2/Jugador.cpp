#include "Jugador.h"



Jugador::Jugador(Coordenada coor1, double angle, double distance, bool myTeam)
{
	setCoor(coor1);
	setAnglePlayer(angle);
	setDistance(distance);
	setMyTeam(myTeam);
}


Jugador::~Jugador()
{
}

Coordenada Jugador::getCoor() {

	return Jugador::c1;

}

void Jugador::setCoor(Coordenada coor1) {

	Jugador::c1 = Coordenada(coor1.getX(), coor1.getY());

}

bool Jugador::getMyTeam() {
	return Jugador::myTeam;
}

void Jugador::setMyTeam(bool myTeam) {
	Jugador::myTeam = myTeam;
}

double Jugador::getAnglePLayer() {

	return Jugador::anglePlayer;

}

void Jugador::setAnglePlayer(double angle) {

	Jugador::anglePlayer = angle;

}

double Jugador::getDistance() {

	return Jugador::distancePlayer;

}

void Jugador::setDistance(double distance) {

	Jugador::distancePlayer = distance;

}