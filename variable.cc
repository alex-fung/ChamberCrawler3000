#include "variable.h"

const int Width = 79;
const int Height = 25; 
const int maxChambers = 10;

const int numEnemies = 20;
const int numGold = 10;
const int numTreasure = numEnemies + numGold;
const int numPotion = 10;

std::map<char, int*> charMap; // character -> {maxhp, hp, atk, def, goldmultiplier, potionreverse, gold, hostility}
std::map<char, std::string> nameStringMap; // maps a type represented in char to a string(full name), should be changed in sync with charMap
std::map<char, int*> TempMap;
std::map<char, int> HPMap;


int ha[8] = {140, 140, 20, 20, 2, 0, 0, 0}; //human
int da[8] = {100, 100, 20, 30, 4, 0, 0, 0}; //dwarf
int ea[8] = {140, 140, 30, 10, 2, 1, 0, 0}; //elf
int oa[8] = {180, 180, 30, 25, 1, 0, 0, 0}; //orc
int aa[8] = {0, 0, 0, 0, 0, 0, 0, 0};       //empty character

int Va[8] = {50, 50, 25, 25, 2, 0, 0, 1}; //vampire
int Wa[8] = {120, 120, 30, 5, 2, 0, 0, 1}; //werewolf
int Ta[8] = {120, 120, 25, 15, 2, 0, 0, 1}; //troll
int Na[8] = {70, 70, 5, 10, 2, 0, 0, 1}; //goblin
int Ma[8] = {30, 30, 70, 5, 2, 0, 0, 0}; //merchant
int Dra[8] = {150, 150, 20, 20, 2, 0, 0, 1}; //dragon
int Xa[8] = {50, 50, 35, 20, 2, 0, 0, 1}; //pheonix

int baa[2] = {5, 0}; //boost attack
int waa[2] = {-5, 0}; //weaken attack 
int bda[2] = {0, 5}; //boost defense
int wda[2] = {0, -5}; //weaken defense

void init(){
    //mapping from player types to stats
    charMap.insert (std::pair<char,int*>('h', ha));
    charMap.insert (std::pair<char,int*>('d', da));
    charMap.insert (std::pair<char,int*>('e', ea));
    charMap.insert (std::pair<char,int*>('o', oa));
    charMap.insert (std::pair<char,int*>('a', aa));
    //mapping from enemy to stats
    charMap.insert (std::pair<char,int*>('V', Va));
    charMap.insert (std::pair<char,int*>('W', Wa));
    charMap.insert (std::pair<char,int*>('T', Ta));
    charMap.insert (std::pair<char,int*>('N', Na));
    charMap.insert (std::pair<char,int*>('M', Ma));
    charMap.insert (std::pair<char,int*>('D', Dra));
    charMap.insert (std::pair<char,int*>('X', Xa));
    
    //mapping from player char type to player string type
    nameStringMap.insert (std::pair<char,std::string>('h', "Human"));
    nameStringMap.insert (std::pair<char,std::string>('d', "Dwarf"));
    nameStringMap.insert (std::pair<char,std::string>('e', "Elf"));
    nameStringMap.insert (std::pair<char,std::string>('o', "Orc"));

    //a mapping to potion type and increase/decrease in HP
    HPMap['I'] = 10; //captial I means upHP
    HPMap['i'] = -10; // lower-case : downHP
    
    // a mapping to potion type and their atk / def increase/decrease (lots of 0's for other fields from Character)
    TempMap.insert (std::pair<char, int*>('Q', baa)); // atk Boost
    TempMap.insert (std::pair<char, int*>('q', waa)); // atk decrease
    TempMap.insert (std::pair<char, int*>('R', bda)); // def Boost
    TempMap.insert (std::pair<char, int*>('r', wda)); // def reduce
}