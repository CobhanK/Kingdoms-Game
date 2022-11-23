#ifndef FORT
#define FORT

class Fort
{
    private:
        int soldiers;
        int defenses;
        int strengthScore;

        int x;
        int y;
        //int provisions;

    public:
        Fort();
        Fort(int _soldiers, int _x, int _y); //, int _provisions);


        //Getters
        int getSoldiers();
        int getDefenses();
        int getStrength();

        int getCoordX();
        int getCoordY();

        //Setters
        void setSoldiers(int);
        void setDefenses(int);

        bool setCoords(int _x, int _y);

        void buildDefenses();
        int siege_loss(int soldier_loss, int defenses_loss);
};

#endif