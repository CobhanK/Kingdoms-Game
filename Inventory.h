#ifndef INVENTORY
#define INVENTORY

class Inventory
{
    private:
        int gold;
        int food;

        int iron;
        int wood;
        int stone;
        
    public:
        Inventory();
        Inventory(int, int); //Gold, Food
        Inventory(int, int, int, int, int); //Gold, Food, Iron, Wood, Stone

        //Getters
        int getGold();
        int getFood();

        int getIron();
        int getWood();
        int getStone();

        //Setters
        void setGold(int);
        void setFood(int);

        void setIron(int);
        void setWood(int);
        void setStone(int);

        int changeBy(int, int); //INDEX, amount (IDX 1 = Gold, 2 = Food, 3 = Iron, 4 = Wood, 5 = Stone)

        void useGold(int amount);
        void useFood(int amount);
        void useIron(int amount);
        void useWood(int amount);
        void useStone(int amount);


        void display();
};

#endif