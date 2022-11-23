#include "Kingdom.h"
#include "Game.h"
#include "Army.h"
#include "Resources.h"
#include "Inventory.h"
#include "Fort.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

//Supporting function
int split(string inputString, char delim, string arr[], int SIZE)
{
    /*Algorithm Split
    1. Accepts string inputstring, char delimiter marking location to be split,
    string array to store elements, int SIZE of array
    2. Checks for instances of delimiter
    3. Store each section between delimiter in an array (excluding delimiter)
    4. Return int count of pieces the text was split into
    5. If no delimiter then return 1
    6. If string is empty then return 0
    */
    int delimTotal = 0;
    string temp;

    // Test for valid input - if String exists
    if (inputString.length() == 0)
        return 0;

    for (int i = 0; i < inputString.length(); i++)
    {
        // Copying/pasting everything besides delimiter to array
        if (inputString[i] != delim)
        {
            temp = temp + inputString[i];
            // cout << i << ':' << temp << endl;
        }
        // Testing if delimiter not found in entire string
        else if (i >= inputString.length() && delimTotal == 0)
        {
            return 1;
        }
        // If ouput is too large for array size
        else if (SIZE <= delimTotal + 1)
        {
            arr[delimTotal] = temp;
            return -1;
        }
        // Delim is found in function
        else
        {
            arr[delimTotal] = temp;
            // cout <<"Stored " << temp << endl;
            temp = "";
            delimTotal++;
            // cout << "Delim: "<< delimTotal << endl;
        }
    }
    if (temp != "")
    {
        arr[delimTotal] = temp;
    }
    // Return # of pieces in array
    return delimTotal + 1;
}


Game::Game()
{
    numKingdoms = 0;
    turnIDX = 0;
    round = 0;

    fileLoadFlag = false;
    winnerRealmIDX = -1;
    mapIDX = 0;
}

bool Game::addKingdom(Kingdom newRealm)
{
    if(numKingdoms < 2)
    {
        kingdoms[numKingdoms] = newRealm;
        numKingdoms++;
        return 1;
    }
    else
    {
        std::cout << "Kingdom not added" << endl;
        return 0;
    }
}

int Game::getNumKingdoms()
{
    return numKingdoms;
}

Kingdom Game::getKingdomAt(int IDX)
{
    return kingdoms[IDX];
}

int Game::getWinner()
{
    return winnerRealmIDX;
}

Map Game::getMap()
{
    return map;
}

bool Game::checkIfUsed(int x, int y) //Check if coordinates available, returns 1 if used, 0 if not used
{
    int test_x;
    int test_y;
    int num_buildings = 0;

    //Kingdom array loop
    for(int i = 0; i < numKingdoms; i++)
    {
        //Test Army indicies
        for(int j = 0; j < kingdoms[i].getNumArmies(); j++)
        {
            test_x = kingdoms[i].getArmyAt(j).getCoordX();
            test_y = kingdoms[i].getArmyAt(j).getCoordY();

            if(test_x == x && test_y == y)
            {
                return 1;
            }
        }

        //Test Resources
        for(int j = 0; j < kingdoms[i].getResourceArrSize(); j++)
        {
            num_buildings = kingdoms[i].getResourceAt(j).getUsed();
            
            for(int n = 0; n < num_buildings; n++)
            {
                test_x = kingdoms[i].getResourceAt(j).getCoordX(n);
                test_y = kingdoms[i].getResourceAt(j).getCoordY(n);

                if(test_x == x && test_y == y)
                {
                    return 1;
                }
            }
        }

        //Forts loop
        for(int j = 0; j < kingdoms[i].getNumForts(); i++)
        {
            test_x = kingdoms[i].getFortAt(j).getCoordX();
            test_y = kingdoms[i].getFortAt(j).getCoordY();

            if(x == test_x && test_y == y)
            {
                return 1;
            }
        }
    }

return 0;
}

//Returns IDX of enemy fort if detected within spacing, returns -1 if none found
int Game::checkFortProximity(int _x, int _y)
{
    const int fortSpacing = 2;

    int testIDX = 2;
    int test_X = -1;
    int test_Y = -1;

    testIDX = (round + 1) % 2;

    for(int i = 0; i < kingdoms[testIDX].getNumForts(); i++)
    {
        test_X = kingdoms[testIDX].getFortAt(i).getCoordX();
        test_Y = kingdoms[testIDX].getFortAt(i).getCoordY();

        test_X = abs(test_X - _x);
        test_Y = abs(test_Y - _y);

        if(test_X <= fortSpacing && test_Y <= fortSpacing)
        {
            return i;
        }
    }
        
    //None in proximity found
    return -1;
}

//Returns IDX of enemy army if detected within spacing, returns -1 if no army found
int Game::checkArmyProximity(int _x, int _y)
{
    const int armySpacing = 1;

    int testIDX = 2;
    int test_X = -1;
    int test_Y = -1;

    //Makes sure only checking the correct kingdom
    if(turnIDX == 0)
    {
        testIDX = 1;
    }
    else
        testIDX = 0;

    
    for(int i = 0; i < kingdoms[testIDX].getNumArmies(); i++)
    {
        test_X = kingdoms[testIDX].getArmyAt(i).getCoordX();
        test_Y = kingdoms[testIDX].getArmyAt(i).getCoordY();

        test_X = abs(test_X - _x);
        test_Y = abs(test_Y - _y);

        if(test_X <= armySpacing && test_Y <= armySpacing)
        {
            return i;
        }
    }
        
    //None in proximity found
    return -1;
}

bool Game::checkCastleProximity(int _x, int _y)
{
    const int castleSpacing = 1;
    
    int defRealmIDX = (round + 1) % 2;
    Fort defCastle = kingdoms[defRealmIDX].getCastle();

    int test_X = kingdoms[defRealmIDX].getCastle().getCoordX();
    int test_Y = kingdoms[defRealmIDX].getCastle().getCoordY();

    test_X = abs(test_X - _x);
    test_Y = abs(test_Y - _y);

    if(test_X <= castleSpacing && test_Y <= castleSpacing)
    {
        return 1;
    }
    else
        return 0;//None in proximity found
}

//If resource found in proximity -> then resource destroyed
bool Game::checkResourceProximity(int _x, int _y)
{
    int defRealmIDX = (round + 1)%2;
    const int resourceSpacing = 1;
    //int distance = -1;

    int test_X = resourceSpacing + 1;
    int test_Y = resourceSpacing + 1;
    
    Resources resource;

    //Resource loop
    for(int r = 0; r < kingdoms[defRealmIDX].getResourceArrSize(); r++)
    {
        resource = kingdoms[defRealmIDX].getResourceAt(r);

        for(int i = 0; i < resource.getUsed(); i++)
        {
            test_X = resource.getCoordX(i);
            test_Y = resource.getCoordY(i);

            _x = abs(_x - test_X);
            _y = abs(_y - test_Y);

            if(test_X <= resourceSpacing && test_Y <= resourceSpacing)
            {
                cout << kingdoms[turnIDX].getName() << " raided a " << resource.getName() << '!'<<endl;
                int bounty = resource.raid(i);

                return 1;
            }
        }

        kingdoms[defRealmIDX].setResourceAt(r, resource);
    }
    
    return 0;
}



bool Game::setKingdomAt(int IDX, Kingdom newRealm)
{
    if (IDX >= 0 && IDX <= sizeKingdomArr)
    {
        kingdoms[IDX] = newRealm;
        numKingdoms++;
        return 1;
    }
    else
        return 0;
}




// Game functions
/*Algoritm battle
    1. Accepts attacking army IDX,
    2. Calculates strength using armor and soldiers for army
    3. Randomizes power using strength score for each army
    4. Army with higher power wins
    5. Returns 1 if attacker wins, 0 if defender wins
    6. Returns -1 if no army in proximity
    */
int Game::battle(int atkArmyIDX)//Attacking army IDX
{
    srand(time(0));
    int defRealmIDX = -1;
    double lossAmount = 0;

    //Makes sure only checking the correct kingdom
    if(turnIDX == 0)
    {
        defRealmIDX = 1;
    }
    else if(turnIDX == 1)
    {
        defRealmIDX = 0;
    }
    else 
        defRealmIDX = -1;
        

    Army atkArmy = kingdoms[turnIDX].getArmyAt(atkArmyIDX);
    int xA = atkArmy.getCoordX();
    int yA = atkArmy.getCoordY();


    int defArmyIDX = checkArmyProximity(xA,yA);

    // Test if within 1 coordinate of eacchother
    if(defArmyIDX != -1)
    {
        Army defArmy = kingdoms[defRealmIDX].getArmyAt(defArmyIDX);

        int atkStrength = floor(atkArmy.getStrength());
        int defStrength = floor(defArmy.getStrength() * 1.2);



        int testNum = rand() % (atkStrength + defStrength); //Returns random number from 0 to total
        lossAmount = abs((rand() % 400) + 200 + (rand() % 100));
        cout << "Lossamount: " << lossAmount << endl;

    cout <<"Total: " << (atkStrength + defStrength) <<endl<<"Testnum: " << testNum << endl;
        

        // If greater, attacker wins, else defender wins
        if(testNum > atkStrength)
        {
            //Attacker wins
            //lossAmount = abs(atkStrength - defStrength);
            //lossAmount = (rand() % 400) + 100;
            

            //Defender loss
            if(defArmy.battle_loss(lossAmount) <= 100)
            {
                kingdoms[defRealmIDX].destroyArmyAt(defArmyIDX);
                map.fillObjects(defArmy.getCoordX(),defArmy.getCoordY());
            }
            else
            {
                cout << kingdoms[defRealmIDX].getName() <<" lost and has " << defArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[defRealmIDX].setArmyAt(defArmyIDX, defArmy);
            }
                
            lossAmount = (rand() % 100);
            //Attacker loss
            if(atkArmy.battle_loss(lossAmount) <= 100)
            {
                kingdoms[turnIDX].destroyArmyAt(atkArmyIDX);
                map.fillObjects(xA,yA);
            }
            else
            {    
                cout << kingdoms[turnIDX].getName() <<" won and has " << atkArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[turnIDX].setArmyAt(atkArmyIDX, atkArmy);
            }

            reCheckObjects();
            return 1; // Attacker wins
        }
        else //Defender wins
        {
            //Attacker loss
            lossAmount = testNum - atkStrength;

            if(atkArmy.battle_loss(lossAmount) <= 100)
            {
                kingdoms[turnIDX].destroyArmyAt(atkArmyIDX);
                map.fillObjects(xA,yA);
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" lost and has " << atkArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[turnIDX].setArmyAt(atkArmyIDX, atkArmy);
            }
            //Winning army 
            lossAmount = (rand() % 100);

            if(defArmy.battle_loss(lossAmount) <= 100)
            {
                kingdoms[defRealmIDX].destroyArmyAt(defArmyIDX);
                map.fillObjects(defArmy.getCoordX(),defArmy.getCoordY());
            }
            else
            {
                cout << kingdoms[defRealmIDX].getName() <<" won and has " << defArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[defRealmIDX].setArmyAt(defArmyIDX, defArmy);
            }
                
            reCheckObjects();
            return 0; // Defender wins
        }
            
    }
    else
        return -1; // Too far away
}


using namespace std;

void Game::moveKingdomArmy()
{
    system("clear");
    string line;
    int armyIDX = -1;

    char terrainChar; //For how many movement points to use
    int result;

    bool quitFlag = false;

    //Check if armies exist
    if(kingdoms[turnIDX].getNumArmies() <= 0)
    {
        cout << "No armies available to move" << endl;
        return;
    }
    else if(kingdoms[turnIDX].getNumArmies() != 1)
    {
        cout << "Enter which army you wish to move" << endl;
        getline(cin, line);
        armyIDX = stoi(line);
    }
    else 
        armyIDX = 0;
    //cout << "ArmyIDX: " <<armyIDX << endl;

//Check if valid index inputted: positive and if armies exist
    if(armyIDX < 0 || armyIDX >= kingdoms[turnIDX].getNumArmies())
    {
        cout << "Army index invalid:: operation not completed" << endl;
        return;
    }
    else if(kingdoms[turnIDX].getArmyAt(armyIDX).getProvisions() < 50)
    {
        cout <<"Not enough provisions in army to move : you must have at least 10" << endl;
        return;
    }

    do
    {
    int oldX = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordX();
    int oldY = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordY();

    int newX = oldX;
    int newY = oldY;
    
    map.printMap();
    printKingdom();

    //Prompt direction to move army
    cout << "Enter direction you wish to move the army \n(W for up, A for left, S for down, D for right, Q to exit movement phase)" << endl;
    getline(cin, line);
    char direction = line[0];

        switch(direction)
        {
            //Up
            case 87:
            case 119:;
                newY--;
                break;

            //Left
            case 65:
            case 97:
                newX--;
                break;

            //Right
            case 100:
            case 68:
                newX++;
                break;

            //Down
            case 115:
            case 83:
                newY++;
                break;

            //Quit
            case 81:
            case 113:
                quitFlag = true;
                break;
        }

        terrainChar = map.findTerrain(newX, newY);

        if(quitFlag)
        {
            cout <<"Ending movement phase" << endl;
            cout << "Remaining moves: " << kingdoms[turnIDX].getRemaingingMoves() << endl;
            return;
        }

        terrainChar = map.findTerrain(newX, newY);
         //Makes sure new index is unused
        if(checkIfUsed(newX, newY))
        {
            cout << "This location is already used" << endl;
            battle(armyIDX);
            siegeFort(armyIDX);
            siegeCastle(armyIDX);
            continue;
        }
        else if(terrainChar == 'W')
        {
            cout << "You cannot move over water" << endl;
        }
        else if(kingdoms[turnIDX].getRemaingingMoves() >= 4) //Checks if user quit of if remaining moves before moving
        { 
            kingdoms[turnIDX].moveArmy(armyIDX, newX, newY);
            
            
            switch(terrainChar)
            {
                case 82:    
                    kingdoms[turnIDX].useMove();
                    break;
                case 95:
                    kingdoms[turnIDX].useMove();
                    kingdoms[turnIDX].useMove();
                    break;
                case 77:
                    kingdoms[turnIDX].useMove();
                    kingdoms[turnIDX].useMove();
                    kingdoms[turnIDX].useMove();
                    kingdoms[turnIDX].useMove();
                    break;
            }

            //Update objects matrix
            map.fillObjects(oldX,oldY);
            switch(turnIDX)
            {
                case 0: //Red Army
                    map.addObject(newX, newY, 'A'); 
                    break;
                case 1: //Purple Army
                    map.addObject(newX, newY, 'a'); 
            }

            kingdoms[turnIDX].use_food(armyIDX);

        }
        else if(kingdoms[turnIDX].getRemaingingMoves() >= 2) //Checks if user quit of if remaining moves before moving
        { 
            kingdoms[turnIDX].moveArmy(armyIDX, newX, newY);
            
            switch(terrainChar)
            {
                case 82:    
                    kingdoms[turnIDX].useMove();
                    break;
                case 95:
                    kingdoms[turnIDX].useMove();
                    kingdoms[turnIDX].useMove();
                    break;
            }

            //Update objects matrix
            map.fillObjects(oldX,oldY);
            switch(turnIDX)
            {
                case 0: //Red Army 
                    map.addObject(newX, newY, 'A'); 
                    break;
                case 1: //Purple Army
                    map.addObject(newX, newY, 'a'); 
            }  
            kingdoms[turnIDX].use_food(armyIDX);
        }
        else if(kingdoms[turnIDX].getRemaingingMoves() >= 1 && terrainChar == 'R') //If only 1 movement point left
        {
            kingdoms[turnIDX].moveArmy(armyIDX, newX, newY);
            kingdoms[turnIDX].useMove();

            //Update objects matrix
            map.fillObjects(oldX,oldY);
            switch(turnIDX)
            {
                case 0: //Red Army 
                    map.addObject(newX, newY, 'A'); 
                    break;
                case 1: //Purple Army
                    map.addObject(newX, newY, 'a'); 
            } 
            kingdoms[turnIDX].use_food(armyIDX);
        }
        else
            cout << "Ending movement phase" << endl;

        system("clear");


        if(checkArmyProximity(newX, newY) != -1)
        {
                print_battleMessage();
                cout << "You entered into battle with an enemy army!" << endl;
                result = battle(armyIDX);
                if(result == -1)
                {
                    cout <<"Confrontation inconsclusive" <<endl;
                }
                else if(result == 0 || 1)
                    quitFlag = true;
        }

        if(checkCastleProximity(newX, newY))
        {
                print_battleMessage();
                cout << "You entered into a confrontation with the enemy castle!" << endl;
                if(siegeCastle(armyIDX) == 2)
                {
                    quitFlag = true;
                    return;
                }
                else if(result == 0 || 1)
                    quitFlag = true;
        }

        if(checkFortProximity(newX, newY) != -1)
        {
            print_battleMessage();
            cout <<"You entered into a confrontation with an enemy fort!" << endl;
            result = siegeFort(armyIDX);
            if(result == -1)
                {
                    cout <<"Confrontation inconsclusive" <<endl;
                }
            else if(result == 0 || 1)
                    quitFlag = true;
        }

        checkResourceProximity(newX, newY);

        if(kingdoms[turnIDX].getArmyAt(armyIDX).getProvisions() < 10 || kingdoms[turnIDX].getNumArmies() <= 0)
            quitFlag = true;

        //cout << "quitFlag: " << quitFlag << endl;
        //cout << "Remaining moves: " << kingdoms[turnIDX].getRemaingingMoves() << endl;
    }while(kingdoms[turnIDX].getRemaingingMoves() > 0 && quitFlag == false);

    return;
}

void Game::next_turn() //Updates resources in all kingdoms
{


    round++;
    turnIDX = round % 2;

    if(kingdoms[turnIDX].getArmySpawnX() == -1 || kingdoms[turnIDX].getArmySpawnY() == -1 || kingdoms[turnIDX].getCastle().getCoordX() == -1 || kingdoms[turnIDX].getCastle().getCoordY() == -1)
    {
        locate_castles();
        kingdoms[turnIDX].fillResources();
    }

    if(turnIDX == 1)
        cout << "\033[37;45m"<<"Its Player " << turnIDX <<"'s turn!" << "\033[0m" <<endl;
    
    else
        cout << "\033[37;41m" <<"Its Player " << turnIDX <<"'s turn!" << "\033[0m" <<endl;
    
    
    kingdoms[turnIDX].new_turn();

}

/*Algorithm readMap
    1. Accepts string fileName
    2. Stores map data in char terrain matrix
    3. Returns -1 if no file
    4. returns 1 if file transfer complete
    */
int Game::readMap(string fileName) // Filename, reads file and store in map 2D array
{
    string line;
    char holder;

    // Open File
    ifstream fin;
    fin.open(fileName);
    
    //map.setMapWidth(25);
    //map.setMapHeight(13);

    // If file not found return -1
    if (fin.fail())
    {
        cout << "File not found" << endl;
        fin.close();
        return -1;
    }

    int lineIDX = 0;
    while (getline(fin, line) && lineIDX < map.getMapHeight())
    {
        //Check if line exists
        if (line.length() > 1)
        {
            for (int i = 0; i < line.length() && i < map.getMapWidth(); i++)
            {
                
                map.addTerrain(i, lineIDX, line[i]);
                //cout << terrain[i][lineIDX];
                holder = line[i];
                //Test for C and V in map - > if true then store in objects 
                if(holder == 'V')
                    {
                        map.addObject(i,lineIDX,'V');
                        kingdoms[0].setArmySpawn(i,lineIDX);
                    }
                else if(holder == 'v')
                    {
                        map.addObject(i,lineIDX,'v');
                        kingdoms[1].setArmySpawn(i,lineIDX);
                    }
                else if(holder == 'C')
                    {
                        map.addObject(i,lineIDX,'C');
                        kingdoms[0].setCastleCoords(i,lineIDX);
                    }
                else if(holder == 'c')
                    {
                        map.addObject(i,lineIDX,'c');
                        kingdoms[1].setCastleCoords(i,lineIDX);                        
                    }
                else if(!(i < map.getMapWidth()))
                {
                    break;
                }
                else
                {
                    map.addObject(i, lineIDX, '-');
                }

            }

            // Wipe for new line
            line = "";
            holder = '-';
            lineIDX++;
        }
    }

    map.setMapFilledFlag(true);
    fin.close();

    //random_gen_resources();

    return 1;
}

int Game::readObjects(string fileName)
{
    string line;
    char holder;

    // Open File
    ifstream fin;
    fin.open(fileName);

    // If file not found return -1
    if (fin.fail())
    {
        cout << "File not found" << endl;
        fin.close();
        return -1;
    }

    //Clear old array
    //map.clearObjects();

    int lineIDX = 0;
    while (getline(fin, line) && lineIDX < map.getMapHeight())
    {
        //Check if line exists
        if (line.length() > 1)
        {
            for (int i = 0; i < line.length() && i < map.getMapWidth(); i++)
            {
                
                //cout << terrain[i][lineIDX];
                holder = line[i];
                //Test for C and V in map - > if true then store in objects 
                if(holder == 'V')
                    {
                        map.addObject(i,lineIDX,'V');
                        kingdoms[0].setArmySpawn(i,lineIDX);
                    }
                else if(holder == 'v')
                    {
                        map.addObject(i,lineIDX,'v');
                        kingdoms[1].setArmySpawn(i,lineIDX);
                    }
                else if(holder == 'C')
                    {
                        map.addObject(i,lineIDX,'C');
                        kingdoms[0].setCastleCoords(i,lineIDX);
                    }
                else if(holder == 'c')
                    {
                        map.addObject(i,lineIDX,'c');
                        kingdoms[1].setCastleCoords(i,lineIDX);                        
                    }
                else if(!(i < map.getMapWidth()))
                {
                    break;
                }
                else
                    {
                        map.addObject(i, lineIDX, '-');
                    }

            }

            // Wipe for new line
            line = "";
            holder = '-';
            lineIDX++;
        }
    }

    //random_gen_resources();


    fin.close();
    return 1;
}



void Game::printKingdom()
{
    kingdoms[turnIDX].printKingdom();
}

//void buildKingdoms(int); // initializes different kingdoms

void Game::display_new_turn()
{
    std::cout << "It is player " << turnIDX <<"'s turn!" << endl;
    std::cout << kingdoms[turnIDX].getName() <<" has " << kingdoms[turnIDX].getRemaingingMoves() << " moves remaining." << endl;
    std::cout << "Select an option\n--------------------------------------------------------------------------------------------------------------------------------"<< endl;
    std::cout <<"\t1. Build\n\t2. Move Army\n\t3. Attack\n\t4. Join Army\n\t5. Display Resources\n\t6. Save game\n\t7. Quit Game" << endl;
    std::cout <<"--------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void Game::joinPlayerArmies()
{
    bool foundFlag = false;

    string line;
    string tempArr[2];

    int x_1;
    int y_1;

    int x_2;
    int y_2;

    int IDX1;
    int IDX2;

    int choice;

    std::cout << "Enter 1 for enter army Indicies, 2 for coordinates, 3 for quit" << endl;
    getline(cin, line);
    choice = stoi(line);

    switch(choice)
    {
        case 2: 
            std::cout << "Enter the coordinates of the army you want to join to the second army separated by a comma" << endl;
            getline(cin, line);
            split(line, ',', tempArr, 2);

            x_1 = stoi(tempArr[0]);
            y_1 = stoi(tempArr[1]);

            cout << "Enter the coordinates of the army you want to join" << endl;
            getline(cin, line);
            split(line, ',', tempArr, 2);

            x_2 = stoi(tempArr[0]);
            y_2 = stoi(tempArr[1]);

            IDX1 = kingdoms[turnIDX].findArmyIDX(x_1, y_1);
            IDX2 = kingdoms[turnIDX].findArmyIDX(x_2, y_2);

            if(IDX1 == -1 || IDX2 == -1)
            {
                std::cout << "Invalid coordinates" << endl;
                return;
            }

            break;
        case 1:
            std::cout << "Enter the number of the joining army to the army to be joined split by a comma" << endl;
            getline(cin, line);
            split(line, ',', tempArr, 2);

            IDX1 = stoi(tempArr[0]);
            IDX2 = stoi(tempArr[1]);
            break;
    }

    x_1 = kingdoms[turnIDX].getArmyAt(IDX1).getCoordX();
    y_1 = kingdoms[turnIDX].getArmyAt(IDX1).getCoordY();

    int returnedVal = kingdoms[turnIDX].joinArmyTo(IDX1, IDX2);

    if(returnedVal == -1)
    {
        std::cout << "Coordinates are too far away: operation not completed" << endl;
    }
    else if(returnedVal == -2)
    {
        std::cout << "Operation coordinates invalid: operation not completed" << endl;
    }
    else
    {
        map.fillObjects(x_1,y_1);
        std::cout <<  "Armies joined successfully." << endl;
        kingdoms[turnIDX].useMove();
    }
    return;
}

//Fills objects in game.
void Game::reCheckObjects()
{
    int x = -1;
    int y = -1;
    int numUsed;
    bool useUpper = 1;
    char symbol;

    int resArrSize = kingdoms[0].getResourceArrSize();

    for(int k = 0; k < numKingdoms; k++)
    {
        //Check to which kingdom item belongs
        if(k == 0)
        {
            useUpper = true;
        }
        else 
        {
            useUpper = false;
        }

        //Resources array
        for(int i = 0; i < resArrSize; i++)
        {
            numUsed = kingdoms[k].getResourceAt(i).getUsed();
            for(int n = 0; n < numUsed; n++)
            {
                x = kingdoms[k].getResourceAt(i).getCoordX(n);
                y = kingdoms[k].getResourceAt(i).getCoordY(n);

                symbol = kingdoms[k].getResourceAt(i).getSymbol();

                //Makes sure resource belongs to certain kingdom
                if(!useUpper)
                {
                    symbol = tolower(symbol);
                }
                
                map.addObject(x,y, symbol);
            }
        }
            
        //Armies array - using 
        for(int i = 0; i < kingdoms[k].getNumArmies(); i++)
        {
            x = kingdoms[k].getArmyAt(i).getCoordX();
            y = kingdoms[k].getArmyAt(i).getCoordY();

            symbol = 'A';

            if(!useUpper)
            {
                symbol = tolower(symbol);
            }

            map.addObject(x,y, symbol);
        }

        //Forts loop
        for(int i = 0; i < kingdoms[k].getNumForts(); i++)
        {
            x = kingdoms[k].getFortAt(i).getCoordX();
            y = kingdoms[k].getFortAt(i).getCoordY();

            symbol = 'F';

            if(!useUpper)
            {
                symbol = tolower(symbol);
            }

            map.addObject(x,y,symbol);
        }
    }
}

bool Game::conscriptArmy()
{
    bool result = false;
    int armySpawnX = kingdoms[turnIDX].getArmySpawnX();
    int armySpawnY = kingdoms[turnIDX].getArmySpawnY();
    
    result = kingdoms[turnIDX].Conscript();
    //cout << "Result: " << result << endl;


    if(result == 1 && turnIDX == 0)
    {
        kingdoms[turnIDX].useMove();
        system("clear");
        map.addObject(armySpawnX, armySpawnY, 'A');
        cout << "Army successfully conscripted" << endl;
        return 1;
    }
    else if(result == 1 && turnIDX == 1)
    {
        kingdoms[turnIDX].useMove();
        system("clear");
        map.addObject(armySpawnX, armySpawnY, 'a');
        cout << "Army successfully conscripted" << endl;
        return 1;
    }
    else if(result == 0)
    {
        std::cout <<"Conscript failed" << endl;
        return 0;
    }
    else
        std::cout <<"Conscript Fail 2" << endl;

        return 0;
}

void Game::start_print()
{
    //system("clear");
    //Opening text
    cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout <<"\t\t\t\t\t\t\t\t__      __   _                    _         " << endl;
    cout <<"\t\t\t\t\t\t\t\t\\ \\    / /__| |__ ___ _ __  ___  | |_ ___   "<< endl;
    cout <<"\t\t\t\t\t\t\t\t \\ \\/\\/ / -_) / _/ _ \\ '  \\/ -_) |  _/ _ \\  "<< endl;
    cout <<"\t\t\t\t\t\t\t\t  \\_/\\_/\\___|_\\__\\___/_|_|_\\___|  \\__\\___/  "<< endl << endl;

    cout << "\t\t\t\t\t\t\t\t        ______             _       _   _   ___                 _ " << endl;
    cout << "\t\t\t\t\t\t\t\t        |  ___|           | |     | | | | / (_)               | |" << endl;
    cout << "\t\t\t\t\t\t\t\t        | |_ ___ _   _  __| | __ _| | | |/ / _ _ __   __ _  __| | ___  _ __ ___  ___ " << endl;
    cout << "\t\t\t\t\t\t\t\t        |  _/ _ \\ | | |/ _` |/ _` | | |    \\| | '_ \\ / _` |/ _` |/ _ \\| '_ ` _ \\/ __|" << endl;
    cout << "\t\t\t\t\t\t\t\t        | ||  __/ |_| | (_| | (_| | | | |\\  \\ | | | | (_| | (_| | (_) | | | | | \\__ \\ " << endl;
    cout << "\t\t\t\t\t\t\t\t        \\_| \\___|\\__,_|\\__,_|\\__,_|_| \\_| \\_/_|_| |_|\\__, |\\__,_|\\___/|_| |_| |_|___/" << endl;
    cout << "\t\t\t\t\t\t\t\t                                                      __/ |                          " << endl;
    cout << "\t\t\t\t\t\t\t\t                                                     |___/                           \t\t\t\t\t\t\t\t Created by : Cobhan Kale" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void Game::kingdom_declaration(Inventory stores, int population)
{
    Kingdom Eastwood;
    Eastwood.setName("Eastwood");
    Eastwood.setPopulation(population);
    Eastwood.setInventory(stores);
    Eastwood.fillResources();

    Kingdom Westwood;
    Westwood.setName("Westwood");
    Westwood.setPopulation(population);
    Westwood.setInventory(stores);
    Westwood.fillResources();

    addKingdom(Eastwood);
    addKingdom(Westwood);
}

int Game::locate_castles()
{
    int locateCount = 0;
    char line;
    //Find castle and army spawn from objects array
    for(int n = 0; n < map.getMapHeight(); n++)
    {
        for(int i = 0; i < map.getMapWidth(); i++)
        {
            line = map.findObject(i,n);

            switch(line)
            {
                case 67: //Red Castle coords
                    kingdoms[0].setCastleCoords(i,n);
                    locateCount++;
                    break;
                case 99: //Purple castle coords
                    kingdoms[1].setCastleCoords(i,n);  
                    locateCount++;
                    break;
                case 86: //Red army spawn
                    kingdoms[0].setArmySpawn(i,n);
                    locateCount++;
                    break;
                case 118: //purple army spawn
                    kingdoms[1].setArmySpawn(i,n);
                    locateCount++;
                    break;
            }
        }
    }

    cout <<"Locate count: " << locateCount << endl;
    return locateCount;
}

//Returns 1 if start game, -2 if quit, -1 if invalid input
int Game::start_menu_print()
{ 
    string line;
    char choice;
    int result;

    cout << "\t\t\tSelect an option:" << endl;
    cout  << "\t\t\t\t" <<setw(20) << "1.  Start Game       " << setw(20) << "\t2.  Load Map   " << setw(25) << "\t\t3.  Load previous save file" << endl<<endl;
    cout  << "\t\t\t\t" <<setw(20) << "4.  Save current game" << setw(20) << "\t5.  How to play" << setw(25) << "\t\t6.  Quit Game              " << endl; 
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    getline(cin,line);
    choice = line[0];

    //High level menu navigation
    switch(choice)
    {
        case 49: //Start game
            if(!fileLoadFlag)
            {
                map.setMapWidth(25);
                map.setMapHeight(13);
                cout << "File load status: " << readMap("Map_Default.txt"); //"Map.txt"
            }
                
                locate_castles();
                random_gen_resources();

            return 1;
            break;
        case 50: //Load map
            line = load_map_print();

            result = readMap(line);

            switch(result)
            {
                case 1:
                    cout << "File loading successful" << endl;
                    fileLoadFlag = true;
                    break;
                
                case -1:
                    cout << "File does not exist : loading unsuccessful" << endl;
                    break;
            }
            break;

        case 51: //Load previous game
            cout << "Enter a filename + .txt for the map file to be loaded: must have 25 width and 13 height. " << endl;
            getline(cin, line);

            map.setMapWidth(25);
            map.setMapHeight(13);

            readMap("Map_Default.txt");
            result = readObjects(line);

            switch(result)
            {
                case 1:
                    cout << "File object loading successful. Don't forget to load terrain" << endl;
                    break;
                
                case -1:
                    cout << "File does not exist : loading unsuccessful" << endl;
                    break;
            }

            break;

        case 52: //Save current game
            cout <<"Enter a .txt filename to save your game to" << endl;
            getline(cin, line);
            map.saveObjects(line);
            break;

        case 53: //How to play
            print_help();
            return 0;
            break;

        case 81:
        case 113:
        case 54: //Quit
            cout << "Quitting game" << endl;
            return -2;
            break;

        default:
            cout << "Invalid input : input valid selection" << endl;
            return -1;
            break;

    }

    return -1;
}

using namespace std;

int Game::options_print()
{
    string line;
    char choice;

    cout << "\t\t\tSelect an option:" << endl;
    cout  << "\t\t\t\t" <<setw(20) << "1.  Conscript Army   " << setw(20) << "\t2.  Move Army  " << setw(20) << "\t\t3.  Join two armies   " << endl<<endl; 
    cout  << "\t\t\t\t" <<setw(20) << "4.  Build Structure  " << setw(20) << "\t5.  End Turn   " << setw(20) << "\t\t6.  Quit to menu      "<< setw(20) << "\t\t7.  Provision Army   " << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    getline(cin, line);
    choice = line[0];

    switch(choice)
    {
        case 99:
        case 67:
        case 49: //Conscript army
            conscriptArmy();
            break;

        case 87: //W
        case 119: //w
        case 65: //A
        case 97: //a
        case 83: //S
        case 115://s
        case 68: //D
        case 100: //d
        case 50: //Move army
            moveKingdomArmy();
            break;

        //Join 2 armies
        case 74:
        case 106:
        case 51:
            joinPlayerArmies();
            break;
        
        //Build menu
        case 66:
        case 98:
        case 52:
            if(kingdoms[turnIDX].getRemaingingMoves() > 0)
                build();
            else
                cout <<"No moves remaining" << endl;

            break;
        
        case 53:
            cout <<"Ending turn." << endl;
            return 5;
            break;


        case 81:
        case 113:
        case 54: //Quitting
            system("clear");
            cout <<"Quitting to menu . . ." << endl;
            return 6;
            break;

        case 55: //Provision
            provisionArmy();
            break;

        default:
            cout <<"Invalid input : enter a number from 1-6" << endl;
            return -1;
            break;
    }

    return -1;
}

//Returns 5 if turn ends, return 6 if quit to menu
int Game::playerLoop()
{
    int result = 0;
    //random_gen_resources();
    do
    {
        reCheckObjects();
        map.printMap();
        printKingdom();
        //map.printObjectsRaw();
        result = options_print();        

        if(kingdoms[turnIDX].getRemaingingMoves() == 0 )
            result = 5;
        
        else if(getWinner() != -1)
            return 4;

        switch(result)
        {
            case 27:
            case 6: //Quit
                return 6;
                break;
            case 5: //End turn
                return 5;
                break;

        }

    }while(kingdoms[turnIDX].getRemaingingMoves() > 0 && result != 6 && result != 5);

    return 5;
}

bool Game::provisionArmy()
{
    kingdoms[turnIDX].getInventory().getFood();
    string line;
    int armyIDX;
    int amount = 0;

    if(kingdoms[turnIDX].getNumArmies() == 1) //1 army -> auto
    {
        armyIDX = 0;
    }
    else if(kingdoms[turnIDX].getNumArmies() <= 0) //No armies -> quit
    {
        cout <<"No armies to provision" << endl;
        return 0;
    }
    else
    {
        cout << "Enter which army you wish to add provisions" << endl;

        getline(cin, line);
        line = line[0];
        armyIDX = stoi(line);
    }

    //Promp amount
    cout <<"Enter how much provisions you want to add" << endl;
    getline(cin, line);
    amount = stoi(line);

    if(kingdoms[turnIDX].addProvisions(armyIDX,amount))
    {
        cout <<"Provisions successfully added" << endl;
        return 1;
    }
    else
        cout <<"Provisions were not added" << endl;

    return 0;
}


// Game functions

/*Algoritm siegeCastle
    1. Accepts attacking army IDX,
    2. Calculates strength using armor and soldiers for army
    3. Randomizes power using strength score for each army
    4. Army with higher power wins
    5. Returns 1 if attacker wins, 0 if castle wins
    6. Returns 2 if castle is destroyed
    7. Returns -castle was not in proximity
    */
int Game::siegeCastle(int atkArmyIDX)//Attacking army IDX
{
    srand(time(0));
    int defRealmIDX = (round + 1) %2;
    

    Army atkArmy = kingdoms[turnIDX].getArmyAt(atkArmyIDX);
    Fort defCastle = kingdoms[defRealmIDX].getCastle();

    int xA = atkArmy.getCoordX();
    int yA = atkArmy.getCoordY();
    
    //Exits program if close enough
    if(!checkCastleProximity(xA,yA))
    {
        return -1;
    }

    int atkStrength = floor(atkArmy.getStrength());
    int defStrength = floor(defCastle.getStrength());

    int testNum = rand() % (atkStrength + defStrength); //Returns random number from 0 to total

    //Castle win numbers
    int loss_soldiers = abs((rand() % 300) + 50);
    int loss_defenses = abs(rand() % 10) + 1;

    //cout << "Lossamount: " << loss_soldiers << endl;
    //cout <<"Total: " << (atkStrength + defStrength) <<endl<<"Testnum: " << testNum << endl << "Threshold: " << defStrength<< endl;
        

        // If greater, castle wins, else defender wins
        if(testNum > defStrength) //Castle wins
        {
            //Army loss - losses
            if(atkArmy.battle_loss(loss_soldiers) <= 100)
            {
                kingdoms[turnIDX].destroyArmyAt(atkArmyIDX);
                map.fillObjects(xA,yA);
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" lost and has " << atkArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[turnIDX].setArmyAt(atkArmyIDX, atkArmy);
            }

            //Castle win - losses
            loss_soldiers = loss_soldiers / 30;
            loss_defenses = loss_defenses / 5;

            if(defCastle.siege_loss(loss_soldiers, loss_defenses) == 0)
            {
                cout << kingdoms[defRealmIDX].getName() << "'s castle has been destroyed! Game is over!" << endl;
                winnerRealmIDX = turnIDX;
                return 2;
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" won and has " << defCastle.getSoldiers() << " soldiers left and " << defCastle.getDefenses() << " defenses remaining."<<endl;
            }

        }
        else //Army success
        {
            int loss_soldiers = abs((rand() % 400));
            

            //Army victory - losses
            if(atkArmy.battle_loss(loss_soldiers) <= 100)
            {
                cout << "Army " << atkArmyIDX <<" has been destroyed!" << endl;
                kingdoms[turnIDX].destroyArmyAt(atkArmyIDX);
                map.fillObjects(xA,yA);
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" won and has " << atkArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[turnIDX].setArmyAt(atkArmyIDX, atkArmy);
            }

            //Castle loses - losses
            loss_soldiers = loss_soldiers / 3;
            int loss_defenses = abs(rand() % 20) + 10;

            if(defCastle.siege_loss(loss_soldiers, loss_defenses) <= 0)
            {
                cout << kingdoms[defRealmIDX].getName() << "'s castle has been destroyed! Game is over!" << endl;
                return 2;
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" lost and has " << defCastle.getSoldiers() << " soldiers left and " << defCastle.getDefenses() << " defenses remaining."<<endl;
                return 1;
            }

        }
    return -1;
}

/*Algoritm siegeCastle
    1. Accepts attacking army IDX,
    2. Calculates strength using armor and soldiers for army
    3. Randomizes power using strength score for each army
    4. Army with higher power wins
    5. Returns 1 if attacker wins, 0 if castle wins
    6. Returns 2 if castle is destroyed
    7. Returns -castle was not in proximity
    */

 //Attacking army IDX
int Game::siegeFort(int atkArmyIDX)
{
    srand(time(0));
    int defRealmIDX = (round + 1) %2;
    
    Army atkArmy = kingdoms[turnIDX].getArmyAt(atkArmyIDX);


    int xA = atkArmy.getCoordX();
    int yA = atkArmy.getCoordY();

    int fortIDX = checkFortProximity(xA,yA);
    
    //Exits program if not close enough
    if(fortIDX == -1)
    {
        return -1;
    }

    Fort defFort = kingdoms[defRealmIDX].getFortAt(fortIDX);


    int atkStrength = floor(atkArmy.getStrength());
    int defStrength = floor(defFort.getStrength());

    int testNum = rand() % (atkStrength + defStrength); //Returns random number from 0 to total

    //Castle win numbers
    int loss_soldiers = abs((rand() % 300) + 50);
    int loss_defenses = abs(rand() % 10) + 1;

    cout << "Lossamount: " << loss_soldiers << endl;
    cout <<"Total: " << (atkStrength + defStrength) <<endl<<"Testnum: " << testNum << endl << "Threshold: " << defStrength<< endl;
        

        // If greater, castle wins, else defender wins
        if(testNum > defStrength) //Castle wins
        {
            //Army loss - losses
            if(atkArmy.battle_loss(loss_soldiers)  <= 100)
            {
                kingdoms[turnIDX].destroyArmyAt(atkArmyIDX);
                map.fillObjects(xA,yA);
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" lost and has " << atkArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[turnIDX].setArmyAt(atkArmyIDX, atkArmy);
            }

            //Castle win - losses
            loss_soldiers = loss_soldiers / 30;
            loss_defenses = loss_defenses / 5;

            if(defFort.siege_loss(loss_soldiers, loss_defenses) <= 0)
            {
                cout << kingdoms[defRealmIDX].getName() << "'s fort has been destroyed!" << endl;
                kingdoms[defRealmIDX].destroyFortAt(fortIDX);
                return 2;
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" won and has " << defFort.getSoldiers() << " soldiers left and " << defFort.getDefenses() << " defenses remaining."<<endl;
                kingdoms[turnIDX].setFortAt(fortIDX,defFort);
            }

        }
        else //Army success
        {
            int loss_soldiers = abs((rand() % 400));
            

            //Army victory - losses
            if(atkArmy.battle_loss(loss_soldiers) <= 100)
            {
                cout << "Army " << atkArmyIDX <<" has been destroyed!" << endl;
                kingdoms[turnIDX].destroyArmyAt(atkArmyIDX);
                map.fillObjects(xA,yA);
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" won and has " << atkArmy.getSoldiers() << " soldiers left." << endl;
                kingdoms[turnIDX].setArmyAt(atkArmyIDX, atkArmy);
            }

            //Castle loses - losses
            loss_soldiers = loss_soldiers / 3;
            int loss_defenses = abs(rand() % 20) + 10;

            if(defFort.siege_loss(loss_soldiers, loss_defenses) <= 0)
            {
                cout << kingdoms[defRealmIDX].getName() << "'s fort has been destroyed!" << endl;
                kingdoms[defRealmIDX].destroyFortAt(fortIDX);
                return 2;
            }
            else
            {
                cout << kingdoms[turnIDX].getName() <<" lost and has " << defFort.getSoldiers() << " soldiers left and " << defFort.getDefenses() << " defenses remaining."<<endl;
                return 1;
            }
        }
    return -1;
}



void Game::end_screen_print()
{
    if(winnerRealmIDX == 0)
    {
        cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;

        cout <<"\t\t\t\t\t _____          _                           _   _    _ _           "<< endl;
        cout <<"\t\t\t\t\t|  ___|        | |                         | | | |  | (_)          "<< endl;
        cout <<"\t\t\t\t\t| |__  __ _ ___| |___      _____   ___   __| | | |  | |_ _ __  ___ "<< endl;
        cout <<"\t\t\t\t\t|  __|/ _` / __| __\\ \\ /\\ / / _ \\ / _ \\ / _` | | |/\\| | | '_ \\/ __|"<< endl;
        cout <<"\t\t\t\t\t| |__| (_| \\__ \\ |_ \\ V  V / (_) | (_) | (_| | \\  /\\  / | | | \\__ \\"<< endl;
        cout <<"\t\t\t\t\t\\____/\\__,_|___/\\__| \\_/\\_/ \\___/ \\___/ \\__,_|  \\/  \\/|_|_| |_|___/"<< endl;
                                                                   
    }
    else
    {
        cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
        
        cout <<"\t\t\t\t\t _    _           _                           _   _    _ _           "<< endl;
        cout <<"\t\t\t\t\t| |  | |         | |                         | | | |  | (_)          "<< endl;
        cout <<"\t\t\t\t\t| |  | | ___  ___| |___      _____   ___   __| | | |  | |_ _ __  ___ "<< endl;
        cout <<"\t\t\t\t\t| |/\\| |/ _ \\/ __| __\\ \\ /\\ / / _ \\ / _ \\ / _` | | |/\\| | | '_ \\/ __|"<< endl;
        cout <<"\t\t\t\t\t\\  /\\  /  __/\\__ \\ |_ \\ V  V / (_) | (_) | (_| | \\  /\\  / | | | \\__ \\"<< endl;
        cout <<"\t\t\t\t\t \\/  \\/ \\___||___/\\__| \\_/\\_/ \\___/ \\___/ \\__,_|  \\/  \\/|_|_| |_|___/"<< endl;
    
    }

        cout <<endl<<"\t\t\t\t\t\t\t\t   ___"<< endl;                                        
        cout <<"\t\t\t\t\t\t\t\t  / _ \\__ _ _ __ ___   ___    _____   _____ _ __ "<< endl;
        cout <<"\t\t\t\t\t\t\t\t / /_\\/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|"<< endl;
        cout <<"\t\t\t\t\t\t\t\t/ /_\\ (_| | | | | | |  __/ | (_) \\ V /  __/ |   "<< endl;
        cout <<"\t\t\t\t\t\t\t\t\\____/\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|   "<< endl;

        cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;                                                                    
}

void Game::print_battleMessage()
{
cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;                                                                    

cout <<" __  __                     __                 __  _      __         __        __  __  __   " <<endl;
cout <<" \\ \\/ /__  __ __  ___ ___  / /____ _______ ___/ / (_)__  / /____    / /  ___ _/ /_/ /_/ /__ "<<endl;
cout <<"  \\  / _ \\/ // / / -_) _ \\/ __/ -_) __/ -_) _  / / / _ \\/ __/ _ \\  / _ \\/ _ `/ __/ __/ / -_)"<<endl;
cout <<"  /_/\\___/\\_,_/  \\__/_//_/\\__/\\__/_/  \\__/\\_,_/ /_/_//_/\\__/\\___/ /_.__/\\_,_/\\__/\\__/_/\\__/ "<<endl;
cout <<"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;                                                                    
                                                                                        
}

void Game::random_gen_resources()
{
    srand(time(0));

    int range = map.getMapWidth() * .4;
    int numRandom = 10; //(map.getMapHeight() * map.getMapWidth())/5;
    int randRange = range;

    Resources resource;

    char symbol;
    char terrain;

    bool mountainResource = false;

    //Both kingdoms
    for(int k = 0; k < numKingdoms; k++)
    {

        //Each resource in aray
        for(int i = 0; i < kingdoms[k].getResourceArrSize(); i++)
        {
            //30% of resources filled
            resource = kingdoms[k].getResourceAt(i);
            numRandom = resource.getAvail() / 2;

            cout << "Numrandom" << numRandom << endl;

        //Builds num random 
            for(int r = 0; r < numRandom; r++)
            {
            //Randomizes x
                srand(time(0) + r);
                randRange = (rand() % (range * 2)) - range;
                //randRange = randRange - range;

                cout << r << ": range: " << range << "randRange: " << randRange<<endl;

                int randX = abs(kingdoms[k].getCastle().getCoordX() + randRange);


            //Randomizes y
                srand(time(0) + r + i);
                randRange = (rand() % (range * 2)) - range;

                int randY = abs(kingdoms[k].getCastle().getCoordY() + randRange);

                terrain = map.findTerrain(randX, randY);

                cout << "Rand X, Y" << randX << ',' << randY;

                if(terrain == 'M' && (i == 2 || i == 3) && randX >= 0 && randY >= 0 && randX < map.getMapWidth() && randY < map.getMapHeight() && !checkIfUsed(randX, randY)) //Iron mine and quarry
                {
                    if(k == 1)
                        symbol = tolower(symbol);

                    map.addObject(randX, randY, symbol);
                    resource.setCoords(randX, randY);
                    cout << "\tSuccess" << endl;
                }
                else if(terrain == '_' && (i == 0 || i == 1) && randX >= 0 && randY >= 0 && randX < map.getMapWidth() && randY < map.getMapHeight() && !checkIfUsed(randX, randY)) //Wheat and lumber
                {
                    if(k == 1)
                        symbol = tolower(symbol);

                    map.addObject(randX, randY, symbol);
                    resource.setCoords(randX, randY);
                    cout << "\tSuccess" << endl;
                }   
                else
                    cout << "\tFail" << endl;
            }
            kingdoms[k].setResourceAt(i,resource);
        }
    }

    //reCheckObjects();

    return;
}

void Game::build()
{
    string line;
    char choice;
    int numArmies = kingdoms[turnIDX].getNumArmies();
    int numForts = kingdoms[turnIDX].getNumForts();
    int x;
    int y;
    int armyIDX;
    char symbol;

    bool useLowerFlag = turnIDX;
    bool buildUnderFlag = false;

    cout << "\t\tWhat do you want to build :\t\t\tBuildings will be built at square above current army if it is unoccupied\t\t\tPress 'U' if you want to build underneath the current army. BuildUnder mode: "<< buildUnderFlag << endl;
    cout  << "\t\t\t\t" <<setw(25) << "1.  Fort        " << setw(25) <<"2.  Fort Defenses  " <<setw(25)<<"3.  Lumber Yard " <<setw(25) <<"4.  Wind Mill   " <<setw(25) << "5.  Iron Mine" <<endl<<endl;
    cout  << "\t\t\t\t" <<setw(25) << "6.  Stone Quarry" <<setw(25) << "7.  Castle Defenses"<<setw(25) <<"8.  Army armor  " <<setw(25) <<"9.  Siege Weapon" <<setw(25) << "0.  Go back  " <<endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    getline(cin, line);
    choice = line[0];

    if(choice == 'U' || choice == 'u')
    {
        buildUnderFlag = true;

        getline(cin, line);
        choice = line[0];
    }

    switch(choice)
    {
        case 48: //Go back
            return;
            break;

        case 49: //Build fort
            if(numArmies == 0)
            {
                cout <<"No army to build fort at" << endl;
                return;
            }
            else if(numArmies == 1)
            {
                x = kingdoms[turnIDX].getArmyAt(0).getCoordX();
                y = kingdoms[turnIDX].getArmyAt(0).getCoordY();
            }
            else if(numArmies >= 2)
            {
                cout <<"What army do you want to build this at?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);
            }

            if(buildUnderFlag)
                y++;
            else
                y--;

            if(kingdoms[turnIDX].buildFort(x,y) && !checkIfUsed(x,y))
            {
                cout <<"Fort was successfully built" << endl;
                symbol = 'F';
                if(useLowerFlag)
                    symbol = tolower(symbol);

                map.addObject(x,y,symbol);
            }
            else 
            {
                cout <<"Insufficient resources to build a fort" << endl;
            }

            break;

        case 50: //Fort defenses
            if(numForts == 0)
            {
                cout <<"No fort to fortify" << endl;
                return;
            }
            else if(numForts == 1)
            {
                armyIDX = 0;
            }
            else if(numForts >= 2)
            {
                cout <<"What fort do you want to fortify?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);
            }


            if(kingdoms[turnIDX].fortifyFort(armyIDX))
            {
                cout << "Fort fortified : 10 defenses added" << endl;
                kingdoms[turnIDX].useMove();
            }
            else
            {
                cout <<"Insufficient resouces : fort not fortified" << endl;
            }

            return;
            break;


        case 51: //Lumber yard
            
            if(numArmies == 0)
            {
                cout <<"No army to build lumber mill at" << endl;
                return;
            }
            else if(numArmies == 1)
            {
                x = kingdoms[turnIDX].getArmyAt(0).getCoordX();
                y = kingdoms[turnIDX].getArmyAt(0).getCoordY();
            }
            else if(numArmies >= 2)
            {
                cout <<"What army do you want to build this at?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);

                if(armyIDX < numArmies && armyIDX >= 0)
                {
                    x = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordX();
                    y = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordY();
                }
                else
                {
                    cout <<"Invalid input : returning to menu" << endl;
                    return;
                }
            }

            if(buildUnderFlag)
                y++;
            else
                y--;

            //Building successful
            if(kingdoms[turnIDX].build_lumber(x,y) && map.findTerrain(x,y) == '_' && !checkIfUsed(x,y))
            {
                cout << "Lumber mill was built at (" << x << ',' <<y <<")." << endl;
                symbol = 'L';

                if(useLowerFlag)
                    symbol = tolower(symbol);

                map.addObject(x,y,symbol);
                kingdoms[turnIDX].useMove();
            }
            else
                cout <<"Building unsuccessful : insufficient resources" << endl;

            break;

        case 52: //Wind mill
            if(numArmies == 0)
            {
                cout <<"No army to build wind mill at" << endl;
            }
            else if(numArmies == 1)
            {
                x = kingdoms[turnIDX].getArmyAt(0).getCoordX();
                y = kingdoms[turnIDX].getArmyAt(0).getCoordY();
            }
            else if(numArmies >= 2)
            {
                cout <<"What army do you want to build this at?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);

                if(armyIDX < numArmies && armyIDX >= 0)
                {
                    x = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordX();
                    y = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordY();
                }
                else
                {
                    cout <<"Invalid input : returning to menu" << endl;
                    return;
                }
            }

            if(buildUnderFlag)
                y++;
            else
                y--;
            //building successful
            if(kingdoms[turnIDX].build_wheat(x,y) && map.findTerrain(x,y) == '_' && !checkIfUsed(x,y))
            {

                cout << "Wind mill was built at (" << x << ',' <<y <<")." << endl;
                symbol = 'W';

                if(useLowerFlag)
                    symbol = tolower(symbol);
                
                map.addObject(x,y,symbol);
                kingdoms[turnIDX].useMove();
            }
            else
                cout <<"Building unsuccessful : insufficient resources" << endl;


            break;

        case 53: //Iron mine
            if(numArmies == 0)
            {
                cout <<"No army to build iron mine at" << endl;
            }
            else if(numArmies == 1)
            {
                x = kingdoms[turnIDX].getArmyAt(0).getCoordX();
                y = kingdoms[turnIDX].getArmyAt(0).getCoordY();
            }
            else if(numArmies >= 2)
            {
                cout <<"What army do you want to build this at?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);

                if(armyIDX < numArmies && armyIDX >= 0)
                {
                    x = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordX();
                    y = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordY();
                }
                else
                {
                    cout <<"Invalid input : returning to menu" << endl;
                    return;
                }
            }

            //Build under or over
            if(buildUnderFlag)
                y++;
            else
                y--;

            if(map.findTerrain(x,y) != 'M')
            {
                cout <<"You can only build mines and quarries in the mountains : building unsuccessful" << endl;
            }
            else if(kingdoms[turnIDX].build_iron(x,y)  && !checkIfUsed(x,y))
            {

                cout << "Iron mine was built at (" << x << ',' <<y <<")." << endl;
                symbol = 'I';

                if(useLowerFlag)
                    symbol = tolower(symbol);
                
                map.addObject(x,y,symbol);
                kingdoms[turnIDX].useMove();
            }
            else
                cout <<"Building unsuccessful : insufficient resources" << endl;


            break;


        case 54: //Stone quarry
            if(numArmies == 0)
            {
                cout <<"No army to build stone quarry at" << endl;
            }
            else if(numArmies == 1)
            {
                x = kingdoms[turnIDX].getArmyAt(0).getCoordX();
                y = kingdoms[turnIDX].getArmyAt(0).getCoordY();
            }
            else if(numArmies >= 2)
            {
                cout <<"What army do you want to build this at?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);

                if(armyIDX < numArmies && armyIDX >= 0)
                {
                    x = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordX();
                    y = kingdoms[turnIDX].getArmyAt(armyIDX).getCoordY();
                }
                else
                {
                    cout <<"Invalid input : returning to menu" << endl;
                    return;
                }
            }

            //Under or over
            if(buildUnderFlag)
                y++;
            else
                y--;


            if(map.findTerrain(x,y) != 'M')
            {
                cout <<"You can only build mines and quarries in the mountains : building unsuccessful" << endl;
            }
            else if(kingdoms[turnIDX].build_stone(x,y)  && !checkIfUsed(x,y))
            {

                cout << "Stone quarry was built at (" << x << ',' <<y <<")." << endl;
                symbol = 'Q';

                if(useLowerFlag)
                    symbol = tolower(symbol);
                
                map.addObject(x,y,symbol);
                kingdoms[turnIDX].useMove();
            }
            else
                cout <<"Building unsuccessful : insufficient resources" << endl;


            break;

        case 55: // Castle defenses
            if(kingdoms[turnIDX].fortifyCastle())
            {
                cout << "Castle fortified : 10 defenses added" << endl;
                kingdoms[turnIDX].useMove();
            }
            else
            {
                cout <<"Insufficient resouces : castle not fortified" << endl;
            }

            return;
            break;

        case 56: //Armor
            if(numArmies <= 0)
            {
                cout <<"No army to build armor for" << endl;
                return;
            }
            else if(numArmies == 1)
            {
                armyIDX = 0;
            }
            else
            {
                cout <<"What army do you want to build armor for?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);


            }

            if(kingdoms[turnIDX].buildArmor(armyIDX))
            {
                cout << "Army " << armyIDX <<" successfully armored" << endl;
            }
            else
                cout <<"Armoring unsuccessful" << endl;

                return;
                break;

        case 57: //Siege Weapon
            if(numArmies <= 0)
            {
                cout <<"No army to build siege weapons for" << endl;
                return;
            }
            else if(numArmies == 1)
            {
                armyIDX = 0;
            }
            else
            {
                cout <<"What army do you want to build a siege weapon for?" << endl;
                getline(cin, line);

                armyIDX = stoi(line);


            }

            if(kingdoms[turnIDX].buildSiegeWeapon(armyIDX))
            {
                cout << "Army " << armyIDX <<" successfully build a siege weapon" << endl;
            }
            else
                cout <<"Building siege weapon unsuccessful" << endl;

                return;
                break;

        
        
        //Go back
        default:
            cout <<"Invalid input : returning to menu" << endl;
            return;
            break;
    }

    return;
}


void Game::print_help()
{
    string line;
    
    // Open File
    ifstream fin;
    fin.open("Kingdoms_readMe.txt");

    // If file not found return -1
    if (fin.fail())
    {
        cout << "Help File not found" << endl;
        fin.close();
        return;
    }

    while (getline(fin, line))
    {
        cout << line << endl;
        line = "";
    }

    fin.close();
    return;
}



string Game::load_map_print()
{
    string line;
    char choice;
    cout << "\t\t\tSelect an option:" << endl;
    cout  << "\t\t\t\t" <<setw(20) << "1.  Load Default Map " << setw(20) << "\t2.  Load Large Map" << setw(25) << "\t\t3.  Load Massive Map" << endl<<endl;
    cout  << "\t\t\t\t" <<setw(20) << "4.  Load Mini Map    " << setw(20) << "\t5.  Return to menu" << endl; 
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    getline(cin, line);
    choice = line[0];

    switch(choice)
    {
        case 49: //Default
            cout << "Default map has been selected." << endl;
            map.setMapWidth(25);
            map.setMapHeight(13);
            return "Map_Default.txt";

        case 50: //Large
            cout << "Large map has been selected." << endl;
            map.setMapWidth(37);
            map.setMapHeight(25);
            return "Map_Large.txt";

        case 51: //Massive
            cout << "Massive map has been selected." << endl;
            map.setMapWidth(85);
            map.setMapHeight(30);
            return "Map_Massive.txt";

        case 52: //Mini
            cout << "Mini map has been selected." << endl;
            map.setMapWidth(19);
            map.setMapHeight(5);
            return "Map_Mini.txt";

        case 53: //Quit
            cout << "Returning to menu" << endl;
            map.setMapWidth(25);
            map.setMapHeight(13);
            return "Map_Default.txt";

        default:
            cout << "Invalid input: return to menu" << endl;
            map.setMapWidth(25);
            map.setMapHeight(13);
            return "Map_Default.txt";
            break;
    }
}