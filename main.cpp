/** 
Name: Bryan Powell
Eid: bp7897
*/

#include "GameController.h"

int main (int argc, char const *argv[])
{
    GameController invaders;
    bool play = true;

    invaders.init();

    while(play)
    	play = invaders.loop();
    
    endwin();   /// cleanup the window

    return 0;
}