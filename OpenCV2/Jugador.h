#pragma once
#include "Coordenada.h"
class Jugador
{
public:
	Jugador(Coordenada c1, double angle, double distance, bool myTeam);
	~Jugador();
	Coordenada getCoor();
	void setCoor(Coordenada c1);
	bool getMyTeam();
	void setMyTeam(bool myTeam);
	double getAnglePLayer();
	void setAnglePlayer(double angle);
	double getDistance();
	void setDistance(double distance);

private:
	Coordenada c1;
	bool myTeam;
	double anglePlayer;
	double distancePlayer;
};

