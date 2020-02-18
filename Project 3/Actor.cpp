#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

Actor::Actor(int imageID, double sX, double sY, Direction dir, int depth,
      StudentWorld* sWorld)
: GraphObject(imageID, sX, sY, dir, depth) {
   world = sWorld;
   aliveStatus = true;
}

Dirt::Dirt(double sX, double sY, StudentWorld* sWorld)
: Actor(IID_DIRT, sX, sY, 0, 1, sWorld) {}

HPActor::HPActor(int imageID, double sX, double sY, Direction dir, int depth,
                 StudentWorld* sWorld)
: Actor(imageID, sX, sY, dir, depth, sWorld), hp(100) {}

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
void Actor::polarToRect(int radius, Direction d, int& x, int& y) {
    x = radius*cos((double)((M_PI/180)*d));
    y = radius*sin((double)((M_PI/180)*d));
}
