#include "treasure.h"
#include "character.h"

using namespace std;

Treasure::Treasure(int value, int r, int c):value(value),r(r),c(c){}

Treasure::~Treasure(){}

int Treasure::getRow(){
    return r;
}

int Treasure::getColumn(){
    return c;
}

bool Treasure::getPickedUp(Character* c){
    increaseValue(c);
    return true;
}

void Treasure::increaseValue(Character* c){
    c->increaseGold(value);
}

Dragon* Treasure::getDragon(){};

Dhorde::Dhorde(int value, int r, int c, int dr, int dc):Treasure(value,r,c),mydragon(new Dragon(dr,dc,this)){}

Dhorde::~Dhorde(){
    delete mydragon;
}

Dragon* Dhorde::getDragon(){
    return mydragon;
}

void Dhorde::setNullDragon(){
    mydragon = 0;
}

bool Dhorde::getPickedUp(Character *c){
    if(mydragon){
        return false;
    }
    else {
        increaseValue(c);
        return true;
    }
} 