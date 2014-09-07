#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "treasure.h"

class Character{
    int maxhp, hp, atk, def; 
    int gvalue; //  = 2 By default, 1 for orc and 4 for dwarf
    int potionABS; //1 by default, -1 for elves
    int r,c; // row, column
    char type; //human,elf,orc OR wolf,vampire,merchant,etc.
    int halfgold; // amount of half gold the player/enemy holds

    public:
        Character(char type, int r, int c);
        virtual ~Character();
        virtual int getAtk();
        virtual int getDef();
        virtual void setCoordinates(int r, int c);
        virtual int getRow();
        virtual int getColumn();
        virtual bool attack(Character &c) = 0;
        virtual void increaseGold(int n);
        virtual void increaseHP(int n); //if HP is less than 0, set to 0, if more than max, set to max
        virtual int getHP();
        virtual int getPotABS();
        virtual int getGold();
        virtual void action(Character* &c) = 0;
        void setType(char type);
        char getType();
};  

//Singleton Player
class Player: public Character{
    static Player* instance;
    Player(int r, int c, char type); // constructs the player according to each type’s specification
    
    public:
        virtual void action(Character* &c);
        static Player* getInstance(int r = -1, int c = -1, char type = 'h');
        virtual bool attack(Character &c); 
		static void cleanup();
        void setType(char type, int r, int c);
};

class Enemy: public Character{
    int hostile; //all hostile = 1 except merchents
    
    public:
        Enemy(char type, int r, int c); //construct enemy
        virtual void action(Character* &c);
        virtual bool attack(Character &c); //check player's coordinates is nearby (remember 50% chance miss), will also check if hostile
        virtual void changeHostility(); //always set hostility to 1 
};

class Dragon: public Enemy{
    Dhorde * myhorde;
    
    public:
        Dragon(int r, int c, Dhorde* myhorde);
		~Dragon();
        virtual bool attack(Character &c); //player's coordinates, checks if near dragon hoard
        void notifyHorde();
};

#endif
