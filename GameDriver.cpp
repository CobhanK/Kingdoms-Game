#include "Resources.h"
#include "Army.h"
#include "Inventory.h"
#include "Kingdom.h"
#include "Fort.h"
#include "Game.h"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
Game game;
Inventory stores(1000, 2000,1000, 1000, 1000);
int population = 20000;

int result;
int choice;


game.kingdom_declaration(stores, population);
//game.start_print();


//Primary game loop
system("clear");

    do //Start menu loop
    {
        game.start_print();
        result = game.start_menu_print();
        switch(result)
        {
            case 6:
            case -2: //Quit game condition
                cout << "Game quit." << endl;
                return 0;
                break;

            case 1: //Start game
                result = 0;
                cout << "Starting game" << endl;

                game.locate_castles();
                game.random_gen_resources();

                //Game playing loop
                do 
                {
                    system("clear");
                    game.next_turn();
                        
                    result = game.playerLoop();

                    if(game.getWinner() != -1)
                    {
                        result = 6;
                        game.end_screen_print();
                        return 0;
                    }
                    else if(result == 5)
                    {
                        continue;
                    }
                    else if(result == 4)
                    {
                        game.end_screen_print();
                        return 0;
                        break;
                    }

                }while(game.getWinner() == -1 && result != 6);//&& quitFlag == false)
    

                break;
            
            case -1: //Invalid input
                cout << "Make a valid selection" << endl;
                break;
            
            default: //Other menu option
                cout << "Returning to menu" << endl;
                
        }
    } while (!(result == 1 || result == -2));

return 0;
}