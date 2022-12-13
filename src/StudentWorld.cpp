#include "StudentWorld.h"
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir), m_protesterPoints(0), isFirstTick(true),scores(0),m_lastTick(0), m_barrels(0), points(0), m_gold(0), m_water(0), m_sonar(0){}

//-----------------------Initialize Function-------------------------
int StudentWorld::init()
{
    isFirstTick = true;
    m_protesterPoints = 0;
    m_lastTick = 0;
    m_protesterPoints = 0;
    m_barrels = 0;
    m_gold = 0;

    init_Earths();
    init_TunnelMan();
    init_Boulders();
    init_Barrels();
    init_Golds();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::init_Earths()
{
    for (int c = 0; c < VIEW_WIDTH; c++)
    {
        std::vector<Earth*> e;
        for (int r = VIEW_HEIGHT - 4; r < VIEW_HEIGHT; r++)
        {
            earth[c][r] = nullptr;
        }
        for (int r = 0; r < VIEW_HEIGHT - 4; r++)
        {
            if (c < 30 || c > 33 || r < 4)
                earth[c][r] = new Earth(c, r, this);
            else
                earth[c][r] = nullptr;
        }
    }
}

void StudentWorld::init_TunnelMan()
{
    m_player = new TunnelMan(this);
}

void StudentWorld::init_Boulders()
{
    
    int B = min(int(current_level_number/2) + 2, 9);
    
    int x, y;
    for (int i = 0; i < B; i++)
    {
        do
        {
            x = rand() % 60 + 1;
            y = rand() % 56 + 1;
        }
        while(isPlayerAndBoulderInRadius(x, y, 6) || (x > 26 && x < 34 && y > 0));
        
        digEarth(x, y, Actor::none);
        m_actor.push_back(new Boulder(this, x, y));
    }
}

void StudentWorld::init_Barrels()
{
    int x , y;
    int L = min(2 + current_level_number, 20);
    
    for (int i = 0; i < L ; i++)
    {
        do {
            x = getRandInt(1, 60);
            y = getRandInt(1, 56);
        }while(isGoodiesInRadius(x, y, 6) || (x > 26 && x < 34 && y>0));
        
        m_actor.push_back(new BarrelOfOils(this, x, y));
        incBarrels();
    }
}

void StudentWorld::init_Golds()
{
    int x , y;
    int G = max(current_level_number/2, 2);
    for (int i = 0; i < G ; i++)
    {
        do {
            x = getRandInt(1, 60);
            y = getRandInt(1, 56);
        }while(isGoodiesInRadius(x, y, 6) || (x > 26 && x < 34 && y>0));
        m_actor.push_back(new GoldNuggets(this, x, y, false));
        incGolds(1);
    }
}

void StudentWorld::init_Protesters()
{
    int T = max(25, 200 - current_level_number);
    int P = fmin(15, 2 + current_level_number * 1.5);
    int probabilityOfHardcore = min(90, current_level_number * 10 + 30);
    if ((m_lastTick > T && m_protesterPoints < P)||isFirstTick)
    {
        if (getRandInt(1, 100) < probabilityOfHardcore)
        {
            Hardcore_Protesters *hp = new Hardcore_Protesters(this, 60, 60);
            m_actor.push_back(hp);
        }
        else
        {
            Regular_Protesters *rp = new Regular_Protesters(this, 60, 60);
            m_actor.push_back(rp);
        }
        m_lastTick = 0;
        m_protesterPoints ++;
        isFirstTick = false;
    }
    m_lastTick++;
}


//-----------------------Earth-------------------------
bool StudentWorld::digEarth(int moveX, int moveY, GraphObject::Direction dir)
{
    bool isDigged = false;
    for (int i = moveX; i < moveX + 4; i++)
    {
        for (int j = moveY; j < moveY + 4; j++)
        {
            if (i < 0 || i >= VIEW_WIDTH || j < 0 || j >= VIEW_HEIGHT)
                return false;
            else
            {
                if (earth[i][j] != nullptr)
                {
                    delete earth[i][j];
                    earth[i][j] = nullptr;
                    
                    isDigged = true;
                    playSound(SOUND_DIG);
                }
            }
        }
    }
    return isDigged;
}

bool StudentWorld::isEarth(int x, int y) const
{
    if (earth[x][y] != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::checkIfAboveEarth(int x, int y) const
{
    for (int i = 0; i < 4; i++)
    {
        if (isEarth(x + i, y - 1))
            return true;
    }
    return false;
}

bool StudentWorld::isBoulder(int x, int y, int radius)
{
    vector<Actor*>::iterator itr;
    for (itr = m_actor.begin(); itr != m_actor.end(); itr++)
    {
        if ((*itr)->getID() == TID_BOULDER && isInRadius(x, y, (*itr)->getX(), (*itr)->getY(), radius))
            return true;
    }
    return false;
}



bool StudentWorld::isInRadius(int moveX, int moveY, int x, int y, int radius) const
{
    int row = (moveX - x);
    int col = (moveY - y);
    
    return (row * row + col * col <= radius * radius);
}

bool StudentWorld::isPlayerInRadius4(Actor *actor) const
{
    return isInRadius(actor->getX(), actor->getY(), m_player->getX(), m_player->getY(), 4);
}

bool StudentWorld::isPlayerInRadius3(Actor *actor) const
{
    return isInRadius(actor->getX(), actor->getY(), m_player->getX(), m_player->getY(), 3);
}
bool StudentWorld::isPlayerAndBoulderInRadius (int x, int y, int radius) const
{
    int currX = m_player->getX();
    int currY = m_player->getY();
    
    return (isInRadius(currX, currY, x, y, radius));
}

bool StudentWorld::isGoodiesInRadius(int x, int y, int radius)
{
   vector<Actor*>::iterator it;
   it = m_actor.begin();
   while (it != m_actor.end()){
       if (isInRadius(x,y, (*it)->getX(),(*it)->getY() ,radius))
           return true;
       it++;
   }
   return false;
}

Protester* StudentWorld::protesterInRadius3(Actor * actor, int radius)
{
        vector<Actor*> * aP = getActors();
        for (auto& atr : *aP)
        {
            if ((atr)->getID() == TID_PROTESTER || (atr)->getID() == TID_HARD_CORE_PROTESTER)
                if (isInRadius(actor->getX(), actor->getY(), (atr)->getX(), (atr)->getY(), radius))
                    return dynamic_cast<Protester*> (atr);
        }
        return nullptr;
}

int StudentWorld::getRandInt(int a, int b){
    int randint = rand() % (b - a + 1) + a;
    return randint;
}

void StudentWorld::moveCloserToExit(Protester *p)
{
    //queue based maze
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 64; j++)
            checkvisited[i][j] = 0;
    
    queue<Square> s;
    s.push(Square(60,60));
    checkvisited[60][60] = 1; //check visited excpet the last square
    
    while(!s.empty())
    {
        Square a = s.front();
        s.pop();
        int x = a.x;
        int y = a.y;
     
        if (checkIfCanMove(x, y, GraphObject::left)&&checkvisited[x - 1][y] == 0)
        {
            s.push(Square(x - 1, y));
            checkvisited[x - 1][y] = checkvisited[x][y] + 1;
        }
        if (checkIfCanMove(x, y, GraphObject::left)&&checkvisited[x - 1][y] == 0)
        {
            s.push(Square(x - 1, y));
            checkvisited[x - 1][y] = checkvisited[x][y] + 1;
        }
        if (checkIfCanMove(x, y, GraphObject::left)&&checkvisited[x - 1][y] == 0)
        {
            s.push(Square(x - 1, y));
            checkvisited[x - 1][y] = checkvisited[x][y] + 1;
        }
        if (checkIfCanMove(x, y, GraphObject::left)&&checkvisited[x - 1][y] == 0)
        {
            s.push(Square(x - 1, y));
            checkvisited[x - 1][y] = checkvisited[x][y] + 1;
        }
    }
    int X = p->getX();
    int Y = p->getY();
    if (checkIfCanMove(X, Y, GraphObject::left) && checkvisited[X - 1][Y] < checkvisited[X][Y])
        p -> moveToDirection(GraphObject::left);
    if (checkIfCanMove(X, Y, GraphObject::right) && checkvisited[X + 1][Y] < checkvisited[X][Y])
        p -> moveToDirection(GraphObject::right);
    if (checkIfCanMove(X, Y, GraphObject::up) && checkvisited[X][Y + 1] < checkvisited[X][Y])
        p -> moveToDirection(GraphObject::up);
    if (checkIfCanMove(X, Y, GraphObject::down) && checkvisited[X][Y - 1] < checkvisited[X][Y])
        p -> moveToDirection(GraphObject::down);
    return;
}

bool StudentWorld::checkIfCanMove(int x, int y, GraphObject::Direction dir)
{
    switch (dir) {
        case GraphObject::left:
            return (x!=0&&!isEarth(x - 1, y) && !isBoulder(x, y));
        case GraphObject::right:
            return (x!=60&& !isEarth(x + 1, y) && !isBoulder(x + 1, y));
        case GraphObject::up:
            return (y!=60&&!isEarth(x, y + 1) && !isBoulder(x, y + 1));
        case GraphObject::down:
            return (y!=0&& !isEarth(x, y - 1) && !isBoulder(x, y - 1));
        case GraphObject::none:
                return false;
        }
        return false;
}

//-----------------------Move Function-------------------------
int StudentWorld::move()
{
    setDisplayText();
    vector<Actor*>::iterator itr;
    for (itr = m_actor.begin(); itr!= m_actor.end(); itr++)
    {
        if ((*itr) -> isAlive())
            (*itr) -> doSomething();
        else
            return GWSTATUS_PLAYER_DIED;
    }
    
    m_player -> doSomething();  //moves direction
    init_Protesters();
    
    // remove dead actors
    for (itr = m_actor.begin(); itr != m_actor.end();) {
        if (!(*itr)->isAlive()) {
            delete *itr;
            itr = m_actor.erase(itr);
        }
        else itr++;
    }
    
    if (!m_player -> isAlive())
    {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (m_barrels == 0)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

//-----------------------CleanUp Function-------------------------
StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::cleanUp()
{
    destruct_Earth();
    destruct_Actor();
}

void StudentWorld::destruct_Earth()
{
    for (int c = 0; c < VIEW_WIDTH; c++)
    {
        for (int r = 0; r < VIEW_HEIGHT - 4; r++)
        {
            if (earth[c][r] != nullptr)
                delete earth[c][r];
        }
    }
}

void StudentWorld::destruct_Actor()
{
    delete m_player;
    
    vector<Actor*>::iterator itr;
    for (itr = m_actor.begin();itr != m_actor.end();itr++)
    {
        delete *itr;
    }
    m_actor.clear();
    
}

void StudentWorld::setDisplayText()
{
    int lv = getLevel();
    int lives = getLives();
    int points = m_player -> getPoints();
    int health = points * 100 / 10;
    int water = m_player -> getWater();
    int gold = m_player -> getGold();
    int barrel = getBarrels();
    int sonar = m_player -> getSonar();
    int scores = getScores();
    
    ostringstream oss;
    oss.setf(ios::right);
    oss << "Lvl: " << setw(2) << lv;
    oss << "  Lives: " << setw(1) << lives;
    oss << "  Hlth: " << setw(3) << health << "%";
    oss << "  Wtr: " << setw(2) << water;
    oss << "  Gld: " << setw(2) << gold;
    oss << "  Oil Left: " << setw(2) << barrel;
    oss << "  Sonar: " << setw(2) << sonar;
    
    oss.fill('0');
    oss << "  Scr: " << setw(6) << scores;
    
    
    setGameStatText(oss.str());
}



// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

