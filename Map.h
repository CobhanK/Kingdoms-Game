#ifndef MAP
#define MAP

#include <iostream>

using namespace std;

class Map
{
    private:
        int used_mapWidth;
        int used_mapHeight;

        const static int mapWidth = 84;  //38;//84;//16; //84;
        const static int mapHeight = 27; //25;//5; //30;66
        

        bool mapFilledFlag;

        char terrain[mapWidth][mapHeight];
        char objects[mapWidth][mapHeight];

    public:
        Map();
        Map(int,int);

        //Getters
        int getMapWidth();
        int getMapHeight();
        char findTerrain(int _x, int _y);
        char findObject(int _x,int _y);

        
        //Setters
        void setMapHeight(int);
        void setMapWidth(int);
        bool addObject(int _x, int _y, char _symbol);
        void fillObjects();
        void fillObjects(int _x, int _y);
        bool addTerrain(int _x, int _y, char _symbol);
        //void clearObjects();

        void setMapFilledFlag(bool);

        //Modifing functions
        int saveObjects(std::string);

        //Print functions
        void printMap();
        void printMapRaw();
        void printObjectsRaw();


        //Checking
        //bool checkIfUsed(int,int);
        //void reCheckObjects();

};

#endif