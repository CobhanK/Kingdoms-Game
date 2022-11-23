#include "Kingdom.h"
#include "Resources.h"
#include "Inventory.h"
#include "Army.h"
#include "Fort.h"

#include <iostream>
#include <iomanip>
#include <vector>

//define colors

using namespace std;

Kingdom::Kingdom()
{
    name = "";
    population = 0;

    numArmies = 0;
    numForts = 0;
    remainingTurnMoves = newTurnAmount;
    armySpawn_X = -1;
    armySpawn_Y = -1;

    castle.setCoords(-1,-1);
}

Kingdom::Kingdom(string _name, vector<Army> _armies, int _numArmies, Resources _buildings[], Inventory _stores)
{
    name = _name;

    // Make Armies
    for (int i = 0; i < _numArmies; i++)
    {
        armies[i] = _armies[i];
    }

    for (int i = 0; i < resourceArrSize; i++)
    {
        resources[i] = _buildings[i];
    }

    numArmies = _numArmies;
    numForts = 0;

    stores = _stores;
    remainingTurnMoves = newTurnAmount;

    armySpawn_X = -1;
    armySpawn_Y = -1;
    castle.setCoords(-1,-1);
}

// Getters
int Kingdom::getPopulation()
{
    return population;
}
int Kingdom::getNumArmies()
{
    return numArmies;
}
int Kingdom::getNumForts()
{
    return numForts;
}
string Kingdom::getName()
{
    return name;
}

Resources Kingdom::getResourceAt(int n) // Index in resource array
{
    if (n <= resourceArrSize && n >= 0)
    {
        Resources building = resources[n];
        return building;
    }
    else
    {
        Resources r;
        return r;
    }
}

Inventory Kingdom::getInventory()
{
    return stores;
}
Army Kingdom::getArmyAt(int n) // Index in army array
{
    if (n < maxArmies && n >= 0)
    {
        return armies[n];
    }
    else
    {
        Army null;
        // null.setName("false");
        return null;
    }
}
Fort Kingdom::getFortAt(int IDX)
{
    if(IDX >= 0 && IDX < numForts)
    {
        return forts[IDX];
    }
    else
    {
        Fort null;
        return null;
    }
}
Fort Kingdom::getCastle()
{
    return castle;
}


int Kingdom::getResourceArrSize()
{
    return resourceArrSize;
}

int Kingdom::getRemaingingMoves()
{
    return remainingTurnMoves;
}

int Kingdom::getArmySpawnX()
{
    return armySpawn_X;
}
int Kingdom::getArmySpawnY()
{
    return armySpawn_Y;
}

// Setters
void Kingdom::setPopulation(int _pop)
{
    population = _pop;
}
void Kingdom::setNumArmies(int _numA)
{
    if (_numA <= maxArmies)
        numArmies = _numA;
}
void Kingdom::setName(string _name)
{
    name = _name;
}

bool Kingdom::setResourceAt(int n, Resources building)
{
    if (n <= resourceArrSize && n >= 0)
    {
        resources[n] = building;
        return 1;
    }
    else
        return 0;
}
void Kingdom::setInventory(Inventory _stores)
{
    stores = _stores;
    return;
}
bool Kingdom::setArmyAt(int n, Army army)
{
    if (n <= maxArmies && n >= 0)
    {
        armies[n] = army;
        return 1;
    }
    else
        return 0;
}

bool Kingdom::setFortAt(int IDX, Fort newFort)
{
    if(IDX <= maxForts && IDX >= 0)
    {
        forts[IDX] = newFort;
        return 1;
    }
    else   
        return 0;
}

bool Kingdom::useMove()
{
    if(remainingTurnMoves > 0)
    {
        remainingTurnMoves--;
        return 1;
    }
    else
        return 0;
}

bool Kingdom::addArmy(Army newArmy)
{
    if(numArmies < maxArmies)
    {
        armies.push_back(newArmy);
        numArmies++;
        return 1;
    }
    else
        return 0;
} 

bool Kingdom::addFort(Fort newFort)
{
    if(numForts < maxForts)
    {
        forts.push_back(newFort);
        numForts++;
        return 1;
    }
    else
        return 0;
} 

void Kingdom::setArmySpawn(int _x, int _y)
{
    armySpawn_X = _x;
    armySpawn_Y = _y;
    cout << "Army Spawn: " << armySpawn_X << armySpawn_Y << endl;
}

void Kingdom::setCastleCoords(int _x, int _y)
{
    if(castle.setCoords(_x, _y))
        cout <<"Castle coords set successfully" << endl;
    else
        cout <<"Castle coords not set" <<endl;

    return;
}

void Kingdom::fillResources()
{
    Resources lumber_mill("Lumber Mill", 10, 0, 'L', 20); 
    Resources wheat_mill("Wheat Mills", 20, 0, 'W', 20);
    Resources iron_mine("Iron Mine", 5, 0, 'I', 20);
    Resources quarry("Quarry", 5, 0, 'Q', 20);

    resources[0] = lumber_mill;
    resources[1] = wheat_mill;
    resources[2] = iron_mine;
    resources[3] = quarry;

    return;
}


void Kingdom::display()
{
    cout << "------------" << endl;
    cout << "Kingdom: " << name << "\nPopulation: " << population <<  "\nNum Armies: " << numArmies <<"\nNum Forts: " << numForts<<endl;
}
void Kingdom::displayFull()
{
    cout << "------------" << endl;0;
    cout << "Kingdom: " << name << "\nPopulation: " << population <<  "\nNum Armies: " << numArmies <<"\nNum Forts: " << numForts<<endl;

    for (int i = 0; i < resourceArrSize; i++)
    {
        resources[i].display();
    }

    if(numArmies > 0)
    {
        for(int n = 0; n < numArmies; n++)
        {
            cout << "------------" << endl;
            cout <<"Army #: " << n;
            armies[n].display();
        }
    }
    else    
        cout << "No Armies" << endl;

        return;
}

void Kingdom::printKingdom()
{
    

    
    std::cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    std::cout << '|' << setw(20) << "Kingdom : " <<           setw(10)  <<getName()             << setw(5) <<'|'<< setw(15) << "Inventory"<< setw(12) <<                                '|' << setw(15) << "Production" << setw(30) << '|';
    for(int i = 0; i < numArmies; i++) {cout << setw(17) << "Army # : "<< i << setw(7)<< '|';}
    
    std::cout <<endl<< '|' << setw(20) << "Population : "  << setw(10)  <<         getPopulation()       << setw(5) <<'|'<< setw(15) << "Gold : " <<  setw(7) <<stores.getGold() << setw(5) <<'|'  <<   setw(15) << "Taxes" << setw(18) << getPopulation() / 30 << setw(9) <<"per turn" << setw(3) <<'|';
    for(int i = 0; i < numArmies; i++) {cout << setw(17) << "Soldiers : "<< setw(5)<<armies[i].getSoldiers() << setw(3)<< '|';}
    
    std::cout <<endl<< '|' << setw(20) << "Number of Armies : " <<setw(10)  <<    getNumArmies()       << setw(5) <<'|'<< setw(15) << "Iron : " <<    setw(7) <<stores.getIron() << setw(5) <<'|' << setw(15) << resources[2].getName() << " :"<< setw(4) <<resources[2].getUsed() << " of " << setw(3)<< resources[2].getAvail() << setw(2) << ", " << setw(3) << resources[2].getProduce() * resources[2].getUsed()<< " per turn"<< setw(3) <<'|';
    for(int i = 0; i < numArmies; i++) {cout << setw(17) << "Siege Weapons : "<<  setw(5)<<armies[i].getSiegeWeapons()<< setw(3)<< '|';}
    
    std::cout <<endl<< '|' << setw(20) << "Remaining Moves : " <<setw(10)  <<     getRemaingingMoves()  << setw(5) <<'|'<< setw(15) << "Food : " <<   setw(7) <<stores.getFood() <<setw(5) <<'|' <<  setw(15) << resources[1].getName() << " :"<< setw(4) <<resources[1].getUsed() <<  " of " <<setw(3)<< resources[1].getAvail()<< setw(2) << ", " << setw(3) << resources[1].getProduce() * resources[1].getUsed()<< " per turn"<< setw(3)<< '|';
    for(int i = 0; i < numArmies; i++) {cout << setw(17) << "Provisions : "<<  setw(5)<<armies[i].getProvisions()<< setw(3)<< '|';}
    
    std::cout <<endl<< '|' << setw(20) << "Castle Health : "    <<setw(10)  <<    castle.getStrength() <<setw(5) <<   '|'<< setw(15) << "Stone : "<<   setw(7) <<stores.getStone() <<setw(5) <<'|' << setw(15) << resources[3].getName() << " :"<< setw(4) <<resources[3].getUsed() <<  " of " <<setw(3)<< resources[3].getAvail()<< setw(2) << ", " << setw(3) << resources[3].getProduce() * resources[3].getUsed()<< " per turn"<< setw(3)<< '|'; 
    for(int i = 0; i < numArmies; i++) {cout << setw(16) << "Coordinates :"<<setw(2)<<'('<< setw(2)<<armies[i].getCoordX() << ',' << setw(2)<<armies[i].getCoordY() <<')' << '|';}
    
    std::cout <<endl<< '|' << setw(19) << "Castle Coordinates: " << '(' << setw(2)<<castle.getCoordX() << ',' <<setw(2) << castle.getCoordY() << ')' << setw(8) <<'|'<< setw(15) << "Wood : " << setw(7) <<stores.getWood() <<setw(5) <<'|' << setw(15) << resources[0].getName() << " :"<< setw(4) <<resources[0].getUsed() <<  " of " <<setw(3)<< resources[0].getAvail()<< setw(2) << ", " << setw(3) << resources[0].getProduce() * resources[0].getUsed()<< " per turn"<< setw(3)<< '|';
    for(int i = 0; i < numArmies; i++) {cout << setw(17) << "Armor : "<<  setw(5)<<armies[i].getArmorScore()<< setw(3)<< '|';}
    
    std::cout <<endl<<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    if(numForts > 0)
    {
        std::cout << '|' << setw(20) << "Castle: ð"<< setw(16) <<" |";
        for(int i = 0; i < numArmies; i++) {cout  <<setw(17) << "Fort (¤) # : "<< i << setw(7)<< '|';}

        std::cout <<endl<< '|' << setw(20) << "Castle Defenses: "<< setw(13) << castle.getDefenses() <<" |";
        for(int i = 0; i < numArmies; i++) {cout <<setw(17) << "Defenses: "<< forts[i].getDefenses() << setw(6)<< '|';}

        std::cout <<endl<< '|' << setw(20) << "Castle Solders: "<< setw(13) << castle.getSoldiers() <<" |";
        for(int i = 0; i < numArmies; i++) {cout <<setw(17) << "Soldiers: "<< forts[i].getSoldiers() << setw(6)<< '|';}

        std::cout <<endl<< '|' << setw(19) << "Castle Coordinates: " << '(' << setw(2)<<castle.getCoordX() << ',' <<setw(2) << castle.getCoordY() << ')' << setw(8) <<" |";
        for(int i = 0; i < numArmies; i++) {cout << setw(16) << "Coordinates :"<<setw(2)<<'('<< setw(2)<<forts[i].getCoordX() << ',' << setw(2)<<forts[i].getCoordY() <<')' << '|';}
        std::cout <<endl<<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    }
}

// Kingdom Functions
void Kingdom::new_turn() // Updates resources in kingdom
{
    int oldAmount = 0;
    int newAmount = 0;
    remainingTurnMoves = newTurnAmount;

    // Change gold
    oldAmount = stores.getGold();
    newAmount = oldAmount + tax();
    stores.setGold(newAmount);

    // Lumber
    oldAmount = stores.getWood();
    newAmount = oldAmount + resources[0].produce();
    stores.setGold(newAmount);

    // Food
    oldAmount = stores.getFood();
    newAmount = oldAmount + resources[1].produce();
    stores.setFood(newAmount);

    // Iron
    oldAmount = stores.getIron();
    newAmount = oldAmount + resources[2].produce();
    stores.setIron(newAmount);

    // Stone
    oldAmount = stores.getStone();
    newAmount = oldAmount + resources[3].produce();
    stores.setStone(newAmount);

    population += (rand() % 300) + 100; 

    return;
}

int Kingdom::tax() // accepts population - take from population
{
    int revenue = 0;
    revenue = population / 30;
    return revenue;
}

// Army Functions

// bool raid() - destroys rival farmland
// bool siege - attack castle

bool Kingdom::Conscript() //- 100 wood, 100 iron, 200 gold, 1000 people, builds army
{
    const int foodPrice = 200;
    const int woodPrice = 100;
    const int ironPrice = 100;
    const int goldPrice = 200;



    if(stores.getWood() >= woodPrice && stores.getIron() >= ironPrice && stores.getGold() >= goldPrice && population >= 1000 && numArmies < maxArmies && stores.getFood() >= foodPrice)
    {
        stores.useWood(woodPrice);
        stores.useFood(foodPrice);
        stores.useIron(ironPrice);
        stores.useGold(goldPrice);
        population -= 1000;

        //Army troops;
        Army troops(1000, armySpawn_X, armySpawn_Y);
        troops.setProvisions(foodPrice);
        //troops.setCoords(armySpawn_X, armySpawn_Y);

        armies.push_back(troops);
        numArmies++;
        useMove();
        return 1;
    }
    else
    {
        cout << "Not enough resources for consciption" << endl;
        return 0;
    }
}
bool Kingdom::addProvisions(int IDX, int amount)
{
    int currentFood = stores.getFood();

    if (currentFood >= amount && amount > 0 && IDX >= 0 && IDX < numArmies)
    {
        armies[IDX].addProvisions(amount);
        currentFood -= amount;
        stores.setFood(currentFood);
        return 1;
    }
    else
        return 0;
}

void Kingdom::use_food(int IDX)
{
    int amount = armies[IDX].getProvisions();
    amount -= 10;
    armies[IDX].setProvisions(amount);
    return;
}

int Kingdom::joinArmyTo(int IDX_1, int IDX_2)
{
    //IDX entered out of range
    if((IDX_1 < 0 || IDX_2 < 0 || IDX_1 > numArmies ))//|| IDX_2 > numArmies))
    {
        return -2;
    }

    //Check if coordinates close enough
        //Get joining army coords
        int x_1 = armies[IDX_1].getCoordX();
        int y_1 = armies[IDX_1].getCoordY();

        //Get joined army coords
        int x_2 = armies[IDX_2].getCoordX();
        int y_2 = armies[IDX_2].getCoordY();
    //If too far return -1
    if(abs(x_2 - x_1) > 1 || abs(y_2 - y_1) > 1)
    {
        return -1;
    }

    //Performs joining
        int newSoldiers = armies[IDX_1].getSoldiers() + armies[IDX_2].getSoldiers();
        int newSiegeW = armies[IDX_1].getSiegeWeapons() + armies[IDX_2].getSiegeWeapons();
        int newArmor = armies[IDX_1].getArmorScore() + armies[IDX_2].getArmorScore();
        int newFood = armies[IDX_1].getProvisions() + armies[IDX_2].getProvisions();
        
        Army newArmy(newSoldiers, x_2, y_2);
        newArmy.setProvisions(newFood);
        newArmy.setArmorScore(newArmor);
        newArmy.setSiegeWeapons(newSiegeW);
        newArmy.setName(armies[IDX_2].getName());
        
        setArmyAt(IDX_2, newArmy);
        armies.erase(armies.begin() + IDX_1);
        numArmies--;
        return 1;  
}
// splitArmy();

bool Kingdom::moveArmy(int armyIDX, int newX,int newY)
{
    armies[armyIDX].setCoords(newX, newY);
    return 1;
}

void Kingdom::destroyArmyAt(int armyIDX)
        {
            cout << "Army #" << armyIDX << " from " <<getName() <<" has been destroyed!" << endl;
            
            if(armyIDX == 0)
                armies.erase(armies.begin());
            else
                armies.erase(armies.begin() + armyIDX);

            numArmies--;
        }

void Kingdom::destroyFortAt(int fortIDX)
{
    cout << "Fort #" << fortIDX <<" from " <<getName() << " has been destroyed!" << endl;
    forts.erase(forts.begin()+fortIDX);
}

// Building
bool Kingdom::buildArmor(int armyIDX) //- 30 iron + 30 gold-> +5 armor
{
    if (stores.getIron() >= 30 && stores.getGold() >= 30)
    {
        stores.changeBy(3, -30);
        stores.changeBy(1, -30);
        int armor = armies[armyIDX].getArmorScore();
        armor += 5;
        armies[armyIDX].setArmorScore(armor);
        return 1;
    }
    else
        return 0;
}
bool Kingdom::buildSiegeWeapon(int IDX) //- 10 iron, 50 wood, 100 gold
{
    if (stores.getIron() >= 10 && stores.getWood() >= 50 && stores.getGold() >= 100)
    {
        stores.useIron(10);
        stores.useWood(50);
        stores.useGold(100);
        int siegeW = armies[IDX].getSiegeWeapons();
        siegeW++;
        armies[IDX].setSiegeWeapons(siegeW);
        return 1;
    }
    else
        return 0;
}
bool Kingdom::buildFort(int x, int y) //- 400 wood, 600 stone, 300 gold = build Fort
{
    if(stores.getWood() >= 400 && stores.getStone() >= 600 && stores.getGold() >= 300 && population >= 200 && numForts < maxForts)
    {
        stores.useGold(300);
        stores.useWood(400);
        stores.useStone(600);
        population -= 200;

        Fort newfort(200,x,y);
        useMove();
        addFort(newfort);
        return 1;
    }
    
    return false;
}

bool Kingdom::fortifyCastle() // - 20 wood, 100 stone = +10 castle strength
{
    if (stores.getWood() >= 20 && stores.getStone() >= 100)
    {
        stores.useWood(20);
        stores.useStone(100);
        castle.buildDefenses();
        useMove();
        return 1;
    }
    else
        return 0;
}

bool Kingdom::fortifyFort(int IDX) // - 20 wood, 100 stone = +10 castle strength
{
    if (stores.getWood() >= 30 && stores.getStone() >= 50)
    {
        stores.changeBy(4, -30);
        stores.changeBy(5, -50);
        forts[IDX].buildDefenses();
        useMove();
        return 1;
    }
    else
        return 0;
}

// double calcSize - size based on food, happiness

// string checkForCoords(int, int); //X, Y

int Kingdom::findResourceIDX(int x, int y, string &nomen)
{
    int _x = 0;
    int _y = 0;
    for (int i = 0; i < resourceArrSize && !(_x == x && _y == y); i++)
    {
        for (int j = 0; j < resources[i].getUsed(); j++)
        {
            _x = resources[i].getCoordX(j);
            _y = resources[i].getCoordY(j);

            if (_x == x && _y == y)
            {
                nomen = resources[i].getName();
                return j;
            }
        }
    }

    // None found
    return -1;
}

int Kingdom::findArmyIDX(int x, int y)
{
    int _x = 0;
    int _y = 0;

    for (int i = 0; i < maxArmies && !(_x == x && _y == y); i++)
    {
        _x = armies[i].getCoordX();
        _y = armies[i].getCoordY();

        if (_x == x && _y == y)
        {
            return i;
        }
    }
    // None found
    return -1;
}

bool Kingdom::build_lumber(int x, int y) //- 400 wood, 600 stone, 300 gold = build Fort
{
    if(stores.getWood() >= 20 && stores.getStone() >= 10 && stores.getGold() >= 5)
    {
        stores.useWood(20);
        stores.useStone(10);
        stores.useGold(5);
        resources[0].setCoords(x, y);
        useMove();
        return 1;
    }
    else
        return 0;
}

bool Kingdom::build_wheat(int x, int y) 
{
    if(stores.getWood() >= 20 && stores.getStone() >= 10 && stores.getGold() >= 5)
    {
        stores.useWood(20);
        stores.useStone(10);
        stores.useGold(5);
        resources[1].setCoords(x, y);
        useMove();
        return 1;
    }
    else
        return 0;
}

bool Kingdom::build_iron(int x, int y) 
{
    if(stores.getWood() >= 100 && stores.getStone() >= 10 && stores.getGold() >= 100)
    {
        stores.useWood(100);
        stores.useStone(10);
        stores.useGold(100);
        resources[2].setCoords(x, y);
        useMove();
        return 1;
    }
    else
        return 0;
}

bool Kingdom::build_stone(int x, int y) 
{
    if(stores.getWood() >= 100 && stores.getIron() >= 20 && stores.getGold() >= 100)
    {
        stores.useWood(100);
        stores.useIron(20);
        stores.useGold(100);
        resources[3].setCoords(x, y);
        useMove();
        return 1;
    }
    else
        return 0;
}


