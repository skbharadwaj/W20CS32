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
       virtual bool isFood() { return false; }
       virtual bool isPit() { return false; }
       virtual bool isBacteria() { return false; }
       virtual bool isSalmonella() { return false; }
       virtual bool isAggressiveSalmonella() { return false; }
       virtual bool isEColi() { return false; }
       StudentWorld* getSW() const { return world; }
       bool isAlive() const { return aliveStatus; }
       static void polarToRect(int radius, Direction d, int& x, int& y);
       virtual ~Actor() {}
   private:
       StudentWorld* world;
       bool aliveStatus;
};
class BacteriaPit: public Actor {
public:
    BacteriaPit(double sX, double sY, StudentWorld* sWorld);
    void doSomething();
    bool isPit() { return true; }
    int getRegSalmonella() { return bacteria[0]; }
    int getAggSalmonella() { return bacteria[1]; }
    int getEColi() { return bacteria[2]; }
    void decBacteria(int a);
    virtual ~BacteriaPit(){};
private:
    int bacteria[3];
};

class Food: public Actor {
public:
    Food(double sX, double sY, StudentWorld* sWorld);
    void doSomething() { return; }
    bool isFood() { return true; }
    virtual ~Food(){}
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
       int getMovementPlanDistance() { return movement_plan_distance; }
       void setMovementPlanDistance(int a) { movement_plan_distance = a; }
       virtual ~HPActor() {}
   private:
       int hp, movement_plan_distance;
};
class Salmonella: public HPActor {
public:
    Salmonella(double sX, double sY, StudentWorld* sWorld);
    void doSomething();
    bool isBacteria() { return true; }
    bool isSalmonella() { return true; }
    virtual ~Salmonella() {}
};

class AggressiveSalmonella: public HPActor {
public:
    AggressiveSalmonella(double sX, double sY, StudentWorld* sWorld);
    void doSomething();
    bool isBacteria() { return true; }
    bool isAggressiveSalmonella() { return true; }
    virtual ~AggressiveSalmonella() {}
};

class EColi: public HPActor {
public:
    EColi(double sX, double sY, StudentWorld* sWorld);
    void doSomething();
    bool isBacteria() { return true; }
    bool isEColi() { return true; }
    virtual ~EColi() {}
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
