#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "StudentWorld.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
    public:
        Actor(int imageID, double sX, double sY, Direction dir, int depth, StudentWorld* sWorld);
        virtual void doSomething() = 0;
        void die() { aliveStatus = false; }
        virtual bool isHPActor() { return false; }
        virtual bool isSocrates() { return false; }
        StudentWorld* getSW() const { return world; }
        bool isAlive() const { return aliveStatus; }
        virtual ~Actor() {}
    private:
        StudentWorld* world;
        bool aliveStatus;
};
class HPActor: public Actor {
    public:
        HPActor(int imageID, double sX, double sY, Direction dir, int depth, StudentWorld* sWorld);
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
class Dirt: public Actor {
    public:
        Dirt(double sX, double sY, StudentWorld* sWorld);
        void doSomething();
        virtual ~Dirt() {}
};
class Socrates: public HPActor {
    public:
        Socrates(StudentWorld* sWorld);
        void doSomething();
        void changeSpray(int x) { spray += x; }
        void changeFlame(int x) { flame += x; }
        bool isSocrates() { return true; }
        int getSpray() { return spray; }
        int flame() { return flame; }
        virtual ~Socrates() {}
    private:
        int spray, flame;
};
#endif // ACTOR_H_
