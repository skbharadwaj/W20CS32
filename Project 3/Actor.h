#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
class StudentWorld;
class Actor: public GraphObject {
   public:
       Actor(int imageID, double sX, double sY, Direction dir, int depth,
             StudentWorld* sWorld);
       virtual void doSomething() = 0;
       void die() { aliveStatus = false; }
       virtual bool isHPActor() { return false; }
       virtual bool isSocrates() { return false; }
       virtual bool isDirt() { return false; }
       StudentWorld* getSW() const { return world; }
       bool isAlive() const { return aliveStatus; }
       static void polarToRect(int radius, Direction d, int& x, int& y);
       virtual ~Actor() {}
   private:
       StudentWorld* world;
       bool aliveStatus;
};


class Dirt: public Actor {
    public:
        Dirt(double sX, double sY, StudentWorld* sWorld);
        void doSomething() { return; }
        bool isDirt() { return true; }
        virtual ~Dirt() {}
};

class HPActor: public Actor {
   public:
       HPActor(int imageID, double sX, double sY, Direction dir, int depth,
               StudentWorld* sWorld);
       void loseHP(int x) { hp -= x; }
       bool isHPActor() { return true; }
       virtual void doSomething() = 0;
       void gainHP(int x) { hp += x; }
       void setHP(int x) { hp = x; }
       int getHP() { return hp; }
       virtual ~HPActor() {}
   private:
       int hp;
};
class Socrates: public HPActor {
    public:
        Socrates(StudentWorld* sWorld);
        void doSomething();
        void changeSpray(const int& x) { spray += x; }
        void changeFlame(const int& x) { flame += x; }
        bool isSocrates() const { return true; }
        int getSpray() const { return spray; }
        int getFlame() const { return flame; }
        virtual ~Socrates() {}
    private:
        int spray, flame;
};
#endif // ACTOR_H_
