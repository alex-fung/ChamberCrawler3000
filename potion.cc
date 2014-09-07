#include "potion.h"
#include <map>
#include <cmath>
#include "variable.h"


Potion::Potion(int r, int c):Character('a',r,c),component(0),pr(r),pc(c){}

Potion::~Potion(){
    Player* temp = Player::getInstance();
    if (component != temp) {
        delete component;
    }
}

bool HPPotion::posSeen = false;
bool HPPotion::negSeen = false;
bool TempPotion::baSeen = false;
bool TempPotion::waSeen = false;
bool TempPotion::bdSeen = false;
bool TempPotion::wdSeen = false;

void HPPotion::setPosSeen(){
    posSeen = true;
}

void HPPotion::setNegSeen(){
    negSeen = true;
}

void TempPotion::setBaSeen(){
    baSeen = true;
}

void TempPotion::setWaSeen(){
    waSeen = true;
}

void TempPotion::setBdSeen(){
    bdSeen = true;
}

void TempPotion::setWdSeen(){
    wdSeen = true;
}

void HPPotion::setNotPosSeen(){
    posSeen = false;
}

void HPPotion::setNotNegSeen(){
    negSeen = false;
}

void TempPotion::setNotBaSeen(){
    baSeen = false;
}

void TempPotion::setNotWaSeen(){
    waSeen = false;
}

void TempPotion::setNotBdSeen(){
    bdSeen = false;
}

void TempPotion::setNotWdSeen(){
    wdSeen = false;
}

bool HPPotion::getPosSeen(){
    return posSeen;
}

bool HPPotion::getNegSeen(){
    return negSeen;
}

bool TempPotion::getBaSeen(){
    return baSeen;
}

bool TempPotion::getWaSeen(){
    return waSeen;
}

bool TempPotion::getBdSeen(){
    return bdSeen;
}

bool TempPotion::getWdSeen(){
    return wdSeen;
}






int Potion::getAtk(){
    return component->getAtk();
}

int Potion::getDef(){
    return component->getAtk();
}

void Potion::setCoordinates(int r, int c){
    component->setCoordinates(r,c);
}

bool Potion::attack(Character &c){
    return component->attack(c);
}
int Potion::getRow(){
    if (component) {return component->getRow();}
	else {return pr;}
}
int Potion::getColumn(){
    if (component) {return component->getColumn();}
	else {return pc;}
}
void Potion::increaseGold(int n){
    component->increaseGold(n);
}
void Potion::increaseHP(int n){
    component->increaseHP(n);
}

int Potion::getGold(){
    return component->getGold();
}

int Potion::getHP(){
    return component->getHP();
}

int Potion::getPotABS(){
    return component->getPotABS();
}

HPPotion::HPPotion(char type, int r, int c):Potion(r,c),health(HPMap[type]){
    Character::setType(type);
}
void HPPotion::setAbs(){
    if (health<0) health = -health;
}

void HPPotion::action(Character* &c){
    c->increaseHP(health);
}

TempPotion::TempPotion(char type, int r, int c):Potion(r,c),atkB(TempMap[type][0]),defB(TempMap[type][1]){
    Character::setType(type);
}

void TempPotion::setAbs(){
    if (atkB<0) atkB = -atkB;
    if (defB<0) defB = -defB;
}
        
int TempPotion::getAtk(){
    return (atkB + component->getAtk());
}

int TempPotion::getDef(){
    return (defB + component->getDef());
}

bool TempPotion::attack(Character &c){
	int damage = ceil((100 * getAtk() / (100 + c.getDef())));

	c.increaseHP(-1*damage);

	return true;
}

void TempPotion::action(Character* &c){
    component = c;
    Character::setType(c->getType());
    c = this;
} 