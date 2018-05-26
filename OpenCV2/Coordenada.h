#pragma once
class Coordenada
{
public:
	Coordenada();
	~Coordenada();
	Coordenada(int, int);
	int getX();
	void setX(int x);

	int getY();
	void setY(int y);

private:
	int x, y;
};

