#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <map>
#include <string>

extern const int Width;
extern const int Height; 
extern const int maxChambers;

extern const int numEnemies;
extern const int numGold;
extern const int numTreasure;
extern const int numPotion;

extern std::map<char, int*> charMap; // character -> {maxhp, hp, atk, def, goldmultiplier, potionreverse, gold}
extern std::map<char, std::string> nameStringMap; // maps a type represented in char to a string(full name), should be changed in sync with charMap
extern std::map<char, int*> TempMap;
extern std::map<char, int*> enemMap; // enemy -> {maxhp, hp, atk, def, goldmultiplier, potionreverse, gold, hostility}
extern std::map<char, int> HPMap;


extern void init();

#endif