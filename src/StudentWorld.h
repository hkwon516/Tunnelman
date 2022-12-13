#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "GameController.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class GraphObject;
class Earth;
class TunnelMan;
class Protesters;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    virtual void cleanUp();
    
    //check if there is Boulder in radius
    bool isBoulder(int x, int y, int radius = 3);
    //check if surrounding
//    void isGameObjectsinRadius(int x, int y, int radius);
    void moveCloserToExit(Protester *p);
    
    bool digEarth(int moveX, int moveY, GraphObject::Direction dir);
    bool isEarth(int x, int y) const;
    bool checkIfAboveEarth(int x, int y) const;
    bool checkIfCanMove(int x, int y, GraphObject::Direction dir);
    
    bool isInRadius(int moveX, int moveY, int x, int y, int radius) const;
    bool isPlayerAndBoulderInRadius (int x, int y, int radius) const;
    bool isPlayerInRadius4 (Actor *actor) const;
    bool isPlayerInRadius3(Actor *actor) const;
    bool isGoodiesInRadius(int x, int y, int radius);
    Protester* protesterInRadius3(Actor *actor, int radius);
    
    TunnelMan* getPlayer(){return m_player;};
    
    void setDisplayText();
    int getRandInt(int a, int b);
    void decProtesterPoints(){m_protesterPoints--;}
    
    std::vector<Protester*> getProtester(){return m_protester;}
    
    std::vector<Actor*> * getActors() { return &m_actor; }
    
    
    int getPoints() const {return points;}
    void incPoints(int p) { points += p;}
    void decPoints(int p) { points -= p;}
    
    int getBarrels() const {return m_barrels;}
    void incBarrels() {m_barrels++;}
    void decBarrels() {m_barrels--;}
    
    int getGolds() const {return m_gold;}
    void incGolds(int g) {m_gold += g;}
    void decGolds(int g) {m_gold--;}
    
    int getScores() const {return scores;}
    void incScores(int s) {scores += s;}
    
    int getWater() const {return m_water;}
    void incWater(int w) {m_water += w;}
    
    int getSonar() const {return m_sonar;}
    void incSonar(int sr) {m_sonar += sr;}
    
    void detectNearActors(int x, int y, int radius);
    
    int current_level_number = getLevel();
    
private:
    //initialize functions
    void init_Earths();
    void init_TunnelMan();
    void init_Boulders();
    void init_Barrels();
    void init_Golds();
    void init_Protesters();
    void init_Sonar();
    
    int m_protesterPoints;
    int m_lastTick;
    
    bool isFirstTick;
    int m_barrels;
    int points;
    int scores;
    int m_gold;
    int m_water;
    int m_sonar;
    
    
//    void init_Barrels();
//    void init_Sonar();
    
    
    void destruct_Earth();
    void destruct_Actor();
    
    Earth* earth[VIEW_WIDTH][VIEW_HEIGHT];
    TunnelMan* m_player;
    std::vector<Actor*> m_actor;
    std::vector<Protester*> m_protester;
    
    
    int checkvisited[64][64];
    struct Square
    {
        int x, y;
        Square(int a, int b): x(a), y(b){}
    };
};

#endif // STUDENTWORLD_H_
