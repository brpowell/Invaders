#ifndef __PLAYERSHIP_4342856
#define __PLAYERSHIP_4342856

#include "Ship.h"

class PlayerShip : public Ship {
public:
	PlayerShip(int, int);
	virtual void timeStep();
};

#endif