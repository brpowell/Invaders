#include "GameController.h"
#include "timehelper.cpp"
//#include <iostream>

void GameController::init(){

	srand (time(NULL)); /// seed to some random number based on time

    if ( initscr() == NULL ) { /// init the screen, defaults to stdscr
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    noecho(); /// don't echo keypresses
    keypad(stdscr, TRUE); /// set the keypad to stdscr
    nodelay(stdscr, false);
    curs_set(0); /// set cursor to invisible (if allowed by the os)
    timeout(delay); /// set how long between keypresses (more or less)

}

void GameController::death(std::list<Projectile>& enemyLasers, std::list<EnemyShip>& enemies, PlayerShip& player){
	for(std::list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter){
		attron(COLOR_PAIR(2));
		mvaddch((*iter).y, (*iter).x, (*iter).shape);
	}

	enemyLasers.clear();
	
	attron(COLOR_PAIR(1));
	mvaddch(player.y,player.x,'*');
	refresh();
	nsleep(2000);
	player.x = 17;
	player.y = 18;
}

void GameController::countDown(){
	attron(COLOR_PAIR(2));
	for(int t = 3; t > 0; --t){
		mvprintw(12, 17, "%i", t);
		refresh();
		nsleep(1000);
	}
}

bool GameController::gameOver(int stats[], bool win){

	int ch;

	erase();
	drawBoard();

	/* 
    statistics table
    	0 - Points 	3 - Shots Fired
    	1 - lives 	4 - Hits
    	2 - Time
    */

	if(win){
		int time_bonus = (60 - stats[2]) * 7;	
		if(time_bonus < 0)
			time_bonus = 0;
		int lives_bonus = stats[1] * 220;
		double accuracy = (double)stats[4] / (double)stats[3];
		int accuracy_bonus = (int)(accuracy * (double)stats[0]);
		int final_score = stats[0] + time_bonus + lives_bonus + accuracy_bonus;

		attron(COLOR_PAIR(2));
		mvprintw(8, 12, "Good Stuff!");
		attron(COLOR_PAIR(7));
		mvprintw(10, 10, "Raw Score: %i", stats[0]);
		mvprintw(11, 10, "Time Bonus: %i", time_bonus);
		mvprintw(12, 10, "Lives Bonus: %i", lives_bonus);
		mvprintw(13, 6, "Accuracy Bonus: %i (%.1f%%)", accuracy_bonus, (accuracy*100));

		attron(COLOR_PAIR(1));
		mvprintw(15, 9, "Final Score: ");
		attron(COLOR_PAIR(7));
		mvprintw(15, 22, "%i pts", final_score);
		attron(COLOR_PAIR(2));
		mvprintw(17, 11, "Try again? y/n");
	}
	else{
		attron(COLOR_PAIR(7));
		mvprintw(8, 12, "Game Over");
		attron(COLOR_PAIR(1));
		mvprintw(10, 9, "Final Score: ");
		attron(COLOR_PAIR(7));
		mvprintw(10, 22, "%i pts", stats[0]);
		attron(COLOR_PAIR(2));
		mvprintw(12, 10, "Try again? y/n");
	}
	refresh();

	while(true){
		ch = getch();
		if(ch == 'y')
			return true;
		else if(ch == 'n')
			return false;
	}

}

void GameController::drawBoard(){
	attron(COLOR_PAIR(5));
	mvprintw(0, 0, "-------------------------------------");
	mvprintw(2, 0, "-------------------------------------");
	mvprintw(23, 0, "-------------------------------------");
	mvprintw(26, 0, "-------------------------------------");
	for(int i = 3; i < 23; ++i){
		mvprintw(i, 0, "|                                   |");
	}
	mvprintw(1, 0, "|                                   |");
	mvprintw(24, 0, "|                                   |");
	mvprintw(25, 0, "|                                   |");

	attron(COLOR_PAIR(3));
	mvprintw(1, 14, "INVADERS!");
}

bool GameController::loop(){
	/// some variables to give you some hints
    int ch = 0;
    bool quit = false;
    int elapsedTime = getElapsedTime();
    int ticks = 0;

    /* 
    statistics table
    	0 - Points 	3 - Shots Fired
    	1 - lives 	4 - Hits
    	2 - Time
    */
    int stats[5] = {0, 2, 0, 0, 0};

    start_color();
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    PlayerShip player(17,18);
    std::list<EnemyShip> enemies;
    std::list<Projectile> lasers;
    std::list<Projectile> enemyLasers;
    Projectile *laserP;
    EnemyShip *enemyP;

    // Fill the play area with enemies
    int posX = 11;
    int posY = 3;
    char eShape;
    for(int i = 0; i < 22; ++i){
    	(i % 2 == 0) ? eShape = 'W' : eShape = 'u';
    	EnemyShip e(posX, posY, eShape);
    	enemies.push_front(e);
    	posX += 2;
    	if(i == 10){
    		posY += 2;
    		posX = 11;
    	}
    }

    drawBoard();
    countDown();
    while (!quit){	
	    ch = getch();
	    erase(); /// erase the screen (after getch())
	    if ( ch != ERR) { /// user has a keypress
	        /// this is to delay until the next tick
	        elapsedTime = getElapsedTime();
	        if (elapsedTime < delay){
	            nsleep(delay-elapsedTime);}
	    }
	
	    drawBoard();
	    mvprintw(27, 0, "press q for destruction of human race");
		mvprintw(24, 6, "Points: %i", stats[0]);
		mvprintw(24, 21, "Ammo: %i", 5-lasers.size());
		mvprintw(25, 6, "Time: %i", stats[2]);
		mvprintw(25, 21, "Lives: %i", stats[1]);


	    switch(ch){
	            case KEY_UP: 
	            	if(player.y > 3)
	            		player.moveUp();
	            	break;
	            case KEY_RIGHT:	
	            	if(player.x < 35)
	            		player.moveRight();
	            	break;
	            case KEY_DOWN:
	            	if(player.y < 22)
	            		player.moveDown();
	            	break;
	            case KEY_LEFT:
	            	if(player.x > 1)
	            		player.moveLeft();
	            	break;
	            case ' ':{
	            	if(lasers.size() < 5){
		            	Projectile p(player.x, player.y, true);
		            	lasers.push_front(p);
		            	stats[3]++;
		            }
	            	break;}
	            case 'q': 
	                return gameOver(stats, false);
	                break;
	            case 'w':
	            	return gameOver(stats, true);
	    }

	    // Collision test player lasers against enemies
	    for(std::list<Projectile>::iterator iterP = lasers.begin(); iterP != lasers.end(); ++iterP){
	    	laserP = &(*iterP);
	    	for(std::list<EnemyShip>::iterator iterE = enemies.begin(); iterE != enemies.end(); ++iterE){
	    		enemyP = &(*iterE);
	    		if(laserP->x == enemyP->x && laserP->y == enemyP->y){
	    			if(enemyP->shape == 'W')
	    				stats[0] += 100;
	    			else if(enemyP->shape == 'u')
	    				stats[0] += 75;
	    			iterE = enemies.erase(iterE);
	    			iterP = lasers.erase(iterP);
					attron(COLOR_PAIR(1));
					mvaddch(enemyP->y,enemyP->x,'*');
					stats[4]++;
	    		}
	    	}
	    }

	    // Collision test enemy lasers against player
	    for(std::list<Projectile>::iterator iter = enemyLasers.begin(); iter != enemyLasers.end(); ++iter){
	    	laserP = &(*iter);
	    	if(laserP->x == player.x && laserP->y == player.y){
	    		death(enemyLasers, enemies, player);
	    		stats[1]--;
	    		break;
	    	}
	    }

	    // Collision test enemy ships against player
	    for(std::list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter){
	    	enemyP = &(*iter);
	    	if(enemyP->x == player.x && enemyP->y == player.y){
	    		iter = enemies.erase(iter);
	    		death(enemyLasers, enemies, player);
	    		stats[1]--;
	    		break;
	    	}
	    }


	    // Player is out of lives? Game Over
	    if(stats[1] < 0)
	    	return gameOver(stats, false);

	    // Player destroys all enemies? Game Win
	    if(enemies.empty())
	    	return gameOver(stats, true);

	    // Enemies reach the bottom of the board? Game Over
	    for(std::list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter){
	    	enemyP = &(*iter);
	    	if(enemyP->y >= 20)
	    		return gameOver(stats, false);
	    }

	    // Player laser updates
	    for(std::list<Projectile>::iterator iter = lasers.begin(); iter != lasers.end(); ++iter){
			laserP = &(*iter);
			laserP->timeStep();
			if(laserP->y < 3)
				iter = lasers.erase(iter);
			else{
				//init_pair(6, COLOR_CYAN, COLOR_BLACK);
				attron(COLOR_PAIR(6));
				mvaddch(laserP->y, laserP->x, laserP->shape);
			}
		}

		// EnemyShip updates
		for(std::list<EnemyShip>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter){
			enemyP = &(*iter);
			enemyP->timeStep();
			if(enemyP->fire){
				Projectile k(enemyP->x, enemyP->y, false);
				enemyLasers.push_front(k);
				enemyP->fire = false;
			}
			attron(COLOR_PAIR(2));
			mvaddch(enemyP->y, enemyP->x, enemyP->shape);
		}

		// Enemy laser updates
		for(std::list<Projectile>::iterator iter = enemyLasers.begin(); iter != enemyLasers.end(); ++iter){
			laserP = &(*iter);
			laserP->timeStep();
			if(laserP->y > 22)
				iter = enemyLasers.erase(iter);
			else{
				attron(COLOR_PAIR(1));
				mvaddch(laserP->y, laserP->x, laserP->shape);
			}
		}

		attron(COLOR_PAIR(7));
	    mvaddch(player.y,player.x,player.shape);

	    refresh(); // refresh the screen after adding everything
	    move(0,0); /// move cursor to 0,0 (looks prettier if os doesn't allow invisible cursors)
	    ticks++;
	    if(ticks % 20 == 0)
	    	stats[2]++;
	}   
}
