#ifndef __ENEMYSHIP_9371856
#define __ENEMYSHIP_9371856

#include "Ship.h"
#include <stdlib.h>

class EnemyShip : public Ship {
public:
	int ticks;
	int shifts;
	bool fire;
	EnemyShip();
	EnemyShip(int, int, char);
	virtual void timeStep();
};

#endif