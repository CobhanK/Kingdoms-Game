#ifndef GAME
#define GAME

#include "Kingdom.h"
#include "Map.h"

class Game
{
    private:
        const static int sizeKingdomArr = 2;
        const static int sizeMapArr = 4;
        int numKingdoms;
        int mapIDX;

        Kingdom kingdoms[sizeKingdomArr];
        Map map;

        int turnIDX; //IDX of whose turn it is
        int round; //total # of rounds in the game

        bool fileLoadFlag;
        int winnerRealmIDX;

    public:
        Game();

        //Getters
        int getNumKingdoms();
        Kingdom getKingdomAt(int);
        int getWinner();
        Map getMap();

        //Setters
        bool setKingdomAt(int, Kingdom); //IDX, Kingdom
        bool addKingdom(Kingdom);

        //Game functions
        int battle(int atkArmyIDX); //Attacking army IDX
        int siegeCastle(int);
        int siegeFort(int);
        void moveKingdomArmy();
        bool conscriptArmy();
        bool provisionArmy();

    //Checks
        bool checkIfUsed(int,int);
        

        //Checks
        int checkFortProximity(int _x, int _y);
        int checkArmyProximity(int _x, int _y);
        bool checkCastleProximity(int _x, int _y);
        bool checkResourceProximity(int _x, int _y);
        void reCheckObjects();

        //Map reading/saving
        int readMap(string); //Filename, reads file and store in map 2D array
        int readObjects(string);

        void printKingdom(); //Print kingdom index information
        void buildKingdoms(int); //initializes different kingdoms  
        void random_gen_resources();
        void refill_resources(); //Fills resources from objects array

        void joinPlayerArmies();

        void display_new_turn();
        void next_turn();

 
        //Player options 
        void start_print() ; //Initializes kingdoms, prints initial output
        void kingdom_declaration(Inventory, int);
        int locate_castles();
        int start_menu_print();
        void end_screen_print();

        int options_print();
        int playerLoop();
        void print_battleMessage();
        void print_help();
        string load_map_print();

        void build();

};




#endif