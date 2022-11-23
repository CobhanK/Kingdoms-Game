#include "Fort.h"

#include <iostream>
using namespace std;

Fort::Fort()
{
    soldiers = 200;
    defenses = 10;
    strengthScore = 0;
    x = -1;
    y = -1;
}
Fort::Fort(int _soldiers, int _x, int _y) //, int _provisions);
{
    soldiers = _soldiers;
    defenses = 10;
    strengthScore = 0;

    x = _x;
    y = _y;

}

// Getters
int Fort::getSoldiers()
{
    return soldiers;
}
int Fort::getDefenses()
{
    return defenses;
}
int Fort::getStrength()
{
    strengthScore = (soldiers * 3) + (defenses * 5);
    return strengthScore;
}


int Fort::getCoordX()
{
    return x;
}
int Fort::getCoordY()
{
    return y;
}

// Setters
void Fort::setSoldiers(int s)
{
    soldiers = s;
}
void Fort::setDefenses(int d)
{
    defenses = d;
}

void Fort::buildDefenses()
{
    defenses += 10;
}

int Fort::siege_loss(int soldier_loss, int defenses_loss)
{
    if(getSoldiers() - soldier_loss >= 0)
    {
        return 0;
    }
    else if(getDefenses() - defenses_loss >= 0)
    {
        return 0;
    }
    else
        return getStrength();
}

bool Fort::setCoords(int _x, int _y)
{
    /*
    if(_x >= 0 && _y >= 0)
    {*/
        x = _x;
        y = _y;
        return 1;
    /*}
    else    
        return 0;*/
}


