#include "Resources.h"
#include <iostream>
using namespace std;

Resources::Resources()
{
    name = "";
    numAvail = 0;
    numUsed = 0;
    symbol = ' ';
    production = 0;
}
Resources::Resources(string _name, int _avail, int _used, char _sym) // int _x[], int _y[]) //Name, total available, usable, matricies locations
{
    name = _name;
    numAvail = _avail;
    numUsed = _used;
    symbol = _sym;
    production = 0;
}
Resources::Resources(string _name, int _avail, int _used, char _sym, int _produce) // int _x[], int _y[]) //Name, total available, usable, matricies locations
{
    name = _name;
    numAvail = _avail;
    numUsed = _used;
    symbol = _sym;
    production = _produce;
}

/*Resources::Resources(string _name, int _avail, int _used, char _sym, int _produce, int _pos[][]) //Name, total available, usable, matricies locations
    {
        name = _name;
        numAvail = _avail;
        numUsed = _used;
        symbol = _sym;
        production = _produce;

        for(int i = 0; i < numUsed; i++)
        {
            position =
        }
    }*/

// Getters
string Resources::getName()
{
    return name;
}
int Resources::getAvail()
{
    return numAvail;
}
int Resources::getUsed()
{
    return numUsed;
}
/*
void Resources::printPos()
    {
        for(int i = )
    }
*/

char Resources::getSymbol()
{
    return symbol;
}

int Resources::getProduce()
{
    return production;
}

int Resources::getCoordX(int IDX)
{
    if (IDX >= 0 && numUsed > IDX && numUsed > 0)
        return x_pos[IDX];
    else
        return -1;
}
int Resources::getCoordY(int IDX)
{
    if (IDX >= 0 && numUsed > IDX && numUsed > 0)
        return y_pos[IDX];
    else
        return -1;
}

// Setters
void Resources::setName(string _name)
{
    name = _name;
}
void Resources::setAvail(int _avail)
{
    numAvail = _avail;
}
void Resources::setUsed(int _used)
{
    if (_used <= numAvail)
        numUsed = _used;
}
// FLAG
/*
void Resources::setPosAt(int _x, int _y, char input)
    {
        position[_x][_y] = input;
    }
*/

void Resources::setSymbol(char _sym)
{
    symbol = _sym;
}

void Resources::setProduce(int _produce)
{
    production = _produce;
}

// Functions
bool Resources::setCoords(int _x, int _y)
{
    if (numUsed < numAvail)
    {
        x_pos.push_back(_x);
        y_pos.push_back(_y);
        numUsed++;
        return 1;
    }
    else
        return 0;
}

int Resources::produce()
{
    return numUsed * production;
}

int Resources::findIDX(int x, int y) //IDX of building in building location array
{
    // Check if negative
    if (x < 0 || y < 0)
    {
        return -2;
    }
    int _x = 0;
    int _y = 0;
    for (int i = 0; i < numUsed; i++)
    {
        _x = x_pos[i];
        _y = y_pos[i];

        if (_x == x && _y == y)
            return i;
    }

    // None found
    return -1;
}

int Resources::raid(int IDX)
{
    // Check for valid input
    if (IDX >= numUsed || IDX < 0)
    {
        return -1;
    }

    x_pos.erase(x_pos.begin() + IDX);
    y_pos.erase(y_pos.begin() + IDX);

    numUsed--;
    return production * 4;
}

void Resources::display() // Print all info
{
    cout << "------------" << endl;
    cout << name << " Available: " << numAvail << "\nUsed: " << numUsed << "\nSymbol: " << symbol << endl;
    cout << "Produce: " << production << "\nCoords:" << endl;

    for (int i = 0; i < numUsed; i++)
    {
        cout << i << ':' << x_pos[i] << ',' << y_pos[i] << endl;
    }
}

// bool Resources::build(int, int)