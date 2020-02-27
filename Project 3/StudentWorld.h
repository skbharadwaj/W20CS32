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
class DamagingObject;
class Goodie;
class EColi;
class Salmonella;
class AggressiveSalmonella;
class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void addActor(Actor* a);
    virtual ~StudentWorld();
    bool overlap(int x, int y);
    void flameAllDamageAbleObjects(DamagingObject* actor);
    void socratesGetsGoodie(Goodie* goodie);
    void eColiMove(EColi* ecoli);
    void salmonellaMove(Salmonella* salmonella);
    void aggressiveSalmonellaMove(AggressiveSalmonella* as);
    bool checkIfDirtNearby(int x, int y);
    bool overlapsWithSocrates(Actor* actor);
    void displayThings();
    void addSpraySocrates();
    bool isOutOfBounds(double x, double y);
    bool checkIfOverlapAndRemove(DamagingObject* damaging);
    void addFoodHere(int x, int y);
    void addFlameCharges();
private:
   list<Actor*> actors;
   Socrates* player;
};

#endif // STUDENTWORLD_H_
