#ifndef ARMY
#define ARMY

#include <iostream>
using namespace std;

class Army
{
    private:
        string name;
        int soldiers;
        int siegeWeapons;
        int armorScore;
        int provisions;
        
        double strength;
        int pos[2] = {0}; //X, Y Coordinates

    public:
        const static int maxArmor = 200;
        const static int maxSiegeWeapons = 5;

        //Constructors
        Army();
        Army(int, int, int); //Num Soldiers, x, y
        Army(int, int, int, int, string, int[]); //NumSoldiers, Num Siege Weapons, Armor Score, provisions, string

        //Getters
        int getSoldiers();
        int getSiegeWeapons();
        int getArmorScore();
        int getProvisions();

        double getStrength();
        string getName();

        int getCoordX();
        int getCoordY();

        //Setters
        void setSoldiers(int);
        bool setSiegeWeapons(int);
        void setArmorScore(int);
        void setProvisions(int);
        void setName(string);

        void setCoords(int, int); //X, Y
        void addProvisions(int);
        void use_food();

        void clearValues();

        int battle_loss(int amount);

        //Other
        void display();
        double calcStrength();
        //bool move(int, int);
        

        
};

#endif