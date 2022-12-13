#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
//many of them share things
//1. x, y coordinates,
//2. to do something (move or shoot) - during each tick
//3. annoyed (tunnelman and protesters annoyed by squirts or boulders)
//4. active or alive

const int HIT_BY_BOULDER = 0;
const int HIT_BY_SQUIRT = 1;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld *sWorld, int imageID, int x, int y, Direction dir = right, double size = 1.0, unsigned int depth = 1.0);
    virtual ~Actor();
    
    //setter
    void setDead() { m_alive = false;}
    virtual void  isAnnoyed(int p){if (points > 0) points -= p;};
    void moveTo(int moveToX, int moveToY);
//    void decPoints(int p) {if (points > 0)points -= p;}
//    bool isNear(Actor * actor, int radius) const;
    int getPoints() {return points;}
    void setPoints(int p) {points = p;}
    
    //getter
    bool isAlive() const {return m_alive;}
    void virtual doSomething() = 0; //move or shorot
    StudentWorld* getWorld() {return sWorld;}
    
private:
    int points;
    bool m_alive;
    StudentWorld *sWorld;
};

class Earth : public Actor
{
public:
    Earth(int earthX, int earthY, StudentWorld *sWorld);
    virtual ~Earth();
    void virtual doSomething(){};
};

class TunnelMan : public Actor
{
public:
    TunnelMan(StudentWorld *sWorld);
    virtual ~TunnelMan(){};
    
    virtual void moveToDirection(Direction direction);
    void doSomething();
    void squirtWater();
    void pick(int goodies);
//    virtual void isAnnoyed(int p);
    void deductAnnoyedPoints(int p);

    //Getter
    int getWater() const {return m_water;};
    int getSonar() const {return m_sonar;};
    int getGold() const {return m_gold;};
//    int getPoints() const {return points;}
    
    int getPlayer() const;
private:
    int m_water;
    int m_sonar;
    int m_gold;
//    int points;
    TunnelMan* m_player;
};


class BarrelOfOils : public Actor
{
public:
    BarrelOfOils (StudentWorld *sworld, int x, int y);
    virtual void doSomething();
};

class GoldNuggets : public Actor
{
public:
    GoldNuggets(StudentWorld *sworld, int x, int y, bool isVisible);
    virtual void doSomething();
private:
    bool isPickable ;
    int m_tick;
};

class Goods : public Actor
{
public:
    Goods(StudentWorld *sworld, int imageID, int x, int y, Direction dir, double size, unsigned int depth);
    virtual void doSomething() = 0;
};


class Sonar : public Goods
{
public:
    Sonar(StudentWorld *sWorld, int imageID, int x, int y);
    void doSomething();
private:
    int m_tick;
};

class WaterPool : public Goods
{
public:
    WaterPool(StudentWorld *sworld, int imageID, int x, int y);
    void doSomething();
private:
    int m_tick;
};


class Protester : public Actor
{
public:
    Protester(StudentWorld *sworld, int x, int y, int imageID, int hp);
    virtual ~Protester(){};
    virtual void moveToDirection(Direction direction);
    void doSomething();
    virtual void isAnnoyed(int hP);
    void protester3();
    void protester6();
    void protester7();
    bool isResting(){return(restTicks!=0);}
    bool isFacing();
    bool straightLine(Direction direction);
    void pickWhereToTurn();
    virtual void pickUpGold();
    bool getInteraction();
    bool canMoveToNewDir(Direction dir);
    
private:
//    int current_level = getWorld() -> getLevel();
    int restTicks;
    int yellTicks;
    int nonRestTicks;
    bool isLeaving;
    int numSquares;
    int m_hit_by;
};

class Regular_Protesters: public Protester
{

public:
    Regular_Protesters(StudentWorld *sworld, int x, int y);
    
};

class Hardcore_Protesters: public Protester
{
public:
    Hardcore_Protesters(StudentWorld *sworld, int x, int y);
};



class Boulder : public Actor
{
public:
    Boulder(StudentWorld *sworld, int bx, int by);
    virtual void doSomething();
    void fallDown();
    void AnnoyedToActors();
private:
    bool isStable;
    bool isFallingDown;
    int m_ticks;
};

class SquirtOfWater : public Actor
{
public:
    int m_tick;
public:
    SquirtOfWater (StudentWorld *sworld, int x, int y, Direction dir);
    virtual void doSomething();
    virtual ~SquirtOfWater() {};

};




#endif // ACTOR_H_
