#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

Actor::Actor(int imageID, double sX, double sY, Direction dir, int depth,
      StudentWorld* sWorld)
: GraphObject(imageID, sX, sY, dir, depth) {
   world = sWorld;
   aliveStatus = true;
}

Flamethrower::Flamethrower(double sX, double sY, StudentWorld* sWorld)
: Goodie(IID_FLAME, sX, sY, sWorld) {}

Fungus::Fungus(double sX, double sY, StudentWorld* sWorld)
: Goodie(IID_FUNGUS, sX, sY, sWorld) {}

ExtraLife::ExtraLife(double sX, double sY, StudentWorld* sWorld)
: Goodie(IID_EXTRA_LIFE_GOODIE, sX, sY, sWorld) {}


void Actor::polarToRect(int radius, Direction d, int& x, int& y) {
    x = radius*cos((double)((M_PI/180)*d));
    y = radius*sin((double)((M_PI/180)*d));
}

Food::Food(double sX, double sY, StudentWorld* sWorld)
: Actor(IID_FOOD, sX, sY, 90, 1, sWorld) {}


Dirt::Dirt(double sX, double sY, StudentWorld* sWorld)
: Actor(IID_DIRT, sX, sY, 0, 1, sWorld) {}

HPActor::HPActor(int imageID, double sX, double sY, Direction dir, int depth,
                 StudentWorld* sWorld)
: Actor(imageID, sX, sY, dir, depth, sWorld), hp(100) {}

Salmonella::Salmonella(double sX, double sY, StudentWorld* sWorld)
: Bacteria(IID_SALMONELLA, sX, sY, sWorld) {
    setMovementPlanDistance(0);
    setHP(4);
}

AggressiveSalmonella::AggressiveSalmonella(double sX, double sY, StudentWorld* sWorld)
: Salmonella(sX, sY, sWorld) {
    setMovementPlanDistance(0);
    setHP(10);
}


Bacteria::Bacteria(int imageID, double sX, double sY, StudentWorld* sWorld)
: HPActor(imageID, sX, sY, 90, 0, sWorld) {}

void Bacteria::doSomething() {
    if(!isAlive()) return;
}

void Bacteria::die() {
    Actor::die();
    if(randInt(0, 1) == 0) {
        getSW()->addFoodHere(getX(), getY());
    }
}

EColi::EColi(double sX, double sY, StudentWorld* sWorld)
: Bacteria(IID_ECOLI, sX, sY, sWorld) {
    setMovementPlanDistance(0);
    setHP(5);
}

void EColi::doSomething() {
    if(!isAlive()) return;
    getSW()->eColiMove(this);
}

void Salmonella::doSomething() {
    if(!isAlive()) return;
    getSW()->salmonellaMove(this);
}

void AggressiveSalmonella::doSomething() {
    if(!isAlive()) return;
    getSW()->aggressiveSalmonellaMove(this);
}

Socrates::Socrates(StudentWorld* sWorld)
: HPActor(IID_PLAYER, 0, VIEW_WIDTH/2, 0, 0, sWorld), spray(20), flame(5) {}

void Socrates::doSomething() {
    if(getHP() <= 0) die();
   if(!isAlive()) return;
   int ch;
   if (getSW()->getKey(ch)) {
      switch(ch) {
          case KEY_PRESS_LEFT: {
              int x = getX();
              int y = getY();
              polarToRect(VIEW_RADIUS, getDirection() - 175, x, y);
              moveTo(VIEW_WIDTH/2 + x, VIEW_HEIGHT/2 + y);
              setDirection(getDirection() + 5);
              break;
          }
          case KEY_PRESS_RIGHT: {
              int x = getX();
              int y = getY();
              polarToRect(VIEW_RADIUS, getDirection() + 175, x, y);
              moveTo(VIEW_WIDTH/2 + x, VIEW_HEIGHT/2 + y);
              setDirection(getDirection() - 5);
              break;
          }
          case KEY_PRESS_SPACE:
              if(getSpray() > 0) getSW()->addSpraySocrates();
              break;
          case KEY_PRESS_ENTER:
              if(getFlame() > 0) getSW()->addFlameCharges();
              break;
      }
   }
}

void BacteriaPit::decBacteria(int a) {
    bacteria[a]--;
}

BacteriaPit::BacteriaPit(double sX, double sY, StudentWorld* sWorld)
: Actor(IID_PIT, sX, sY, 0, 1, sWorld) {
    bacteria[0] = 5;
    bacteria[1] = 3;
    bacteria[2] = 2;
}

void BacteriaPit::doSomething() {
    if(bacteria[0] == 0 && bacteria[1] == 0 && bacteria[2] == 0) {
        die();
        return;
    }
    if(randInt(1, 50) == 1) {
        int whichBac = randInt(0, 2);
        while(bacteria[whichBac] == 0) whichBac = randInt(0, 2);
        Actor* newBac = nullptr;
        switch(whichBac) {
            case 0:
                newBac = new Salmonella(getX(), getY(), getSW());
                break;
            case 1:
                newBac = new AggressiveSalmonella(getX(), getY(), getSW());
                break;
            case 2:
                newBac = new EColi(getX(), getY(), getSW());
                break;
        }
        getSW()->addActor(newBac);
        bacteria[whichBac]--;
        getSW()->playSound(SOUND_BACTERIUM_BORN);
    }

}

DamagingObject::DamagingObject(int imageID, double sX, double sY, Direction dir, int depth, StudentWorld* sWorld)
: Actor(imageID, sX, sY, dir, depth, sWorld) {}

Flame::Flame(double sX, double sY, Direction dir, StudentWorld* sWorld)
: DamagingObject(IID_FLAME, sX, sY, dir, 1, sWorld) {
    setTravelDistance(32);
}

Spray::Spray(double sX, double sY, Direction dir, StudentWorld* sWorld)
: DamagingObject(IID_SPRAY, sX, sY, dir, 1, sWorld) {
    setTravelDistance(112);
}

void DamagingObject::doSomething() {
    if(!isAlive()) return;
    getSW()->flameAllDamageAbleObjects(this);
    moveAngle(getDirection(), SPRITE_WIDTH);
    setTravelDistance(getTravelDistance() - SPRITE_WIDTH);
    if(getTravelDistance() == 0) die();
    return;
}

Goodie::Goodie(int imageID, double sX, double sY,StudentWorld* sWorld)
: Actor(imageID, sX, sY, 1, 0, sWorld) {
    setLifetime(max(randInt(0, 300 - 10 *(sWorld->getLevel()) - 1), 50));
}

RestoreHealth::RestoreHealth(double sX, double sY, StudentWorld* sWorld)
: Goodie(IID_RESTORE_HEALTH_GOODIE, sX, sY, sWorld) {}

void Goodie::doSomething() {
    if(!isAlive()) return;
    getSW()->socratesGetsGoodie(this);
    return;
}


















