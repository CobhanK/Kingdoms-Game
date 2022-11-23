#include "Inventory.h"
#include <iostream>
using namespace std;

// Constructors
Inventory::Inventory()
{
    gold = 0;
    food = 0;

    iron = 0;
    wood = 0;
    stone = 0;
}

Inventory::Inventory(int _gold, int _food)
{
    gold = _gold;
    food = _food;

    iron = 0;
    wood = 0;
    stone = 0;
}
Inventory::Inventory(int _gold, int _food, int _iron, int _wood, int _stone) // Gold, Food, Iron, Lumber, Stone
{
    gold = _gold;
    food = _food;

    iron = _iron;
    wood = _wood;
    stone = _stone;
}
// Getters
int Inventory::getGold()
{
    return gold;
}
int Inventory::getFood()
{
    return food;
}
int Inventory::getIron()
{
    return iron;
}
int Inventory::getWood()
{
    return wood;
}
int Inventory::getStone()
{
    return stone;
}

// Setters
void Inventory::setGold(int _gold)
{
    gold = _gold;
}
void Inventory::setFood(int _food)
{
    food = _food;
}
void Inventory::setIron(int _iron)
{
    iron = _iron;
}
void Inventory::setWood(int _wood)
{
    wood = _wood;
}
void Inventory::setStone(int _stone)
{
    stone = _stone;
}

/*Algorithm changeBy
1. Accepts resource index, amount changed (IDX 1 = Gold, 2 = Food, 3 = Iron, 4 = Wood, 5 = Stone))
2. Adds amount to resource (or subtracts)
3. Returns true if successful
*/
int Inventory::changeBy(int choice, int amount) // INDEX, amount (IDX 1 = Gold, 2 = Food, 3 = Iron, 4 = Wood, 5 = Stone)
{
    switch (choice)
    {
    case 1: // Gold
    {
        if ((gold += amount) >= 0)
        {

            return gold;
        }
        else
        {
            gold -= amount;
            return -1;
        }
        break;
    }
    case 2: // Food
    {
        if ((food += amount) >= 0)
        {

            return food;
        }
        else
        {
            food -= amount;
            return -1;
        }
        break;
    }
    case 3: // Iron
    {
        if ((iron += amount) >= 0)
        {
            return iron;
        }
        else
        {
            iron += amount;
            return -1;
        }
        break;
    }
    case 4: // Wood
    {
        if ((wood += amount) >= 0)
        {
            return wood;
        }
        else
            wood -= amount;
        return -1;

        break;
    }
    case 5: // Stone
    {
        if ((stone += amount) >= 0)
        {
            return stone;
        }
        else
            stone -= amount;
        return -1;

        break;
    }
    default:
        return -1;
    }
}

void Inventory::useGold(int amount)
{
    gold = gold - amount;
}
void Inventory::useFood(int amount)
{
    food = food - amount;
}
void Inventory::useIron(int amount)
{
    iron = iron - amount;
}
void Inventory::useWood(int amount)
{
    wood = wood - amount;
}
void Inventory::useStone(int amount)
{
    stone = stone - amount;
}

void Inventory::display()
{
    cout << "------------" << endl;
    cout << "Gold: " << gold << "\nFood: " << food << "\nIron: " << iron << "\nWood: " << wood << "\nStone: " << stone << endl;
}
