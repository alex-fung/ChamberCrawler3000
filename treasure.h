#ifndef __TREASURE_H__
#define __TREASURE_H__

class Character;
class Dragon;

class Treasure{
    int value;
    int r,c;
    
    public:
        Treasure(int value,int r, int c);
        virtual bool getPickedUp(Character* c); //note character is player, call c->increaseGold(value) of Class Character and return true
        int getRow();
        int getColumn();
		virtual Dragon* getDragon();
        void increaseValue(Character* c);
        virtual ~Treasure();
};

// has a relationship with Dragon
class Dhorde: public Treasure{
    Dragon * mydragon;
    
    public:
        Dhorde(int value, int r, int c, int dr, int dc); // constructs a Dhorde, and a new dragon
        ~Dhorde(); //delete mydragon
        Dragon* getDragon(); //returns dragon pointer
        void setNullDragon();
        virtual bool getPickedUp(Character *c); 
        //if mydragon is not null, return false. if dragon is Null, call c->increaseGold(value) of Class Character and return true;
};

#endif