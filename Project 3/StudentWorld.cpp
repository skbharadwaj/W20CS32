#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <cmath>
#include <algorithm>
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
    soc = new Socrates(this);
    for(int i = 0;i < max(180 - 20 * getLevel(), 20);i++) {
        int locX = randInt(0, VIEW_WIDTH);
        int locY = randInt(0, VIEW_HEIGHT);
        while(sqrt((locX - VIEW_WIDTH/2)*(locX - VIEW_WIDTH/2) + (locY - VIEW_HEIGHT/2)*(locY - VIEW_HEIGHT/2)) > 120) 
            locY = randInt(0, VIEW_WIDTH);
        addDirt(locX, locY);
    } 
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::addDirt(const int &x, const int &y) {
    Dirt *d = new Dirt(x, y, this);
    actors.push_back(d);
}
int StudentWorld::move()
{
    soc->doSomething();
    for(list<*Actor>::iterator p = actors.begin();p != actors.end();p++) {
        *p->doSomething();
    }
    if(!soc.isAlive()) return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    delete soc;
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++)
        delete *p;
    actors.clear();
}
StudentWorld::~StudentWorld() {
    cleanUp();
}
