#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include <iostream>
using namespace std;
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    player = new Socrates(this);
    for(int i = 0;i < max(20, 180 - 20 * getLevel());i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        Dirt* newDirt = new Dirt(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newDirt);
    }
    return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.


    player->doSomething();

    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        (*p)->doSomething();
    }
    if(!player->isAlive()){ decLives();  return GWSTATUS_PLAYER_DIED; }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    /*
    delete player;
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++)
        delete *p;
    actors.clear();
     */
}

void StudentWorld::addActor(Actor *a) {
    // actors.push_front(a);
}

StudentWorld::~StudentWorld() {
    // cleanUp();
}
