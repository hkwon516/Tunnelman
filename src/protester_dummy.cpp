////-------------------------------Protester----------------------
//Protester::Protester(StudentWorld *sworld, int x, int y, int imageID, int hp):Actor(sworld, imageID, x, y, left, 1.0, 0), yellTicks(15), nonRestTicks(200)
//{
//    isLeaving = false;
//    
//    int current_level = getWorld() -> getLevel();
//    restTicks = max(0, 3 - current_level / 4);
//    
//    numSquares = getWorld() -> getRandInt(8, 60);
//}
//
//void Protester::doSomething()
//{
//    //1
//    if (!isAlive())
//        return;
//    
//    protester3();
//    protester6();
//
//    //8.
//    moveToDirection(getDirection());
//    //9.
//    if (!getWorld()->checkIfCanMove(getX(), getY(), getDirection()))
//    {
//        restTicks = 0;
//    }
//    
//    //2
//    if (isResting())    //resting
//    {
//        restTicks--;
//        return;
//    }
//    else
//    {
//        restTicks = max(0, 3 - (int)getWorld()->getLevel() / 4);
//        nonRestTicks++;
//        yellTicks++;
//    }
//    if (isLeaving)
//    {
//        //4
//        if (getWorld()->isPlayerInRadius4(this) )
//        {
//            if (yellTicks >= 15 && isFacing())
//            {
//                getWorld()->playSound(SOUND_PROTESTER_YELL);
//                getWorld()->getPlayer()->isAnnoyed(2);
//                yellTicks = 0;
//                return;
//            }
//        }
//        //5
//        else if ( getDirection() != none && straightLine(getDirection()) && !getWorld()->isPlayerInRadius4(this))
//        {
//            moveToDirection( getDirection());
//            numSquares = 0;
//        }
//        restTicks++;
//        nonRestTicks++;
//    }
//    
//    protester7();
//    
//}
//
//
//void Protester::protester3()
//{
//    if (isLeaving)  //is at leave-the-oil-field state
//    {
//        if (getX() == 60 && getY() == 60) //a
//        {
//            setDead();
//            getWorld()->decProtesterPoints();
//            return;
//        }
//        else
//        {
//            getWorld()->moveCloserToExit(this); //b
//            return; //c
//        }
//    }
//}
//
//bool Protester::isFacing()
//{
//    int currPlayerX = getWorld()->getPlayer()->getX();
//    int currPlayerY = getWorld()->getPlayer()->getY();
//    
//    int px = getX();
//    int py = getY();
//    Direction pDir = getDirection();
//    
//    if (py>= currPlayerY -3 && py <= currPlayerY +3)
//    {
//        if      (currPlayerX>px && pDir == right) return true;
//        else if (currPlayerX<px && pDir == left)  return true;
//    }
//    if (px>= currPlayerX-3 && px <= currPlayerX +3)
//    {
//        if      (currPlayerX>py && pDir == up)    return true;
//        else if (currPlayerX<py && pDir == down)  return true;
//    }
//    
//    return false;
//}
//
//void Protester::protester6()
//{
//    if (numSquares <= 0)
//    {
//        //a. The Regular Protester will pick a random new direction to move (up, down, left or right).
//        Direction dir = none;
//        int rd = rand() % 4;
//        if (rd == 0) dir = left;
//        else if (rd == 1) dir = right;
//        else if (rd == 2) dir = up;
//        else dir = left;
//            
//        
//        if (getWorld()->checkIfCanMove(getX(), getY(), dir))
//        {setDirection(dir);
//            numSquares = getWorld()->getRandInt(8, 60);}
//        
//    }
//}
//
//void Protester::protester7()
//{
//    if (getDirection() ==up || getDirection() == down)
//    {
//        if ((getWorld()->checkIfCanMove(getX(), getY(), left) || getWorld()->checkIfCanMove(getX(), getY(), right)) && restTicks >= 200)
//        {
//            if (getWorld()->checkIfCanMove(getX(), getY(), left) == false)
//                setDirection(left);
//            else if (getWorld()->checkIfCanMove(getX(), getY(), right) == false)
//                setDirection(right);
//            else    //random
//            {
//                int rd = rand() % 2;
//                if (rd == 0)
//                    setDirection(right);
//                else
//                    setDirection(left);
//            }
//            
//            nonRestTicks = 0;
//            restTicks = getWorld()->getRandInt(8, 60);
//        }
//    }
//    if (getDirection()==left || getDirection() == right)
//    {
//        if ((getWorld()->checkIfCanMove(getX(), getY(), up) || getWorld()->checkIfCanMove(getX(), getY(), down)) && restTicks > 200)
//        {
//            if (getWorld()->checkIfCanMove(getX(), getY(), up) == false)
//                setDirection(up);
//            else if (getWorld()->checkIfCanMove(getX(), getY(), down) == false)
//                setDirection(down);
//            else    //random
//            {
//                int rd = rand() % 2;
//                if (rd == 0)
//                    setDirection(down);
//                else
//                    setDirection(up);
//            }
//        }
//        nonRestTicks = 0;
//        restTicks = getWorld()->getRandInt(8, 60);
//    }
//}
//
//bool Protester::straightLine(Direction dir)
//{
//    int x = getWorld()->getPlayer()->getX();
//    int y = getWorld()->getPlayer()->getY();
//    
//    switch(dir)
//    {
//        case left:
//            for (int i = getX(); i >= x; i--)
//            {
//                if (getWorld()->isEarth(i, getY()) || getWorld()->isBoulder(i, getY()))
//                    return false;
//            }
//            return true;
//        case right:
//            for (int i = getX(); i <= x; i++)
//            {
//                if (getWorld()->isEarth(i, getY()) || getWorld()->isBoulder(i, getY()))
//                    return false;
//            }
//            return true;
//        case up:
//            for (int j = getY(); j >= y; j++)
//            {
//                if (getWorld()->isEarth(getX(), j) || getWorld()->isBoulder(getX(), j))
//                    return false;
//            }
//            return true;
//        case down:
//            for (int j = getX(); j >= y; j--)
//            {
//                if (getWorld()->isEarth(getX(), j) || getWorld()->isBoulder(getX(), j))
//                    return false;
//            }
//            return true;
//        case none:
//            return false;
//    }
//}
//
//void Protester::isAnnoyed(int hp)
//{
//    int lv = getWorld()->getLevel();
//    if (isLeaving)
//        return;
//    
//    decPoints(hp);
//    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
//    restTicks = max(50, 100 - lv * 100);
//    
//    if (hp <= 0)
//    {
//        getWorld() -> playSound(SOUND_PROTESTER_ANNOYED);
//        isLeaving = true;
//        restTicks = 0;
//        
//        if (hp == 100)
//            getWorld()->incScores(500);
//        else if (getID()==TID_PROTESTER)
//            getWorld()->incScores(100);
//        else
//            getWorld()->incScores(250);
//    }
//}
//
//
//void Protester::moveToDirection(Direction dir)
//{
//    int X = getX();
//    int Y = getY();
//    Direction movingDir = getDirection();
//
//    switch(dir)
//    {
//        case left:
//            if(movingDir == left)
//            {
//                if (X != 0)
//                    moveTo(X - 1, Y);
//                else
//                    setDirection(left);
//            }
//            else setDirection(left);
//            break;
//        case right:
//            if(movingDir == right)
//            {
//                if (X != 0)
//                    moveTo(X + 1, Y);
//                else
//                    setDirection(right);
//            }
//            else setDirection(right);
//            break;
//        case up:
//            if(movingDir == up)
//            {
//                if (Y != 0)
//                    moveTo(X , Y + 1);
//                else
//                    setDirection(up);
//            }
//            else setDirection(up);
//            break;
//        case down:
//            if(movingDir == down)
//            {
//                if (Y != 0)
//                    moveTo(X , Y - 1);
//                else
//                    setDirection(down);
//            }
//            else setDirection(down);
//            break;
//        case none:
//            return;
//    }
//}
//
//void Protester::pickUpGold()
//{
//    if(getID()==TID_PROTESTER)
//    {
//        getWorld()->incScores(25);
//        isLeaving = true;
//    }
//    else
//    {
//        getWorld()->incScores(50);
//        restTicks = max(50,100- int(getWorld()->getLevel())*10);
//    }
//}
//
//Regular_Protesters::Regular_Protesters(StudentWorld *sworld, int x, int y):Protester(sworld, x, y, TID_PROTESTER, 5)
//{
//    
//}
//
//Hardcore_Protesters::Hardcore_Protesters(StudentWorld *sworld, int x, int y):Protester(sworld, x, y, TID_HARD_CORE_PROTESTER, 5)
//{
//    
//}
//
