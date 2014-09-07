#ifndef __POTION_H__
#define __POTION_H__

#include "character.h"

class Potion: public Character{
    
    protected:
        Character* component;
		int pr,pc;
    public:
        Potion(int r, int c); 
        virtual ~Potion(); //make sure to not delete component if == to getInstance()
        virtual void action(Character* &c) = 0;
        virtual void setAbs() = 0; // set all fields to their absolute value;
        virtual int getAtk(); //only used by HPpotion, virtual for decorators
        virtual int getDef(); //only used by HPpotion, virtual for decorators
        virtual void setCoordinates(int r, int c);; // call componet's setCoordinates()
        virtual bool attack(Character &c); // call components' attack()
        virtual int getRow();
        virtual int getColumn();
        virtual void increaseGold(int n);
        virtual void increaseHP(int n); //if HP is less than 0, set to 0, if more than max, set to max
        virtual int getHP();
        virtual int getPotABS(); //get potion reverse
       // virtual void setType(char type);
        virtual int getGold();
};

//Non-decorators
class HPPotion: public Potion{
    int health;
    static bool posSeen;
    static bool negSeen;
    
    //call function in floor to set character to new upHP
    
    public:
        static void setPosSeen();
        static void setNegSeen();
        static bool getPosSeen();
        static bool getNegSeen();
        static void setNotPosSeen();
        static void setNotNegSeen();
        HPPotion(char type, int r, int c);
        virtual void setAbs();
        virtual void action(Character* &c);//c->increaseHP(health);
};

//Decorators
class TempPotion: public Potion{
    int atkB; //B stands for boost
    int defB;
    static bool baSeen;
    static bool waSeen;
    static bool bdSeen;
    static bool wdSeen;
    
    public:
        static void setBaSeen();
        static void setWaSeen();
        static void setBdSeen();
        static void setWdSeen();
        static bool getBaSeen();
        static bool getWaSeen();
        static bool getBdSeen();
        static bool getWdSeen();
        static void setNotBaSeen();
        static void setNotWaSeen();
        static void setNotBdSeen();
        static void setNotWdSeen();
        TempPotion(char type, int r, int c);
        virtual void setAbs();
        virtual int getAtk();
        virtual int getDef();
        virtual void action(Character* &c); //set component = c; c = this;
        virtual bool attack(Character &c);
};

#endif
    