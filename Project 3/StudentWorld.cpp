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

bool StudentWorld::overlap(int x, int y) {
    for(list<Actor*>::iterator p = actors.begin(); p != actors.end();p++) {
        Actor* temp = *p;
        int thisX = temp->getX(), thisY = temp->getY();
        int dx = x - thisX, dy = y - thisY;
        if((dx*dx + dy*dy) <= 64) return true;
    }
    return false;
}

int StudentWorld::init()
{
    player = new Socrates(this);
    for(int i = 0;i < getLevel();i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        BacteriaPit* newBP = new BacteriaPit(VIEW_WIDTH/2 + locX,
                                             VIEW_HEIGHT/2 + locY, this);
        addActor(newBP);
        cout << "loop 1";
    }

    for(int i = 0;i < min(5 * getLevel(), 25);i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        Food* newFood = new Food(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newFood);
        cout << "loop 2" << endl;
    }

    for(int i = 0;i < max(20, 180 - 20 * getLevel());i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        Dirt* newDirt = new Dirt(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newDirt);
        cout << "loop 3" << endl;
    }
    return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

    player->doSomething();


    cout << "point 1" << endl;
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* temp = *p;
        temp->doSomething();
        cout << "dirt moving " << endl;
    }

    if(!player->isAlive()){ decLives();  return GWSTATUS_PLAYER_DIED; }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{

    delete player;
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++)
        delete *p;
    actors.clear();
}

void StudentWorld::addActor(Actor *a) {
    cout << "addactor" << endl;
    actors.push_back(a);
}

StudentWorld::~StudentWorld() {
    cleanUp();
}
