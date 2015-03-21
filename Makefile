CC = g++ -Wall -g
NAME = invaders
SRCS = GameObject.cpp Ship.cpp PlayerShip.cpp EnemyShip.cpp Projectile.cpp GameController.cpp main.cpp
OBJS = $(SRCS.cpp=.o)


all: $(OBJS)
	$(CC) -o $(NAME) $(SRCS) -lncurses


clean:
	rm -rf *.o
