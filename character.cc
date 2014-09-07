#include "character.h"
#include "variable.h"
#include <cmath>
#include <cstdlib>
#include <map>
#include <ctime>

using namespace std;


Character::Character(char type, int r, int c): type(type), maxhp(charMap[type][0]), hp(charMap[type][1]), atk(charMap[type][2]), def(charMap[type][3]), gvalue(charMap[type][4]), potionABS(charMap[type][5]), halfgold(charMap[type][6]), r(r), c(c) {}

Character::~Character(){};

int Character::getAtk(){
	return atk;
}

int Character::getDef(){
	return def;
}

void Character::setCoordinates(int r, int c){
	this->r = r;
	this->c = c;
}

int Character::getRow(){
	return r;
}

int Character::getGold(){
    return halfgold;
}

int Character::getColumn(){
	return c;
}

void Character::increaseGold(int n){
	halfgold += gvalue * n;
}

void Character::increaseHP(int n){
	hp += n;
	if (hp < 0) hp = 0;
	if (hp > maxhp) hp = maxhp;
}

int Character::getHP(){
	return hp;
}

int Character::getPotABS(){
	return potionABS;
}

char Character::getType(){
	return type;
}

void Character::setType(char type){
    this->type = type;
}

Player* Player::instance = NULL;

Player::Player(int r, int c, char type): Character(type, r, c){}

void Player::action(Character* &c){}

void Player::cleanup(){
	delete instance;
	instance = 0;
}

Player* Player::getInstance(int r, int c, char type){
	if(!instance){
		instance = new Player(r, c, type);
		atexit(cleanup);
	}
	return instance;
}

bool Player::attack(Character &c){
	int damage = ceil((100 * getAtk() / (100 + c.getDef())));

	c.increaseHP(-1*damage);

	return true;
}

Enemy::Enemy(char type, int r, int c): Character(type, r, c), hostile(charMap[type][7]){}

void Enemy::action(Character* &c){}

bool Enemy::attack(Character &c){
	if(hostile == 1 && abs(c.getRow() - getRow()) <= 1 && abs(c.getColumn() - getColumn()) <= 1)
	{
	    srand(time(NULL));
		int random = rand() % 2;
		int damage = -1;
		if (random == 0)
			damage = 0;
		else if(random == 1)
			damage = ceil((100 *getAtk() / (100 + c.getDef())));
			

		c.increaseHP(-1*damage);

		return true;
	}

	return false;
}

void Enemy::changeHostility(){
	hostile = 1;
}

Dragon::Dragon(int r, int c, Dhorde* myhorde): Enemy('D', r, c), myhorde(myhorde){}

bool Dragon::attack(Character &c){
	int row = c.getRow();
	int col = c.getColumn();
	int hrow = myhorde->getRow();
	int hcol = myhorde->getColumn();
	if(abs(hrow - row) <= 1 && abs(hcol - col) <= 1 && abs(getRow() - row) <= 1 && abs(getColumn() - col) <= 1){
		srand(time(NULL));
		int random = rand() % 2;
		int damage = -1;
		if (random == 0)
			damage = 0;
		else if(random == 1)
			damage = ceil((100 *getAtk() / (100 + c.getDef())));
			

		c.increaseHP(-1*damage);

		return true;
	}

	return false;
}

void Dragon::notifyHorde(){
    myhorde->setNullDragon();
}

Dragon::~Dragon(){
	notifyHorde();
}