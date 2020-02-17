#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include <list>
#include <algorithm>
#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates; 
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void addDirt(const int &x, const int &y);
    virtual void cleanUp();
    virtual ~StudentWorld();

private:
    list<Actor *> actors;
    Socrates* soc;
    int level;
};

#endif // STUDENTWORLD_H_
