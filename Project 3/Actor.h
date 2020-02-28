#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
class StudentWorld;
class Actor: public GraphObject {
   public:
       Actor(int imageID, double sX, double sY, Direction dir, int depth,
             StudentWorld* sWorld);
       virtual void doSomething() = 0;
       virtual void die() { aliveStatus = false; }
       virtual bool isHPActor() { return false; }
       virtual bool isDirt() { return false; }
       virtual bool isFood() { return false; }
       virtual bool isPit() { return false; }
       virtual bool isFungus() { return true; }
       virtual bool isFlame() { return false;  }
       virtual bool isBacteria() { return false; }
       virtual bool isSalmonella() { return false; }
       virtual bool isAggressiveSalmonella() { return false; }
       virtual bool isDamageableObject() { return false; }
       virtual bool isBlockingObject() { return false; }
       virtual bool isEColi() { return false; }
       virtual bool isGoodie() { return false; }
       virtual void gainHP(int x) {}
       virtual void setHP(int x) {}
       virtual int getHP() { return 0 ;}
       StudentWorld* getSW() const { return world; }
       bool isAlive() const { return aliveStatus; }
       static void polarToRect(int radius, Direction d, int& x, int& y);
       virtual ~Actor() {}
   private:
       StudentWorld* world;
       bool aliveStatus;
};

class Goodie: public Actor {
public:
    Goodie(int imageID, double sX, double sY,StudentWorld* sWorld);
    virtual void doSomething();
    bool isGoodie() { return true; }
    int getLifetime(){ return lifetime; }
    void setLifetime(int x) { lifetime = x; }
    virtual bool isRestoreHealth() { return false; }
    virtual bool isFlamethrower() { return false; }
    virtual bool isExtraLife() { return false; }
    virtual bool isFungus() { return false; }
    bool isDamageableObject() { return true; }
    virtual ~Goodie() {}
private:
    int lifetime;
};

class Fungus: public Goodie {
public:
    Fungus(double sX, double sY, StudentWorld* sWorld);
    bool isFungus() { return true; }
    virtual ~Fungus() {}
};

class RestoreHealth: public Goodie {
public:
    RestoreHealth(double sX, double sY, StudentWorld* sWorld);
    bool isRestoreHealth() { return true; }
    virtual ~RestoreHealth() {};
};

class Flamethrower: public Goodie {
public:
    Flamethrower(double sX, double sY, StudentWorld* sWorld);
    bool isFlamethrower() { return true; }
    virtual ~Flamethrower() {}
};

class ExtraLife: public Goodie {
public:
    ExtraLife(double sX, double sY, StudentWorld* sWorld);
    bool isExtraLife() { return true; }
    virtual ~ExtraLife() {}
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

class DamagingObject: public Actor {
public:
    DamagingObject(int imageID, double sX, double sY, Direction dir, int depth,
    StudentWorld* sWorld);
    int getTravelDistance() { return max_travel_distance; }
    void setTravelDistance(int x) { max_travel_distance = x; }
    void doSomething();
    virtual bool isSpray() { return false; }
    virtual bool isFlame() { return false; }
    virtual ~DamagingObject() {}
private:
    int max_travel_distance;
};

class Flame: public DamagingObject {
public:
    Flame(double sX, double sY, Direction dir, StudentWorld* sWorld);
    bool isFlame() { return true; }
    virtual ~Flame() {}
};

class Spray: public DamagingObject {
public:
    Spray(double sX, double sY, Direction dir, StudentWorld* sWorld);
    bool isSpray() { return true; }
    virtual ~Spray() {}
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
        bool isDamageableObject() { return true; }
        virtual ~Dirt() {}
};

class HPActor: public Actor {
   public:
       HPActor(int imageID, double sX, double sY, Direction dir, int depth,
               StudentWorld* sWorld);
       void loseHP(int x) { hp -= x; }
       bool isHPActor() { return true; }
       void gainHP(int x) { hp += x; }
       void setHP(int x) { hp = x; }
       int getHP() { return hp; }
       int getMovementPlanDistance() { return movement_plan_distance; }
       void setMovementPlanDistance(int a) { movement_plan_distance = a; }
       bool isDamageableObject() { return true; }
       virtual ~HPActor() {}
   private:
       int hp, movement_plan_distance;
};

class Bacteria: public HPActor {
public:
    Bacteria(int imageID, double sX, double sY, StudentWorld* sWorld);
    virtual void doSomething();
    virtual bool isBacteria() { return true; }
    virtual bool isSalmonella() { return false; }
    virtual bool isAggressiveSalmonella() { return false; }
    virtual bool isEColi() { return false; }
    virtual bool isDamageableObject() { return true; }
    void die();
    int getFoodEaten() { return foodEaten; }
    void setFoodEaten(int x) { foodEaten = x; }
private:
    int foodEaten;
};

class Salmonella: public Bacteria {
public:
    Salmonella(double sX, double sY, StudentWorld* sWorld);
    bool isSalmonella() { return true; }
    virtual void doSomething();
    virtual ~Salmonella() {}
};

class AggressiveSalmonella: public Salmonella {
public:
    AggressiveSalmonella(double sX, double sY, StudentWorld* sWorld);
    virtual void doSomething();
    bool isAggressiveSalmonella() { return true; }
    virtual ~AggressiveSalmonella() {}
};

class EColi: public Bacteria {
public:
    EColi(double sX, double sY, StudentWorld* sWorld);
    void doSomething();
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

