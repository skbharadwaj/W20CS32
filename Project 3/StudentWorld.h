#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
#include <algorithm>
#include <cmath>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates;
class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void addActor(Actor* a);
    virtual ~StudentWorld();
    bool overlap(int x, int y);

private:
   list<Actor*> actors;
   Socrates* player;
};

#endif // STUDENTWORLD_H_
