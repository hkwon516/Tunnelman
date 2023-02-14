#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <algorithm>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld *world, int imageID, int x, int y, Direction dir, double size, unsigned int depth) : GraphObject(imageID, x, y, dir, size, depth), points(0), m_alive(true)
{
    sWorld = world;
    setVisible(true);
}

Actor::~Actor()
{
    setVisible(false);
}

void Actor::moveTo(int moveToX, int moveToY)
{
    if (moveToX<0)
        moveToX = 0;
    if(moveToX>60)
        moveToX = 60;
    if(moveToY<0)
        moveToY = 0;
    if(moveToY>60)
        moveToY = 60;
    GraphObject::moveTo(moveToX, moveToY);
}

//-------------------------------Earth-----------------------------------
Earth::Earth(int earthX, int earthY, StudentWorld *sWorld):Actor(sWorld, TID_EARTH, earthX, earthY, right, .25, 3) {}

Earth::~Earth() {}
//-------------------------------TunnelMan-----------------------------------
TunnelMan::TunnelMan(StudentWorld *sWorld) : Actor(sWorld, TID_PLAYER, 30, 60, right, 1.0, 0), m_water(5), m_gold(0), m_sonar(1) {setPoints(10);}

void TunnelMan::doSomething()
{
    if(!isAlive())
        return;
    
    if (getPoints() <= 0)
    {
        getWorld()->decLives();
        setPoints(10);
    }

    if(getWorld()->getLives() == 0)
    {setDead();
        return;}
    
    
    int X = getX();
    int Y = getY();
    int key = 0;
    if (getWorld() -> getKey(key) == true)
    {
        switch(key)
        {
            //end game
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            //moving direction
            case KEY_PRESS_LEFT:
                moveToDirection(left);
                getWorld()->digEarth(X - 1 , Y, left);
                break;
            case KEY_PRESS_RIGHT:
                moveToDirection(right);
                getWorld()->digEarth(X + 1, Y, right);
                break;
            case KEY_PRESS_DOWN:
                moveToDirection(down);
                getWorld()->digEarth(X , Y - 1, down);
                break;
            case KEY_PRESS_UP:
                moveToDirection(up);
                getWorld()->digEarth(X, Y + 1, up);
                break;
                
            //squirts water
            case KEY_PRESS_SPACE:
                if (m_water > 0)
                {
                    m_water--;
                    switch(getDirection())
                    {
                        case left:
                            if(!getWorld()->isEarth(getX()-4,getY())&& !getWorld()->isBoulder(getX()-4, getY()))
                            {
                                getWorld()->getActors()->push_back(new SquirtOfWater(getWorld(), getX() - 4, getY(), getDirection()));
                            }
                            break;
                        case right:
                            if(!getWorld()->isEarth(getX() + 4,getY())&& !getWorld()->isBoulder(getX()+4, getY()))
                            {
                                getWorld()->getActors()->push_back(new SquirtOfWater(getWorld(), getX() + 4, getY(), getDirection()));
                            }
                            break;
                        case up:
                            if(!getWorld()->isEarth(getX(),getY() + 4)&& !getWorld()->isBoulder(getX(), getY() + 4))
                            {
                                getWorld()->getActors()->push_back(new SquirtOfWater(getWorld(), getX() , getY() + 4, getDirection()));
                            }
                            break;
                        case down:
                            if(!getWorld()->isEarth(getX(),getY() - 4)&& !getWorld()->isBoulder(getX(), getY() - 4))
                            {
                                getWorld()->getActors()->push_back(new SquirtOfWater(getWorld(), getX() , getY() - 4, getDirection()));
                            }
                            break;
                        case none:
                        default:
                            break;
                    }
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                }
                break;
            case KEY_PRESS_TAB:
                    getWorld()->getActors()->push_back(new GoldNuggets(getWorld(), getX(), getY(), true));
                    if (m_gold > 0){m_gold--;}

                break;
            default:
                moveToDirection(none);
                break;

            case 'Z':
            case 'z':
                getWorld()->getActors()->push_back(new Sonar(getWorld(), 0, 60, true));
                if (m_sonar > 0)
                {
                    m_sonar--;
                }
                else break;
                break;
            
        }
    }
}

void TunnelMan::moveToDirection(Direction direction)
{
    int X = getX();
    int Y = getY();
    Direction movingDir = getDirection();
    
    switch(direction)
    {
        case left:
            if (movingDir == left)
            {
                if (!getWorld()->isBoulder(X - 1, Y))
                    moveTo(X - 1, Y);
                else
                    moveTo(X,Y);
            }
            else setDirection(left);
            break;
        case right:
            if (movingDir == right)
            {
                if (!getWorld()->isBoulder(X + 1, Y))
                    moveTo(X + 1, Y);
                else
                    moveTo(X,Y);
            }
            else setDirection(right);
            break;
        case up:
            if (movingDir == up)
            {
                if (!getWorld()->isBoulder(X , Y + 1))
                    moveTo(X, Y + 1);
                else
                    moveTo(X,Y);
            }
            else setDirection(up);
            break;
        case down:
            if (movingDir == down)
            {
                if (!getWorld()->isBoulder(getX() , getY() - 1))
                    moveTo(X, Y - 1);
                else
                    moveTo(X,Y);
            }
            else setDirection(down);
            break;
        case none:
        default:
            return;
    }
}

void TunnelMan::pick(int goodies)
{
    if (goodies == TID_GOLD)
        m_gold++;
    if (goodies == TID_WATER_POOL)
        m_water += 5;
    if (goodies == TID_SONAR)
        m_sonar += 2;
}

//-------------------------------Boulder----------------------
Boulder::Boulder(StudentWorld *sworld, int bx, int by):Actor(sworld, TID_BOULDER, bx, by, down, 1.0, 1)
{
    isStable = true;
    m_ticks = 0;
    //check if the direction should be none
    sworld->digEarth(bx, by, none);
}

void Boulder::doSomething()
{
    if(!isAlive())
        return;
    
    if (isStable && getWorld() -> checkIfAboveEarth(getX(), getY() - 1))    //stable
    {
        return;
    }
    else if (isStable)
    {
        isStable = false; //waiting
    }
    else    //falling
    {
        if(m_ticks >= 30)
        {
            isFallingDown = true;
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }
    }
    m_ticks++;
    if (isFallingDown)  //falling
    {
        fallDown();
        AnnoyedToActors();
    }
}

void Boulder::fallDown()
{
    //A. set dead if
    //1. hits the bottom of the oil field
    //2. runs into the top of another boulder
    //3. runs into earth
    if (getY() == 0 || getWorld()->isBoulder(getX(), getY() - 4, 0) || getWorld()->checkIfAboveEarth(getX(), getY() - 1))
    {
        setDead();
    }
    else
    {
        moveTo(getX(), getY() - 1);
    }
}

void Boulder::AnnoyedToActors() //protesters & tunnelman applies the same
{
    //1. if boulder comes within radius of 3 - 100 points deducted
    //2. if annoys tunnelman instantly lose a life
    if (getWorld()->isPlayerAndBoulderInRadius(getX(),getY(), 3))
        getWorld()->getPlayer()->isAnnoyed(100);
    
    //player
    TunnelMan *t = getWorld()->getPlayer();
    int tx = t -> getX();
    int ty = t -> getY();
    
    //protester
    int px, py ;
    
    double x, y;
    std::vector<Protester*>::iterator itr;
    for (itr = getWorld()->getProtester().begin(); itr != getWorld()->getProtester().end(); itr++)
    {
        px = (*itr) -> getX();
        py = (*itr) -> getY();
        
        x = getX() - px;
        y = getY() - py;
        
        if (x * x + y * y <= 3)
        {
            (*itr)->isAnnoyed(100);
        }
    }
    
    x = getX() - tx;
    y = getY() - ty;
    if (x * x + y * y <= 3)
    {
        t->setDead();
    }
}
//-------------------------------Squirts----------------------
SquirtOfWater::SquirtOfWater(StudentWorld *sworld, int x, int y, Direction dir) : Actor(sworld, TID_WATER_SPURT, x, y, dir, 1.0, 1), m_tick(0)
{
}

void SquirtOfWater::doSomething()
{
    Direction dir = getDirection();
    int X = getX();
    int Y = getY();
    
    if (!isAlive())
        return;

    Protester *p = getWorld()-> protesterInRadius3(this, 3);
    if (p != nullptr)
    {
        if (m_tick == 4)
        {
            p->isAnnoyed(2);
            setDead();
            return;
        }
    }
    
    switch(dir)
    {
        case left:
            if (getWorld()->isEarth(X - 1, Y) || getWorld() ->isBoulder(X - 1, Y))
                {
                setDead();
                return;
            }
            else moveTo(X - 1, Y);
            break;
        case right:
            if (getWorld()->isEarth(X + 1, Y) || getWorld() ->isBoulder(X + 1, Y) || (getX() == 60 && getY() == 60))
                {
                setDead();
                return;
            }
            else moveTo(X + 1, Y);
            break;
        case up:
            if (getWorld()->isEarth(X, Y + 1) || getWorld() ->isBoulder(X , Y + 1))
                {
                setDead();
                return;
            }
            else moveTo(X, Y + 1);
            break;
        case none:
        default:
            return;
                
    }
    m_tick++;
}
//-------------------------------Barrel----------------------
BarrelOfOils::BarrelOfOils(StudentWorld *sworld, int x, int y) : Actor(sworld, TID_BARREL, x, y, right, 1.0, 2)
{
    setVisible(false);
}

void BarrelOfOils::doSomething()
{
    if (!isAlive())
        return;
    if (!isVisible() && getWorld()->isPlayerInRadius4(this))
    {
        setVisible(true);
        return;
    }
    if (getWorld()->isPlayerInRadius3(this))
    {
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->incScores(1000);
        getWorld()->decBarrels();
        return;
    }
}

//-------------------------------Gold--------------------------
GoldNuggets::GoldNuggets (StudentWorld *sworld, int x, int y, bool isVisible):Actor(sworld, TID_GOLD, x, y)
{
//    isPickable = true;
    setVisible(false);
}

void GoldNuggets::doSomething()
{
    if (!isAlive())
        return;
    else if (!isVisible() )
    {
        if (getWorld()->isPlayerInRadius4(this))
        {
            setVisible(true);
            return;
        }
    }
    else if (isPickable == true && getWorld()->isPlayerInRadius3(this))
    {
        
        setDead();
        setVisible(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->pick(getID());
        getWorld()->incScores(10);
        return;
    }
    else if (isPickable == false && getWorld()->protesterInRadius3(this, 3))
    {
        Protester *p = getWorld()->protesterInRadius3(this, 3);

        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        setDead();
        if (p != nullptr)
        {
            p -> pickUpGold();

        }
        getWorld()->incScores(25);

        if (m_tick == 100)
        {
            setDead();
        }
        else m_tick++;
        
    }
    
}
//-------------------------------Goodies----------------------
Goods::Goods(StudentWorld *sworld, int imageID, int x, int y, Direction dir, double size, unsigned int depth):Actor(sworld, imageID, x, y, dir, size, depth)
{
    
}
//-------------------------------Sonar----------------------
Sonar::Sonar(StudentWorld *sworld, int imageID, int x, int y):Goods(sworld, TID_SONAR, x, y, right, 1.0, 2)
{
    m_tick = std::max(100, 300 - 10 * (int(getWorld()->getLevel())));
}

void Sonar::doSomething()
{
    if (!isAlive())
        return;
    m_tick--;
    if (m_tick <= 0)
    {
        setDead();
    }
    if (getWorld()->isPlayerInRadius3(this))
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->pick(getID());
        getWorld()->incScores(75);
    }
}
//-------------------------------Water Pool----------------------
WaterPool::WaterPool(StudentWorld *sworld, int imageID, int x, int y):Goods(sworld, TID_SONAR, x, y, right, 1.0, 2)
{
    m_tick = std::max(100, 300 - 10 * (int(getWorld()->getLevel())));
    
}
void WaterPool::doSomething()
{
    if (!isAlive())
        return;
    m_tick--;
    if (m_tick <= 0)
    {
        setDead();
    }
    if (getWorld()->isPlayerInRadius3(this))
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->pick(getID());
        getWorld()->incScores(500);
    }
}
//-------------------------------Protester----------------------
Protester::Protester(StudentWorld *sworld, int x, int y, int imageID, int hp):Actor(sworld, imageID, x, y, left, 1.0, 0), yellTicks(15), nonRestTicks(200)
{
    isLeaving = false;
    int current_level = getWorld() -> getLevel();
//    if (nonRestTicks == 0)
    restTicks = max(0, 3 - current_level / 4);
    numSquares = getWorld() -> getRandInt(8, 60);
}

void Protester::doSomething()
{
    //1
    if (!isAlive())
        return;
    
    //2
    if (isResting())    //resting
    {
        restTicks--;
        return;
    }
    else
    {
        restTicks = max(0, 3 - (int)getWorld()->getLevel() / 4);
        nonRestTicks++;
        yellTicks++;
    }
    protester3();
    
    //4
    if (getWorld()->isPlayerInRadius4(this)&& isFacing())
    {
        if (yellTicks >= 15)
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->getPlayer()->isAnnoyed(2);
//            TunnelMan* t = getWorld()->getPlayer();
            
            yellTicks = 0;
            return;
        }
        return;
    }
    restTicks--;
    //5
    if ( getDirection() != none && straightLine(getDirection()) && !getWorld()->isPlayerInRadius4(this))
    {
        moveToDirection(getDirection());
        numSquares = 0;
    }

    protester6();
    protester7();
    
    //8.
    if (canMoveToNewDir(getDirection()))
        moveToDirection(getDirection());
    //9.
    if (!getWorld()->checkIfCanMove(getX(), getY(), getDirection()))
    {
        restTicks = 0;
    }

}

bool Protester::canMoveToNewDir(Direction dir)
{
    int x = getX();
    int y = getY();
    
    // bounds check;
    if (x == 0)
    {
        if (dir == left) return false;
    }
    else if ( x >= 60)
    {
        if (dir == right) return false;
    }
    else if ( y == 0)
    {
        if (dir == down) return false;
    }
    else if ( y >= 60)
    {
        if (dir == up) return false;
    }
    return false;
}

void Protester::protester3()
{
    if (isLeaving)  //is at leave-the-oil-field state
    {
        if (getX() == 60 && getY() == 60) //a
        {
            setDead();
            getWorld()->decProtesterPoints();
            return;
        }
        else
        {
            getWorld()->moveCloserToExit(this); //b
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            return; //c
        }
    }
}

bool Protester::isFacing()
{
    int currPlayerX = getWorld()->getPlayer()->getX();
    int currPlayerY = getWorld()->getPlayer()->getY();

    if (getDirection() == left)
        return currPlayerX  <= getX();
    else if (getDirection() == right)
        return currPlayerX >= getX();
    else if (getDirection() == up)
        return currPlayerY >= getY();
    else if (getDirection() == down)
        return currPlayerY  <= getY();
    else
        return false;
}


void Protester::protester6()
{
    if (numSquares <= 0)
    {
        //a. The Regular Protester will pick a random new direction to move (up, down, left or right).
        Direction dir = none;
        int rd = rand() % 4;
        if (rd == 0) dir = left;
        else if (rd == 1) dir = right;
        else if (rd == 2) dir = up;
        else dir = down;

        
        if (getWorld()->checkIfCanMove(getX(), getY(), dir))
        {setDirection(dir);
            numSquares = getWorld()->getRandInt(8, 60);}
            
        
    }
}
bool Protester::getInteraction()
{
    if (getDirection() == up || getDirection() == down) {
        return (getWorld()->checkIfCanMove(getX(), getY(), left) || getWorld()->checkIfCanMove(getX(), getY(), right));
        }
        else
            return (getWorld()->checkIfCanMove(getX(), getY(), up) || getWorld()->checkIfCanMove(getX(), getY(), down));
        
}

void Protester::pickWhereToTurn()
{
    if (getDirection() == up || getDirection() == down) {
        if (!getWorld()->checkIfCanMove(getX(),getY(), left)) setDirection(right);
        else if (!getWorld()->checkIfCanMove(getX(),getY(), right)) setDirection(left);
        else {
            switch (rand() % 2) {
            case 0: setDirection(left);
            case 1: setDirection(right);
            }
        }
    }
    else {
        if (!getWorld()->checkIfCanMove(getX(),getY(), up)) setDirection(down);
        else if (!getWorld()->checkIfCanMove(getX(),getY(), down)) setDirection(up);
        else {
            switch (rand() % 2) {
            case 0: setDirection(up);
            case 1: setDirection(down);
            }
        }
    }

}
void Protester::protester7()
{
    if (getInteraction() && restTicks > 200)
    {
        pickWhereToTurn();
        nonRestTicks = 0;
        restTicks = getWorld()->getRandInt(8, 60);
    }
}

bool Protester::straightLine(Direction dir)
{
    int x = getWorld()->getPlayer()->getX();
    int y = getWorld()->getPlayer()->getY();
    
    switch(dir)
    {
        case left:
            for (int i = getX(); i >= x; i--)
            {
                if (getWorld()->isEarth(i, getY()) || getWorld()->isBoulder(i, getY()))
                    return false;
            }
            return true;
        case right:
            for (int i = getX(); i <= x; i++)
            {
                if (getWorld()->isEarth(i, getY()) || getWorld()->isBoulder(i, getY()))
                    return false;
            }
            return true;
        case up:
            for (int j = getY(); j <= y; j++)
            {
                if (getWorld()->isEarth(getX(), j) || getWorld()->isBoulder(getX(), j))
                    return false;
            }
            return true;
        case down:
            for (int j = getX(); j >= y; j--)
            {
                if (getWorld()->isEarth(getX(), j) || getWorld()->isBoulder(getX(), j))
                    return false;
            }
            return true;
        case none:
            return false;
    }
}

void Protester::isAnnoyed(int hp)
{
    int lv = getWorld()->getLevel();
    if (isLeaving)
        return;
    
    isAnnoyed(hp);
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    restTicks = max(50, 100 - lv * 100);
    
    if (hp <= 0)
    {
        getWorld() -> playSound(SOUND_PROTESTER_ANNOYED);
        isLeaving = true;
        restTicks = 0;
        
        if (hp == 100)
            getWorld()->incScores(500);
        else if (getID()==TID_PROTESTER)
            getWorld()->incScores(100);
        else
            getWorld()->incScores(250);
    }
}


void Protester::moveToDirection(Direction dir)
{
    int X = getX();
    int Y = getY();
    Direction movingDir = getDirection();

    switch(dir)
    {
        case left:
            if(movingDir == left)
            {
                if (X != 0)
                    moveTo(X - 1, Y);
                else
                    setDirection(left);
            }
            else setDirection(left);
            break;
        case right:
            if(movingDir == right)
            {
                if (X != 0)
                    moveTo(X + 1, Y);
                else
                    setDirection(right);
            }
            else setDirection(right);
            break;
        case up:
            if(movingDir == up)
            {
                if (Y != 0)
                    moveTo(X , Y + 1);
                else
                    setDirection(up);
            }
            else setDirection(up);
            break;
        case down:
            if(movingDir == down)
            {
                if (Y != 0)
                    moveTo(X , Y - 1);
                else
                    setDirection(down);
            }
            else setDirection(down);
            break;
        case none:
            return;
    }
}

void Protester::pickUpGold()
{
    if(getID()==TID_PROTESTER)
    {
        getWorld()->incScores(25);
        isLeaving = true;
    }
    else
    {
        getWorld()->incScores(50);
        restTicks = max(50,100- int(getWorld()->getLevel())*10);
    }
}



