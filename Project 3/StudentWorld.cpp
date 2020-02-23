#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include <iostream>
using namespace std;
GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}
// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath): GameWorld(assetPath) {}

bool StudentWorld::overlap(int x, int y) {
    for(list<Actor*>::iterator p = actors.begin(); p != actors.end();p++) {
        Actor* temp = *p;
        if(!temp->isDirt()) {
            int thisX = temp->getX(), thisY = temp->getY();
            int dx = x - thisX, dy = y - thisY;
            if((dx*dx + dy*dy) <= SPRITE_WIDTH*SPRITE_WIDTH)
                return true;
        }
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
    }

    for(int i = 0;i < min(5 * getLevel(), 25);i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        while(overlap(locX + VIEW_WIDTH/2, locY + VIEW_HEIGHT/2)) {
            randRadius = randInt(0, 120);
            randDeg = randInt(0, 359);
            locX = randRadius*cos(randDeg*M_PI/180);
            locY = randRadius*sin(randDeg*M_PI/180);
        }
        Food* newFood = new Food(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newFood);
    }

    for(int i = 0;i < max(20, 180 - 20 * getLevel());i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        while(overlap(locX + VIEW_WIDTH/2, locY + VIEW_HEIGHT/2)) {
            randRadius = randInt(0, 120);
            randDeg = randInt(0, 359);
            locX = randRadius*cos(randDeg*M_PI/180);
            locY = randRadius*sin(randDeg*M_PI/180);
        }
        Dirt* newDirt = new Dirt(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newDirt);
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    player->doSomething();
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* temp = *p;
        temp->doSomething();
    }

    if(!player->isAlive()){ decLives();  return GWSTATUS_PLAYER_DIED; }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++)
        delete *p;
    actors.clear();
}

void StudentWorld::addActor(Actor *a) {
    actors.push_back(a);
}

void StudentWorld::flameAllDamageAbleObjects(DamagingObject* flame) {
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* curr = *p;
        int distX = flame->getX() - curr->getY();
        int distY = flame->getY() - curr->getY();
        int dist = distX*distX + distY*distY;
        if(curr->isDamageableObject() && dist <= SPRITE_WIDTH*SPRITE_WIDTH ) {
            if(curr->isDirt()) {
                curr->die();
                flame->die();
                return;
            }
            else if(curr->isHPActor()) {
                if(flame->isSpray()) curr->setHP(curr->getHP() - 2);
                else if(flame->isFlame()) curr->setHP(curr->getHP() - 5);
                flame->die();
                return;
            }
        }
    }

}

StudentWorld::~StudentWorld() {
    cleanUp();
}
