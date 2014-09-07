#ifndef __FLOOR_H__
#define __FLOOR_H__

#include <string>
#include "variable.h"
#include "character.h"
#include "potion.h"
#include "treasure.h"
#include <map>

class Floor {

  public:
    char** original;  
    char** current; 
    
    char*** chamber; //char** chamber[maxChambers]
    char** createChamber(); //records the '.' area of the first chamber found
    void CurrentChamberPrep(); // run before generate chamber from current, removes everything but '.'
    void addToCham(int r, int c, char** cham); // add the point (r c) to cham, and recursively call add cham on its neighbours
	
    void copyToCurrent(bool num = true); // copies the original grid to the current grid;
    
    bool done;
    int floornum; // records which floor the player is on;
    char pRace;
    Character* player; //singleton player
    Enemy** enemies; // array of 20 enemies, Enemy* enemies[numEnemies]
    Treasure** treasure; // array of 30 treasures(gold) in case enemies die and drop gold,Treasure* treasure[numTreasure]
    Potion** potion; // array 10 potions , Treasure* treasure[numPotion]
    
    void makeDragonHorde(int i, int Gcham, int r, int c);
    void spawnPlayerStair();
    void spawnPotions();
    void spawnGold();
    void spawnEnemies();
    Character* findAtLocation(int r, int c);
    
    std::string action;
  
    int stairR,stairC;
//  public:
  
    Floor(std::fstream& layout, char pRace = 'h');
    ~Floor(); //deallocates arrays, make sure delete player last(because of getInstance())
    void print();
    void increaseHP(int n); //call increaseHP in character by n
    void usePotion(Character *p); //call p->action(player), and then call turn()
    //^also delete HPpotion, and set to NULL in array always.
    // set all same-type potions's seen variable to true;
    void attackEnemy(Character *e); //call attack in character,
    //player->attack(*e); -->if merchant, all merchant hostile
    //^function above would call increaseHP on the enemy by negative, after calculating with damage function
    //IF NOT DRAGON: Check HP of enemy --> if equal to 0, delete it, and set it in array to NULL, then construct 1 
    //gold if type of enemy was not merchant or dragon, otherwise construct gold with value 4
    //IF DRAGON: if dragon dies, getDragon from Dhorde, notify Dhorde, and then delete dragon
    //and then call turn()
    void move(char first, char second); //player.setCoordinates(r, c); takes care of potions, enmies, treasure, walls
    //calculate new coordinates
    //search through current to see if it is a accessible space (i.e. '.','+','G','#','\')
    //if so, move the player to new coordinates, and search if it is beside any potion -> if so, print that it sees it.
    //AND if it was a G, search through treasure to determine which treasure is walked over
        //^deletes treasure if getPickedUp(player) returns true, otherwise false, and then set treasure in array to null
    //BUT if it is a '\', then we need to regenerate floor -> call deleteAll(), and then generate()
    //call turn(), unless it reached stairs
    Character* findAtLocation(char first, char second);//return enemy or potion, 
    //or NULL if potion or enemy is not there by searching through arrays above
    void turn(); //run after user's command, make all enemies attack, unless false, then set new valid coordinates(only '.') for enemies, 
    //also check if player is still alive - if not, updateCurrent, print it as it is with the game over message at the bottom,
                                            //set done to true.
    void generate(); //calls each spawn function
    void deleteAll(); // delete all enemies, treasure, and potions, and if player is a potion, delete it.
    void updateCurrent(); // copies original[][] to current[][], get r and c from each of player, enemies, treasure and potion, 
                          // set current[r][c] to the corresponding symbol;
    bool returnStatus(); //returns if done is true or false

};

#endif