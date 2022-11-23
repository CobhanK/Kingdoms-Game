#ifndef KINGDOM
#define KINGDOM

#include "Resources.h"
#include "Army.h"
#include "Inventory.h"
#include "Fort.h"

#include <string>
using namespace std;

class Kingdom
{
private:
    string name;
    int population;

    const static int resourceArrSize = 4;
    const static int numProducers = 4;
    const static int maxArmies = 4;
    const static int maxForts = 5; //4 + 1 castle

    int numArmies;
    int numForts;

    int remainingTurnMoves;
    const static int newTurnAmount = 10;


    int armySpawn_X;
    int armySpawn_Y;

    // Resources Initialization

    Resources resources[resourceArrSize]; //= {0 lumber_mill, 1 wheat_mill, 2 iron_mine, 3 quarry, 4 forts, 5 fort_defenses, 6 castle, 7 castle_defenses};

    vector<Army> armies;
    vector<Fort> forts;
    Fort castle;
    Inventory stores;

public:
    Kingdom();
    Kingdom(string, vector<Army>, int, Resources[], Inventory);

    // Getters
        int getPopulation();
        //int getCastleStrength();
        int getNumArmies();
        int getNumForts();
        string getName();

        int getResourceArrSize();

        Resources getResourceAt(int); // Index in resource array
        Inventory getInventory();
        Army getArmyAt(int); // Index in army array
        Fort getFortAt(int);
        Fort getCastle();

        int getRemaingingMoves();
        int getArmySpawnX();
        int getArmySpawnY();

    // Setters
        void setPopulation(int);
        //void setCastleStrength(int);
        void setNumArmies(int);
        void setName(string);

        bool setResourceAt(int, Resources); // Index in array, resource
        void setInventory(Inventory);
        bool setArmyAt(int, Army); // IDX in array, army
        bool addArmy(Army);
        bool addFort(Fort);
        bool setFortAt(int, Fort);

        void setCastleCoords(int _x, int _y);


    bool useMove();

    void fillResources();
    //void random_gen_resourcesArr(int _mapWidth);

    void display();
    void displayFull();
    void printKingdom(); //Display in game

    void setArmySpawn(int _x, int _y);

    // Kingdom Functions
    void new_turn(); // Updates resources in kingdom
    int tax();       // accepts population - calculate taxes that can be taken from population

    // Army Functions
    // bool battle() - returns true/false based on success
    // bool raid() - destroys rival farmland
    // bool siege - attack castle

    //Army functions
    bool Conscript(); //- 100 wood, 100 iron, 200 gold, 1000 people, builds army
    bool addProvisions(int IDX, int amount);
    int joinArmyTo(int, int); // Army being joined, army IDX joining //-2 if IDX out of bounds, -1 if coords too far away, 1 if successful
    void use_food(int);
    // splitArmy();
    bool moveArmy(int, int, int);
    void destroyArmyAt(int armyIDX);

    //Fort Functions
        void destroyFortAt(int);

    // Building
    bool buildArmor(int);       // IDX for army //- 30 iron + 30 gold-> +5 armor
    bool buildSiegeWeapon(int); //- 10 iron, 30 wood, 100 gold
    bool buildFort(int, int);   //- 400 wood, 400 stone = biuld Fort

    bool fortifyFort(int IDX);  // - 20 wood, 100 stone = +10 fort strength
    bool fortifyCastle();       

    // string checkForCoords(int, int); //X, Y
    int findResourceIDX(int, int, string &); // x,y, name to store resource in, returns index of resource at coordinates
    int findArmyIDX(int, int); //if none found return -1

    bool build_lumber(int, int);
    bool build_wheat(int, int);
    bool build_iron(int x, int y);
    bool build_stone(int x, int y);
    

};

#endif