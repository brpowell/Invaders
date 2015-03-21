#ifndef __SHIP_4763856
#define __SHIP_4763856

#include "GameObject.h"

class Ship : public GameObject {
public:
	void moveUp(){ y--; }
	void moveDown(){ y++; }
	void moveLeft(){ x--; }
	void moveRight(){ x++; }
};

#endif