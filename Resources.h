#ifndef RESOURCES
#define RESOURCES

#include <iostream>
#include <vector>

using namespace std;

class Resources
{
    private:
        
        string name;
        int numAvail;
        int numUsed;
        
        char symbol;
        int production;

        vector<int> x_pos;
        vector<int> y_pos;

    public:
        Resources();
        Resources(string, int, int, char);
        Resources(string, int, int, char, int);//Name, total available, usable, matricies locations, productions, 
        //Resources(string, int, int, char, int, int[][]);

        //Getters
        string getName();
        int getAvail();
        int getUsed();
        char getSymbol();
        int getProduce();
        
        int getCoordX(int); //return coord of IDX
        int getCoordY(int); //return coord of IDX

        //void printPos();

        //Setters
        void setName(string);
        void setAvail(int);
        void setUsed(int);
        void setSymbol(char);
        void setProduce(int);
        //void setRaidAmount(int);

        bool setCoords(int, int); //x, y, 


        //Functions
        int produce();
        int raid(int); //Returns bounty from raid on IDX, reshuffles 

        void display();

        int findIDX(int,int); //Returns index of coords entered
        //int build(int, int); //Build resource at coords 
        
};

#endif
