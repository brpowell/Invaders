#ifndef __PROJECTILE_743856
#define __PROJECTILE_743856

#include "GameObject.h"

class Projectile : public GameObject{
public:
	bool player;
	Projectile();
	Projectile(int, int, bool);
	virtual void timeStep();
};

#endif