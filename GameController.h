#ifndef __GAMECONTROLLER_243856
#define __GAMECONTROLLER_243856

#include <ncurses.h>    // the ncurses library
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h>     // sleep
#include <list>

//GameObjects
#include "PlayerShip.h"
#include "Projectile.h"
#include "EnemyShip.h"

class GameController{
	static const int delay = 50;
public:
	void init();
	bool loop();
	void countDown();
	void drawBoard();
	void death(std::list<Projectile>&, std::list<EnemyShip>&, PlayerShip&);
	bool gameOver(int[], bool);
};

#endif