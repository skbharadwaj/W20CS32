#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;
GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}
// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(string assetPath): GameWorld(assetPath) {}

bool StudentWorld::overlap(int x, int y) {
    for(list<Actor*>::iterator p = actors.begin(); p != actors.end();p++) {
        Actor* temp = *p;
        if(!temp->isDirt()) {
            int thisX = temp->getX(), thisY = temp->getY();
            int dx = x - thisX, dy = y - thisY;
            if((dx*dx + dy*dy) <= SPRITE_WIDTH*SPRITE_WIDTH)
                return true;
        }
    }
    return false;
}

void StudentWorld::displayThings() {
    ostringstream stream;
    stream << "  Score: ";
    stream.fill('0');
    if(getScore() >= 0) stream << setw(6) << getScore();
    else stream << "-" << setw(5) << getScore();
    stream.fill(' ');
    stream << "  Level " << setw(2) << getLevel() << "  Lives: " << setw(1)
    << getLives() << "  Health: " << setw(2) << player->getHP() << "  Sprays: "
    << setw(2) << player->getSpray() << "  Flames: " << setw(2) <<
    player->getFlame();
    setGameStatText(stream.str());
}

int StudentWorld::init()
{
    player = new Socrates(this);
    for(int i = 0;i < getLevel();i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        BacteriaPit* newBP = new BacteriaPit(VIEW_WIDTH/2 + locX,
                                             VIEW_HEIGHT/2 + locY, this);
        addActor(newBP);
    }

    for(int i = 0;i < min(5 * getLevel(), 25);i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        while(overlap(locX + VIEW_WIDTH/2, locY + VIEW_HEIGHT/2)) {
            randRadius = randInt(0, 120);
            randDeg = randInt(0, 359);
            locX = randRadius*cos(randDeg*M_PI/180);
            locY = randRadius*sin(randDeg*M_PI/180);
        }
        Food* newFood = new Food(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newFood);
    }

    for(int i = 0;i < max(20, 180 - 20 * getLevel());i++) {
        int randRadius = randInt(0, 120), randDeg = randInt(0, 359);
        int locX = randRadius*cos(randDeg*M_PI/180),
        locY = randRadius*sin(randDeg*M_PI/180);
        while(overlap(locX + VIEW_WIDTH/2, locY + VIEW_HEIGHT/2)) {
            randRadius = randInt(0, 120);
            randDeg = randInt(0, 359);
            locX = randRadius*cos(randDeg*M_PI/180);
            locY = randRadius*sin(randDeg*M_PI/180);
        }
        Dirt* newDirt = new Dirt(VIEW_WIDTH/2 + locX, VIEW_HEIGHT/2 + locY, this);
        addActor(newDirt);
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    displayThings();
    player->doSomething();
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* temp = *p;
        temp->doSomething();
        
    }

    if(!player->isAlive()){
        decLives();
        playSound(SOUND_PLAYER_DIE);
        delete player;
        return GWSTATUS_PLAYER_DIED;
    }

    actors.remove_if([this](Actor *a) {
      bool shouldRemove = !a->isAlive();
      if (shouldRemove)
        delete a;
        return shouldRemove;
    });


    int chanceFungus = max(510 - 10*getLevel(), 200);
    if(randInt(0, chanceFungus - 1) == 0) {
        int randAngle = randInt(0, 359);
        int fungusX = VIEW_RADIUS*cos((randAngle*M_PI/180)) + VIEW_WIDTH/2,
        fungusY = VIEW_RADIUS*sin((randAngle*M_PI/180)) + VIEW_HEIGHT/2;
        Fungus* f = new Fungus(fungusX, fungusY, this);
        addActor(f);
    }


    int chanceGoodie = max(510 - 10*getLevel(), 250);
    if(randInt(0, chanceGoodie-1) == 0) {
        int randAngle = randInt(0, 359);
        int goodieX = VIEW_RADIUS*cos((randAngle*M_PI/180)) + VIEW_WIDTH/2,
        goodieY = VIEW_RADIUS*sin((randAngle*M_PI/180)) + VIEW_HEIGHT/2;
        int whichGoodie = randInt(1, 10);
        if(whichGoodie >= 1 && whichGoodie <= 6) 
            addActor(new RestoreHealth(goodieX, goodieY, this));
        else if(whichGoodie >= 7 && whichGoodie <= 9)
            addActor(new Flamethrower(goodieX, goodieY, this));
        else addActor(new ExtraLife(goodieX, goodieY, this));
    }

    
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* temp = *p;
        if(temp->isPit() || temp->isBacteria())
            return GWSTATUS_CONTINUE_GAME;
    }
    playSound(SOUND_FINISHED_LEVEL);
    return GWSTATUS_FINISHED_LEVEL;
}

void StudentWorld::cleanUp()
{
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++)
        delete *p;
    actors.clear();
}

void StudentWorld::addActor(Actor *a) {
    actors.push_back(a);
}

void StudentWorld::flameAllDamageAbleObjects(DamagingObject* flame) {
    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* curr = *p;
        if(!curr->isFlame() && curr != flame && !curr->isFood()) {
            int distX = flame->getX() - curr->getX();
            int distY = flame->getY() - curr->getY();
            if(distX*distX + distY*distY <= (SPRITE_WIDTH*SPRITE_WIDTH/4)) {
                curr->die();
                flame->die();
                return;
            }
        }
    }

    for(list<Actor*>::iterator p = actors.begin();p != actors.end();p++) {
        Actor* curr = *p;
        int distX = flame->getX() - curr->getX();
        int distY = flame->getY() - curr->getY();
        int dist = distX*distX + distY*distY;
        if(!curr->isFood()) {
            if(curr->isDamageableObject() && dist <= SPRITE_WIDTH*SPRITE_WIDTH/4) {
                if(curr->isHPActor()) {
                    if(flame->isSpray()) curr->setHP(curr->getHP() - 2);
                    else if(flame->isFlame()) curr->setHP(curr->getHP() - 5);
                    flame->die();
                    if(curr->isSalmonella()) {
                        if(curr->getHP() > 0) playSound(SOUND_SALMONELLA_HURT);
                        else {
                            playSound(SOUND_SALMONELLA_DIE);
                            curr->die();
                            increaseScore(100);
                        }
                    }
                    else if(curr->isEColi()) {
                        if(curr->getHP() > 0) playSound(SOUND_ECOLI_HURT);
                        else{
                            playSound(SOUND_ECOLI_DIE);
                            curr->die();
                            increaseScore(100);
                        }
                    }
                    return;
                }
                else if((curr->isDirt() || curr->isFungus()) && !curr->isFood()) {
                    curr->die();
                    flame->die();
                    return;
                }
            }
        }
    }
    return;
}

void StudentWorld::socratesGetsGoodie(Goodie* goodie) {
    if(!goodie->isAlive()) return;
    int socX = player->getX(), socY = player->getY();
    int dx = socX - goodie->getX(), dy = socY - goodie->getY();
    if((dx*dx + dy*dy) <= SPRITE_WIDTH*SPRITE_WIDTH) {
        int scoreAddition = 0;
        if(goodie->isExtraLife()) {
            scoreAddition = 500;
            this->incLives();
        }
        else if(goodie->isFlamethrower()) {
            scoreAddition = 300;
            player->changeFlame(5);
        }
        else if(goodie->isFungus()) {
            scoreAddition = -50;
            player->setHP(player->getHP() - 20);
        }
        else if(goodie->isRestoreHealth()) {
            scoreAddition = 250;
            player->setHP(100);
        }
        this->increaseScore(scoreAddition);
        this->playSound(SOUND_GOT_GOODIE);
        goodie->die();
        return;
    }
    if(goodie->getLifetime() == 0) goodie->die();
    goodie->setLifetime(goodie->getLifetime() - 1);
    return;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::eColiMove(EColi* ecoli) {
    int currX = ecoli->getX(), currY = ecoli->getY();
    int playerX = player->getX(), playerY = player->getY();
    if(overlapsWithSocrates(ecoli)) {
        player->setHP(player->getHP() - 4);
        playSound((SOUND_PLAYER_HURT));
    }

    else if(ecoli->getFoodEaten() >= 3) {
        int newX, newY;

        if(currX > VIEW_WIDTH/2) newX = currX - SPRITE_WIDTH/2;
        else if(currX == VIEW_WIDTH/2) newX = currX;
        else newX = currX + SPRITE_WIDTH/2;

        if(currY > VIEW_HEIGHT/2) newY = currY - SPRITE_WIDTH/2;
        else if(currY == VIEW_HEIGHT/2) newY = currY;
        else newY = currY + SPRITE_HEIGHT/2;

        EColi* newBac = new EColi(newX, newY, this);
        addActor(newBac);
        ecoli->setFoodEaten(0);
    }

    else {
        list<Actor*>::iterator p;
        for(p = actors.begin();p != actors.end();p++) {
            if((*p)->isFood()) {
                Actor* food = *p;
                int foodX = food->getX(), foodY = food->getY();
                int dx = currX - foodX, dy = currY - foodY;
                if(dx*dx + dy*dy <= SPRITE_WIDTH*SPRITE_WIDTH) {
                    ecoli->setFoodEaten(ecoli->getFoodEaten()+1);
                    food->die();
                    break;
                }
            }
        }
    }

    double dx = currX - playerX, dy = currY - playerY;
    int socAngle;
    if(dx*dx + dy*dy <= 256*256) {
        if(dx > 0 && dy >= 0) { // correct
            socAngle = 180 + ((int)((atan(dy/dx))*(180/M_PI)));
        }
        else if(dx > 0 && dy < 0) { // correct
            socAngle = 180 + (int)((atan(dy/dx))*(180/M_PI));
        }
        else if(dx < 0 && dy >= 0) { // correct
            socAngle = (int)((atan(dy/dx))*(180/M_PI));
        }
        else if(dx < 0 && dy <= 0) { // correct
            socAngle = ((int)((atan(dy/dx))*(180/M_PI)));
        }

        else socAngle = 0;

        int tempAngle = socAngle;
        for(int i = 0;i < 10;i++) {
            double newX, newY;
            ecoli->getPositionInThisDirection(tempAngle, 2, newX, newY);
            if(!checkIfDirtNearby(newX, newY)) {
                ecoli->moveAngle(tempAngle, 2);
                return;
            }
            else {
                tempAngle = (tempAngle + 10) % 360;
            }
        }
        return;
    }
    return;
}

void StudentWorld::salmonellaMove(Salmonella *salmonella) {
    int currX = salmonella->getX(), currY = salmonella->getY();
    int playerX = player->getX(), playerY = player->getY();
    if(overlapsWithSocrates(salmonella)) {
        player->setHP(player->getHP() - 1);
        playSound(SOUND_PLAYER_HURT);
    }

    else if(salmonella->getFoodEaten() >= 3) {
        int newX, newY;

        if(currX > VIEW_WIDTH/2) newX = currX - SPRITE_WIDTH/2;
        else if(currX == VIEW_WIDTH/2) newX = currX;
        else newX = currX + SPRITE_WIDTH/2;

        if(currY > VIEW_HEIGHT/2) newY = currY - SPRITE_WIDTH/2;
        else if(currY == VIEW_HEIGHT/2) newY = currY;
        else newY = currY + SPRITE_HEIGHT/2;

        Salmonella* newBac = new Salmonella(newX, newY, this);
        addActor(newBac);
        salmonella->setFoodEaten(0);
    }

    else {
        list<Actor*>::iterator p;
        for(p = actors.begin();p != actors.end();p++) {
            if((*p)->isFood()) {
                Actor* food = *p;
                int foodX = food->getX(), foodY = food->getY();
                int dx = currX - foodX, dy = currY - foodY;
                if(dx*dx + dy*dy <= SPRITE_WIDTH*SPRITE_WIDTH/4) {
                    salmonella->setFoodEaten(salmonella->getFoodEaten()+1);
                    food->die();
                    break;
                }
            }
        }
    }

    if(salmonella->getMovementPlanDistance() > 0) {
        double newX, newY;
        salmonella->getPositionInThisDirection(salmonella->getDirection(),
                                               3, newX, newY);
        if(!isOutOfBounds(newX, newY) && !checkIfDirtNearby(newX, newY)) {
            salmonella->moveAngle(salmonella->getDirection(), 3);
        }
        else {
            salmonella->setDirection(randInt(0,359));
            salmonella->setMovementPlanDistance(10);
        }
        return;
    }

    bool isThereFood = false;
    double closestFoodX = -1, closestFoodY = -1;
    double foodDistMin = -1;
    list<Actor*>::iterator p;
    for(p = actors.begin();p != actors.end();p++) {
        Actor *temp = *p;
        if(temp->isFood()) {
            int dx = temp->getX() - salmonella->getX();
            int dy = temp->getY() - salmonella->getY();
            int dist = dx*dx + dy*dy;
            if(dist <= 128*128) {
                isThereFood = true;
                if(foodDistMin < 0 || foodDistMin > dist) {
                    foodDistMin = dist;
                    closestFoodX = temp->getX();
                    closestFoodY = temp->getY();
                }
            }
        }
    }
    if(!isThereFood) {
        salmonella->setDirection(randInt(0,359));
        salmonella->setMovementPlanDistance(10);
        return;
    }
    int dx = closestFoodX - salmonella->getX();
    int dy = closestFoodY - salmonella->getY();
    int socAngle;
    if(dx > 0 && dy >= 0) { // correct
        socAngle = 180 + ((int)((atan(dy/dx))*(180/M_PI)));
    }
    else if(dx > 0 && dy < 0) { // correct
        socAngle = 180 + (int)((atan(dy/dx))*(180/M_PI));
    }
    else if(dx < 0 && dy >= 0) { // correct
        socAngle = (int)((atan(dy/dx))*(180/M_PI));
    }
    else if(dx < 0 && dy <= 0) { // correct
        socAngle = ((int)((atan(dy/dx))*(180/M_PI)));
    }

    double newX, newY;
    salmonella->getPositionInThisDirection(socAngle, 3, newX, newY);
    if(!checkIfDirtNearby(newX, newY) && !isOutOfBounds(newX, newY)) {
        salmonella->moveAngle(socAngle, 3);
        return;
    }
    else {
        salmonella->setDirection(randInt(0,359));
        salmonella->setMovementPlanDistance(10);
        return;
    }
}

bool StudentWorld::isOutOfBounds(double x, double y) {
    int dx = x - VIEW_WIDTH/2, dy = y - VIEW_HEIGHT/2;
    return (dx*dx + dy*dy) >= VIEW_RADIUS*VIEW_RADIUS;
}

void StudentWorld::aggressiveSalmonellaMove(AggressiveSalmonella *salmonella) {
    int currX = salmonella->getX(), currY = salmonella->getY();
    int playerX = player->getX(), playerY = player->getY();
    double dx = currX - playerX, dy = currY - playerY;
    int socAngle;
    bool nearSoc = false;
    if(dx*dx + dy*dy <= 72*72) {
        nearSoc = true;
        if(dx > 0 && dy >= 0) { // correct
            socAngle = 180 + ((int)((atan(dy/dx))*(180/M_PI)));
        }
        else if(dx > 0 && dy < 0) { // correct
            socAngle = 180 + (int)((atan(dy/dx))*(180/M_PI));
        }
        else if(dx < 0 && dy >= 0) { // correct
            socAngle = (int)((atan(dy/dx))*(180/M_PI));
        }
        else if(dx < 0 && dy < 0) { // correct
            socAngle = ((int)((atan(dy/dx))*(180/M_PI)));
        }
        else socAngle = 0;
        salmonella->setDirection(socAngle);
        int newX = (int)(3*cos(salmonella->getDirection())) + currX;
        int newY = (int)(3*sin(salmonella->getDirection())) + currY;
        if(!checkIfDirtNearby(newX, newY))
            salmonella->moveAngle(salmonella->getDirection(), 3);
    }

    if(overlapsWithSocrates(salmonella)) {
        player->setHP(player->getHP() - 2);
        playSound(SOUND_PLAYER_HURT);
        if(nearSoc) return;
    }

    else if(salmonella->getFoodEaten() >= 3) {
        int newX, newY;

        if(currX > VIEW_WIDTH/2) newX = currX - SPRITE_WIDTH/2;
        else if(currX == VIEW_WIDTH/2) newX = currX;
        else newX = currX + SPRITE_WIDTH/2;

        if(currY > VIEW_HEIGHT/2) newY = currY - SPRITE_WIDTH/2;
        else if(currY == VIEW_HEIGHT/2) newY = currY;
        else newY = currY + SPRITE_HEIGHT/2;

        AggressiveSalmonella* newBac = new AggressiveSalmonella(newX, newY, this);
        addActor(newBac);
        salmonella->setFoodEaten(0);
    }

    else {
        list<Actor*>::iterator p;
        for(p = actors.begin();p != actors.end();p++) {
            if((*p)->isFood()) {
                Actor* food = *p;
                int foodX = food->getX(), foodY = food->getY();
                int dx = currX - foodX, dy = currY - foodY;
                if(dx*dx + dy*dy <= SPRITE_WIDTH*SPRITE_WIDTH) {
                    salmonella->setFoodEaten(salmonella->getFoodEaten()+1);
                    food->die();
                    break;
                }
            }
        }
    }

    if(salmonella->getMovementPlanDistance() > 0) {
        double newX, newY;
        salmonella->getPositionInThisDirection(salmonella->getDirection(),
                                               3, newX, newY);
        if(!isOutOfBounds(newX, newY) && !checkIfDirtNearby(newX, newY)) {
            salmonella->moveAngle(salmonella->getDirection(), 3);
        }
        else {
            salmonella->setDirection(randInt(0,359));
            salmonella->setMovementPlanDistance(10);
        }
        return;
    }

    bool isThereFood = false;
    double closestFoodX = -1, closestFoodY = -1;
    double foodDistMin = -1;
    list<Actor*>::iterator p;
    for(p = actors.begin();p != actors.end();p++) {
        Actor *temp = *p;
        if(temp->isFood()) {
            int dx = temp->getX() - salmonella->getX();
            int dy = temp->getY() - salmonella->getY();
            int dist = dx*dx + dy*dy;
            if(dist <= 128*128) {
                isThereFood = true;
                if(foodDistMin < 0 || foodDistMin > dist) {
                    foodDistMin = dist;
                    closestFoodX = temp->getX();
                    closestFoodY = temp->getY();
                }
            }
        }
    }

    if(!isThereFood) {
        salmonella->setDirection(randInt(0,359));
        salmonella->setMovementPlanDistance(10);
        return;
    }

    dx = -(closestFoodX - salmonella->getX());
    dy = -(closestFoodY - salmonella->getY());
    if(dx > 0 && dy >= 0) { // correct
        socAngle = 180 + ((int)((atan(dy/dx))*(180/M_PI)));
    }
    else if(dx > 0 && dy < 0) { // correct
        socAngle = 180 + (int)((atan(dy/dx))*(180/M_PI));
    }
    else if(dx < 0 && dy >= 0) { // correct
        socAngle = (int)((atan(dy/dx))*(180/M_PI));
    }
    else if(dx < 0 && dy <= 0) { // correct
        socAngle = ((int)((atan(dy/dx))*(180/M_PI)));
    }


    else socAngle = 0;
    double newX, newY;
    salmonella->setDirection(socAngle);
    salmonella->getPositionInThisDirection(socAngle, 3, newX, newY);
    if(!checkIfDirtNearby(newX, newY) && !isOutOfBounds(newX, newY)) {
        salmonella->moveAngle(socAngle, 3);
        return;
    }
    else {
        salmonella->setDirection(randInt(0,359));
        salmonella->setMovementPlanDistance(10);
        return;
    }
}

void StudentWorld::addFoodHere(int x, int y) {
    Food* newFood = new Food(x, y, this);
    addActor(newFood);
}

bool StudentWorld::overlapsWithSocrates(Actor* actor) {
    int dx = actor->getX() - player->getX();
    int dy = actor->getY() - player->getY();
    int dist = dx*dx + dy*dy;
    return dist <= SPRITE_WIDTH*SPRITE_WIDTH;
}

bool StudentWorld::checkIfDirtNearby(int x, int y) {
    for(list<Actor*>::iterator p = actors.begin(); p != actors.end();p++) {
        Actor* temp = *p;
        if(temp->isDirt()) {
            int thisX = temp->getX(), thisY = temp->getY();
            int dx = x - thisX, dy = y - thisY;
            if((dx*dx + dy*dy) <= SPRITE_WIDTH*SPRITE_WIDTH/4)
                return true;
        }
    }
    return false;
}

void StudentWorld::addSpraySocrates() {
    double locX, locY;
    player->getPositionInThisDirection(player->getDirection(), SPRITE_WIDTH, locX, locY);
    Spray* newSpray = new Spray(locX, locY, player->getDirection(), this);
    addActor(newSpray);
    playSound(SOUND_PLAYER_SPRAY);
    player->changeSpray(-1);
}

void StudentWorld::addFlameCharges() {
    for(int i = 0;i < 16;i++) {
        double locX, locY;
        player->getPositionInThisDirection(player->getDirection() + 22*i,
                                           SPRITE_WIDTH, locX, locY);
        Flame* newSpray = new Flame(locX, locY, player->getDirection() + 22*i, this);
        addActor(newSpray);

    }
    playSound(SOUND_PLAYER_FIRE);
    player->changeFlame(-1);
}
