#pragma once
#include "Coordenada.h"
class Jugador
{
public:
	Jugador();
	~Jugador();

private:
	Coordenada c1;
	bool myTeam;
	double anglePlayer;
	double distancePlayer;

};

