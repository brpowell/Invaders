#ifndef __GAMEOBJECT_4762356
#define __GAMEOBJECT_4762356

class Object;

class GameObject{
public:
	int x, y;
	int shape;

	virtual void timeStep() = 0;
	virtual char getShape() const{ return shape; }
};

#endif