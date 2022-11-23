        #include "Army.h"
        #include <iostream>
        using namespace std;

        //Constructors
        Army::Army()
        {
            soldiers = 0;
            siegeWeapons = 0;
            armorScore = 0;
            provisions = 0;
            strength = 0;
            name = "";
        }
        Army::Army(int _soldiers, int _x, int _y) //Num Soldiers
        {
            soldiers = _soldiers;
            provisions = 0;
            siegeWeapons = 0;
            armorScore = 0;
            strength = 0;
            name = "";

            pos[0] = _x;
            pos[1] = _y;
        }
        Army::Army(int _soldiers, int _siegew, int _armor, int _food, string _name, int _pos[]) //NumSoldiers, Num Siege Weapons, Armor Score, name
        {
            soldiers = _soldiers;
            siegeWeapons = _siegew;
            armorScore = _armor;
            provisions = _food;
            strength = 0;
            name = _name;

            pos[0] = _pos[0];
            pos[1] = _pos[1];
        }

        //Getters
        int Army::getSoldiers()
        {
            return soldiers;
        }
        int Army::getSiegeWeapons()
        {
            return siegeWeapons;
        }
        int Army::getArmorScore()
        {
            return armorScore;
        }
        int Army::getProvisions()
        {
            return provisions;
        }

        double Army::getStrength()
        {
            calcStrength();
            return strength;
        }
        
        string Army::getName()
        {
            return name;
        }

        int Army::getCoordX()
        {
            return pos[0];
        }
        int Army::getCoordY()
        {
            return pos[1];
        }

        //Setters
        void Army::setSoldiers(int s)
        {
            soldiers = s;
        }
        bool Army::setSiegeWeapons(int _siegeW)
        {
            if(_siegeW < 3 && _siegeW >= 0)
            {
                siegeWeapons = _siegeW;
                return 1;
            }
            else    
                return 0;
            
        }
        void Army::setArmorScore(int _armor)
        {
            armorScore = _armor;
        }
        void Army::setProvisions(int _food)
        {
            provisions = _food;
        }
        void Army::setName(string _name)
        {
            name = _name;
        }

        void Army::setCoords(int _x, int _y)
        {
            pos[0] = _x;
            pos[1] = _y;
        }
        void Army::addProvisions(int _food)
        {
            provisions += _food;
        }


        void Army::clearValues()
        {
            soldiers = 0;
            siegeWeapons = 0;
            armorScore = 0;
            provisions = 0;
            strength = 0;
            name = "";
            pos[0] = 0;
            pos[1] = 0;
        }

        void Army::display()
        {
            cout << name << "\nSoldiers: " << soldiers << "\nSiege Weapons: " << siegeWeapons << "\nArmor: " << armorScore << "\nFood: " << provisions << endl;
            cout << "Coords: (" << pos[0] << ',' << pos[1] << ')' << endl;
        }
        double Army::calcStrength()
        {
            strength = soldiers * (1 + (armorScore/100));
            return strength;
        }
        
        //Returns # of soldiers remaining, if 0 then none left destroy army
        int Army::battle_loss(int amount)
        {
            if(getSoldiers() - abs(amount) > 0)
            {
                soldiers = soldiers - abs(amount);
            }
            else
                soldiers = 0;


            return soldiers;
        }
