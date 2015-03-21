#include "EnemyShip.h"
#include <ctime>

EnemyShip::EnemyShip(){
	ticks = shifts = 0;
	x = y = 0;
	shape = ' ';
	fire = false;
}

EnemyShip::EnemyShip(int x, int y, char shape){
	ticks = shifts = 0;
	this->x = x;
	this->y = y;
	this->shape = shape;
	fire = false;
}

void EnemyShip::timeStep(){
	int chance;
	ticks++;

	if(shape == 'W')
		chance = 700;
	else if(shape == 'u')
		chance = 800;

	if(rand()%chance < 3)
		fire = true;

	if(shifts == 14)
		shifts = 0;

	if(ticks % 20 == 0){
		if(shifts < 7)
			moveLeft();
		else
			moveRight();
		shifts++;
	}

	if(ticks == 210){
		moveDown();
		ticks = 0;
	}
}