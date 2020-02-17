#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
class StudentWorld;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double sX, double sY, Direction dir, int depth, StudentWorld* sWorld) 
: GraphObject(imageID, sX, sY, dir, depth) {
    world = sWorld;
    aliveStatus = true;
}
HPActor::HPActor(int imageID, double sX, double sY, Direction dir, int depth, StudentWorld* sWorld)
: Actor(imageID, sX, sY, dir, depth, sWorld), hp(100) {}
Dirt::Dirt(double sX, double sY, StudentWorld* sWorld)
: Actor(IID_DIRT, sX, sY, 90, 1, sWorld) {}
void Dirt::doSomething() {
    return; 
}
void Socrates::Socrates(StudentWorld * sWorld) 
: HPActor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, sWorld) {
    setHP(100);
    spray = 20;
    flame = 5;
}
void Socrates::doSomething() {
    if(getHP() <= 0) return;
    int ch;
    if(getSW()->getKey(ch)) {
        switch(ch) {
            case KEY_PRESS_LEFT:
                if(getX() - 5 >= 0) moveTo(getX() - 5, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(RIGHT);
                if(getX() + 5 <= VIEW_WIDTH) moveTo(getX() + 5, getY());
                break;
            case KEY_PRESS_UP: 
                setDirection(UP);
                if(getY() + 5 <= VIEW_HEIGHT) moveTo(getX(), getY() + 5);
                break;
            case KEY_PRESS_DOWN: 
                setDirection(DOWN);
                if(getY() - 5 >= 0) moveTo(getX(), getY() - 5);    
                break;       
        }
    }
}
