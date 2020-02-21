#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

Actor::Actor(int imageID, double sX, double sY, Direction dir, int depth,
      StudentWorld* sWorld)
: GraphObject(imageID, sX, sY, dir, depth) {
   world = sWorld;
   aliveStatus = true;
}

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
: HPActor(IID_SALMONELLA, sX, sY, 90, 0, sWorld) {
    setMovementPlanDistance(0);
    setHP(4);
}

void Salmonella::doSomething() {
    
    return;
}

AggressiveSalmonella::AggressiveSalmonella(double sX, double sY, StudentWorld* sWorld)
: HPActor(IID_SALMONELLA, sX, sY, 90, 0, sWorld) {
    setMovementPlanDistance(0);
    setHP(10);
}

void AggressiveSalmonella::doSomething() {
    return;
}

EColi::EColi(double sX, double sY, StudentWorld* sWorld)
: HPActor(IID_ECOLI, sX, sY, 90, 0, sWorld) {
    setMovementPlanDistance(0);
    setHP(5);
}

void EColi::doSomething() {
    return;
}

Socrates::Socrates(StudentWorld* sWorld)
: HPActor(IID_PLAYER, 0, VIEW_WIDTH/2, 0, 0, sWorld), spray(20), flame(5) {}
void Socrates::doSomething() {
   if(!isAlive()) return;
   int ch;
   if (getSW()->getKey(ch)) {
      switch(ch) {
          case KEY_PRESS_UP: {
          case KEY_PRESS_LEFT: {
              int x = getX();
              int y = getY();
              polarToRect(VIEW_RADIUS, getDirection() - 175, x, y);
              moveTo(VIEW_WIDTH/2 + x, VIEW_HEIGHT/2 + y);
              setDirection(getDirection() + 5);
              break;
          }
          }
          case KEY_PRESS_DOWN: {
          case KEY_PRESS_RIGHT: {
              int x = getX();
              int y = getY();
              polarToRect(VIEW_RADIUS, getDirection() + 175, x, y);
              moveTo(VIEW_WIDTH/2 + x, VIEW_HEIGHT/2 + y);
              setDirection(getDirection() - 5);
              break;
          }
      }
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
        Actor* newBac;
        switch(whichBac) {
            case 0:
                newBac = new Salmonella(getX(), getX(), getSW());
                break;
            case 1:
                newBac = new AggressiveSalmonella(getX(), getX(), getSW());
            case 2:
                newBac = new EColi(getX(), getX(), getSW());
            default: newBac = new Salmonella(getX(), getX(), getSW());
        }
        getSW()->addActor(newBac);
        decBacteria(whichBac);
        getSW()->playSound(SOUND_BACTERIUM_BORN);
    }

}
