#include "Projectile.h"

Projectile::Projectile(){
	x = 0;
	y = 0;
	shape = '|';
	player = false;
}

Projectile::Projectile(int x, int y, bool player){
	this->x = x;
	this->y = y;
	this->player = player;
	shape = '|';
}

void Projectile::timeStep(){
	player ? y-- : y++;
}