#ifndef BOX
#define BOX "█"

#ifndef STAR
#define STAR "¤"

#ifndef TOWER
#define TOWER "ð"

#define RED "\033[30;41m"
#define PURPLE "\033[37;45m"

#include "Map.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

Map::Map()
{
    mapFilledFlag = false;

    used_mapWidth = 84;
    used_mapHeight = 30;

    for(int i = 0; i < mapHeight; i++)
    {
        for(int j = 0; j < mapWidth; j++)
        {
            objects[j][i] = '-';
        }
    }
    
}

Map::Map(int _width, int _height)
{
    mapFilledFlag = false;

    used_mapWidth = _width;
    used_mapHeight = _height;

    for(int i = 0; i < mapHeight; i++)
    {
        for(int j = 0; j < mapWidth; j++)
        {
            objects[j][i] = '-';
        }
    }
    
}

// Getters
int Map::getMapWidth()
{
    return used_mapWidth;
}
int Map::getMapHeight()
{
    return used_mapHeight;
}
char Map::findTerrain(int _x, int _y)
{
    if(mapFilledFlag)
    {
        return terrain[_x][_y];
    }
    else
        return '0';
}

char Map::findObject(int _x, int _y)
{
    if (mapFilledFlag)
    {
        return objects[_x][_y];
    }
    else
        return ' ';
}

// Setters
void Map::setMapHeight(int _h)
{
    used_mapHeight = _h;
}
void Map::setMapWidth(int _w)
{
    used_mapWidth = _w;
}

void Map::setMapFilledFlag(bool tf)
{
    mapFilledFlag = tf;
}

bool Map::addObject(int _x, int _y, char _symbol)
{
    if (_x >= 0 && _y >= 0 && _x < mapWidth && _y < mapHeight)
    {
        objects[_x][_y] = _symbol;
        return 1;
    }
    else
        return 0;
}

// If object moves -> re-fills objects array
void Map::fillObjects(int _x, int _y)
{
    char terrainChar = findTerrain(_x, _y);

    switch (terrainChar)
    {
    case 77: // Gray mtn
    case 95: // Green grass
    case 82: // Yellow Road
    case 87: // Blue Water
        objects[_x][_y] = '-';
        break;
    default:
        objects[_x][_y] = terrainChar;
        break;
    }

    return;
}
/*
void Map::clearObjects()
{
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            objects[j][i] = '-';
        }
    }
    
    return;
}
*/
// Directly changes terrain matrix
bool Map::addTerrain(int _x, int _y, char _symbol)
{
    if (_x >= 0 && _y >= 0 && _x < getMapWidth() && _y < getMapHeight())
    {
        terrain[_x][_y] = _symbol;
        return 1;
    }
    else
        return 0;
}

    // Modifing functions

/*Algorithm save objects
1. accepts string for filename
2. Prints map from terrain to file
3. If no matching file -> create new file
4. Return 1 if successful
*/
int Map::saveObjects(string fileName)
{
    char line = '_';

    // Open file
    ofstream fout;
    fout.open(fileName);

    if (fout.fail())
    {
        cout << "Failed to save game" << endl;
        return -1;
    }

    for (int i = 0; i < getMapHeight(); i++)
    {
        for (int j = 0; j < getMapWidth(); j++)
        {
            line = objects[j][i];
            // cout << line;
            // line = '_';
            fout << line;
        }
        // cout << endl;
        fout << endl;
    }

    cout << "Objects have been saved" << endl;
    fout.close();

    return 1;
}

using namespace std;

// Print functions
void Map::printMap()
{
    char holder;
    char objectHolder = '-';
    const int spacing = 3;

    // Print line
    for (int i = 0; i < (getMapWidth() * 3) + spacing; i++)
    {
        cout << '_';
    }
    cout << endl;

    // Printing top coords for ease of use
    cout << setw(spacing * 2);
    for (int i = 0; i < getMapWidth(); i++)
    {
        cout << i << setw(spacing);
    }
    cout << endl << setw(spacing + 1);

    // Print map
    for (int i = 0; i < getMapHeight(); i++)
    {
        cout << setw(spacing) << i << ' ';
        for (int j = 0; j < getMapWidth(); j++)
        {
            holder = terrain[j][i];
            objectHolder = objects[j][i];

            // Tests if important object in location
            switch (objectHolder)
            {
            case 65: // RED ARMY
                cout << ' ' << "\033[37;41mAA";
                break;
            case 97: // PURPLE ARMY
                cout << ' ' << "\033[30;45mAA";
                break;
            case 70:
            case 86: // RED TEAM FORT
                // cout << objectHolder << objectHolder<< objectHolder;
                cout << ' ' << RED << STAR << STAR;
                break; 
            case 102:
            case 118: // PURPLE FORT
                // cout << objectHolder << objectHolder<< objectHolder;
                cout << ' ' << PURPLE << STAR << STAR;
                break;
            case 67: // RED CASTLE
                cout << ' ' << "\033[37;41m" << TOWER << TOWER;
                break;
            case 99: // PURPLE CASTLE
                cout << ' ' << "\033[30;45m" << TOWER << TOWER;
                break;

            //Red Resources
            case 76:
            case 87:
            case 73:
            case 81:
                cout << ' ' << RED << objectHolder << objectHolder;
                break;

            //Purple Resoruces
            case 108:
            case 119:
            case 105:
            case 113:
                objectHolder = toupper(objectHolder);
                cout << ' ' << PURPLE << objectHolder << objectHolder;
                break;

            default: // If no objects found then print map

                switch (holder)
                {
                case 77: // Gray mtn
                    cout << "\033[1m\033[30m" << ' ' << BOX << BOX;
                    break;

                case 95: // Green grass
                    cout << "\033[32m" << ' ' << BOX << BOX;
                    break;

                case 82: // Yellow Road
                    cout << "\033[33m" << ' ' << BOX << BOX;
                    break;

                case 87: // Blue Water
                    cout << "\033[44m ##";
                    break;

                default:
                    cout << " __";
                    break;
                }
                break;
            }

            objectHolder = '-';
            cout << "\033[0m";

            if (i >= mapHeight)
                break;
        }

        cout << endl
            << setw(spacing);
    }

    // Printing bottom coords for ease of use
    cout << setw(spacing * 2);
    for (int i = 0; i < getMapWidth(); i++)
    {
        cout << i << setw(spacing);
    }
    cout << endl
         << setw(spacing + 1);

    // Print line
    for (int i = 0; i < (getMapWidth() * 3) + spacing; i++)
    {
        cout << '_';
    }
    cout << endl;

    //printObjectsRaw();
}

void Map::printMapRaw()
{
    for (int i = 0; i < getMapHeight(); i++)
    {
        for (int j = 0; j < getMapWidth(); j++)
        {
            std::cout << terrain[j][i];
        }
        std::cout << std::endl;
    }
    return;
}

void Map::printObjectsRaw()
{
    for (int i = 0; i < getMapHeight(); i++)
    {
        for (int j = 0; j < getMapWidth(); j++)
        {
            cout << objects[j][i];
        }
        cout << endl;
    }
    return;
}

#endif
#endif
#endif