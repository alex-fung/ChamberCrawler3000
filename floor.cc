#include "floor.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "variable.h"

using namespace std;

extern map<char, std::string> nameStringMap;
extern const int Width, Height;
extern const int numEnemies,numTreasure,numPotion,numGold;
extern const int maxChambers;
Floor::Floor(fstream& layout, char race):original(new char* [Height]),current(new char* [Height]),chamber(new char** [maxChambers]),potion(new Potion* [numPotion]),treasure(new Treasure* [numTreasure]),enemies(new Enemy* [numEnemies]),done(false),floornum(0),player(0),pRace(race),action(""){
    
    //allocating space
     for(int i = 0; i < Height; i++){
        original[i] = new char[Width];
        current[i] = new char[Width];
    }
    
    //initializing floor
    string mystr;
    for(int i = 0; i< Height; i ++){
        getline(layout,mystr);
        for(int j=0; j< Width; j++){
            current[i][j] = mystr[j];
            original[i][j] = mystr[j];
        }
    }
    
    //initialize enemies, potion, treasure
	for(int i = 0; i<numEnemies; i ++){
		enemies[i] = 0;
	}
    for(int i = 0; i<numPotion; i ++){
		potion[i] = 0;
	}
	for(int i = 0; i<numTreasure; i ++){
		treasure[i] = 0;
	}
	
    //set all chambers	
	CurrentChamberPrep();
    for(int i = 0; i<maxChambers; i++){
        chamber[i] = createChamber();
    }
    copyToCurrent();
    
    generate();
	print();
}

void Floor::spawnPlayerStair(){

  int ncham = 0;
  for(int i = 0;i<maxChambers;i++){
      if (chamber[i]!=0) ncham++;
  }
  srand (time(NULL));
  int pcham = rand() % ncham; // player chamber
  int scham = rand() % ncham; // stair chamber
  while (scham == pcham) {
      scham = rand() % ncham;
  }
  
  int r = rand() % Height;
  int c = rand() % Width;
  
  
  while (chamber[pcham][r][c]!='.'){
      r = rand() % Height;
      c = rand() % Width;
  }
  player = Player::getInstance(r,c,pRace);
  current [r][c] = '@';
  player->setCoordinates(r, c);
  
  while (chamber[scham][r][c]!='.'){
      r = rand() % Height;
      c = rand() % Width;
  }
  current[r][c] = '\\';
  stairR=r;
  stairC=c;
  action = action + "Player character has spawned.";
}
void Floor::spawnPotions(){
    int ncham = 0;
    for(int i = 0;i<maxChambers;i++){
      if (chamber[i]!=0) ncham++;
    }
    srand (time(NULL));
    int Pcham; // potion chamber
    int whichPo;
    int r = rand() % Height;
    int c = rand() % Width;
    
    for (int i = 0; i < numPotion ; i++){
        
        Pcham = rand() % ncham;
        while ((chamber[Pcham][r][c]!='.')||(current[r][c]!='.')){
            r = rand() % Height;
            c = rand() % Width;
        }
        whichPo = rand() % 6;
        switch (whichPo){
            case 0:
                potion[i] = new HPPotion('I', r, c);
                break;
            case 1:
                potion[i] = new HPPotion('i', r, c);
                break;
            case 2:
                potion[i] = new TempPotion('Q', r, c);
                break;
            case 3:
                potion[i] = new TempPotion('q', r, c);
                break;
            case 4:
                potion[i] = new TempPotion('R', r, c);
                break;
            case 5:
                potion[i] = new TempPotion('r', r, c);
                break;
        }
        current[r][c] = 'P';
        if(player->getPotABS())
            potion[i]->setAbs();
    }
}
void Floor::makeDragonHorde(int i, int Gcham, int r, int c){
    
    srand (time(NULL));
    int dr=-1, dc=-1;
/*    while ((dr==dc)&&(dc==0)){
        dr = (rand() % 3) - 1;
        dc = (rand() % 3) - 1;
    }
*/    
    int count = -1;
    while (current[r+dr][c+dc] != '.'){
        if (count > 5){
            while ((chamber[Gcham][r][c]!='.')||(current[r][c]!='.')){
                r = rand() % Height;
                c = rand() % Width;
            }
            return makeDragonHorde(i,Gcham,r,c);
        }
        
        count++;
        switch (count){
            case 0:
                dr = -1;
				dc = 0;
                break;
            case 1:
                dr = -1;
				dc = 1;
                break;
            case 2:
                dr = 0;
				dc = -1;
                break;
            case 3:
                dr = 0;
				dc = 1;
                break;
            case 4:
                dr = 1;
				dc = -1;
                break;
            case 5:
                dr = 1;
				dc = 0;
                break;
			case 6:
                dr = 1;
				dc = 1;
                break;
        }
    }
    treasure[i] = new Dhorde(6,r,c,r+dr,c+dc);
	for (int g=0;g<numEnemies;g++){
		if (enemies[g]==0){
			enemies[g] = treasure[i]->getDragon();
			break;
		}
	}
    current[r+dr][c+dc] = 'D';
    current[r][c] = 'G';
}

void Floor::spawnGold(){
    srand (time(NULL));
    int Gcham; // Gold chamber
    int whichHorde; // 0-4 : normal gold, 5-6: small horde, 7: dragon horde
    int r = rand() % Height;
    int c = rand() % Width;

      int ncham = 0;
    for(int i = 0;i<maxChambers;i++){
      if (chamber[i]!=0) ncham++;
    }
    for (int i = 0; i < numGold ; i++){
        
        Gcham = rand() % ncham;
        while ((chamber[Gcham][r][c]!='.')||(current[r][c]!='.')){
            r = rand() % Height;
            c = rand() % Width;
        }
        whichHorde = rand() % 8;
        
        if (whichHorde <= 4) {
            treasure[i] = new Treasure (1,r,c);
            current[r][c] = 'G';
        }
        else if (whichHorde <= 6) {
            treasure[i] = new Treasure(2,r,c);
            current[r][c] = 'G';
        }
        else { // find a dragon's location and pass that to Dhorde constructor
            makeDragonHorde(i,Gcham, r,c);
        }
    }
}

void Floor::spawnEnemies(){
    
    srand (time(NULL));
    int Echam; // enemy chamber
    int whichEn;
    
    int r = rand() % Height;
    int c = rand() % Width;
    
    // find the index of the pointer p s.t. Enemies[p] is null(since dragons were created before)
    int p = 0;
    while(enemies[p] != 0){
        p++;
    }
    
    int ncham = 0;
    for(int i = 0;i<maxChambers;i++){
      if (chamber[i]!=0) ncham++;
    }
    
    for (; p < numEnemies ; p++){
        Echam = rand() % ncham;
        while ((chamber[Echam][r][c]!='.')||(current[r][c]!='.')){
            r = rand() % Height;
            c = rand() % Width;
        }
        
        whichEn = rand() % 18;
        
        if (whichEn <= 1) { // Troll
            enemies[p] = new Enemy('T', r, c);
            current[r][c] = 'T';
        }
        else if (whichEn <=3){ // Phoenix
            enemies[p] = new Enemy('X', r, c);
            current[r][c] = 'X';
        } 
        else if (whichEn <=5) { // Merchant
            enemies[p] = new Enemy('M', r, c);
            current[r][c] = 'M';
        }
        else if (whichEn <=9) { // Werewolf
            enemies[p] = new Enemy('W', r, c);
            current[r][c] = 'W';
        }
        else if (whichEn <=12 ) { // Vampire
            enemies[p] = new Enemy('V', r, c);
            current[r][c] = 'V';
        }
        else { // Goblin
            enemies[p] = new Enemy('N', r, c);
            current[r][c] = 'N';
        }
    }

}

//check if a floor plan has been provided, if true, generate according to plan
//if not ture, random spawning()
void Floor::generate(){
	bool FloorPlanProvided = false;
	copyToCurrent();
	for(int i = 0; i < Height; i++){
        for (int j=0; j < Width ; j++){
            if((original[i][j] != '|')&&(original[i][j] != '-')&&(original[i][j] != '+')&&(original[i][j] != '#')&&(original[i][j] != ' ')&&(original[i][j] != '.'))
				FloorPlanProvided = true;
        }
    }
	if (FloorPlanProvided){
	    bool pspawned = false;
	    for(int i = 0; i < Height; i++){
          for (int j=0; j < Width ; j++){
			if (original[i][j] == '@') {
				player = Player::getInstance(i,j,pRace);
				pspawned = true;
				player->setCoordinates(i,j);
			}
		  }
		}
		for(int i = 0; i < Height; i++){
            for (int j=0; j < Width ; j++){
                if((original[i][j] != '|')&&(original[i][j] != '-')&&(original[i][j] != '+')&&(original[i][j] != '#')&&(original[i][j] != ' ')&&(original[i][j] != '.')){
			        
			        int pE=0;
			        int pT = 0;
			        int pP=0;
					int Gcham = 0; // Dragon's chamber
			        for (;pE<numEnemies;pE++){
			            if (enemies[pE] == 0) break;
			        }
			        for (;pT<numTreasure;pT++){
			            if (treasure[pT] == 0) break;
			        }
			        for (;pP<numPotion;pP++){
			            if (potion[pP] == 0) break;
			        }
			        
			        if (original[i][j] == '0') {
						potion[pP] = new HPPotion('I', i, j);
						current[i][j] = 'P';
					}
			        else if (original[i][j] == '1') {
						potion[pP] = new TempPotion('Q', i, j);
						current[i][j] = 'P';
					}
			        else if (original[i][j] == '2') {
						potion[pP] = new TempPotion('R', i, j);
						current[i][j] = 'P';
					}
			        else if (original[i][j] == '3') {
						potion[pP] = new HPPotion('i', i, j);
						if(player->getPotABS())
                            potion[pP]->setAbs();
						current[i][j] = 'P';
					}
			        else if (original[i][j] == '4') {
						potion[pP] = new TempPotion('q', i, j);
						if(player->getPotABS())
                            potion[pP]->setAbs();
						current[i][j] = 'P';
					}
			        else if (original[i][j] == '5') {
						potion[pP] = new TempPotion('r', i, j);
						if(player->getPotABS())
                            potion[pP]->setAbs();
						current[i][j] = 'P';
					}
			        else if (original[i][j] == '6') {
						treasure[pT] = new Treasure (1,i,j);
						current[i][j] = 'G';
					}
			        else if (original[i][j] == '7') {
						treasure[pT] = new Treasure (2,i,j);
						current[i][j] = 'G';
					}
			        else if (original[i][j] == '8') {
						treasure[pT] = new Treasure (4,i,j);
						current[i][j] = 'G';
					}
			        else if (original[i][j] == '9') {
						for (int c = 0; chamber[c] != 0; c++){
							if (chamber[c][i][j] == '.') Gcham = c;
						}
						makeDragonHorde(pT, Gcham, i, j);// assuming this will be bypassed.
			        }
					else if (original[i][j] == '@') {
						player = Player::getInstance(i,j,pRace);
						pspawned = true;
						player->setCoordinates(i,j);
					}
					else if (original[i][j]== '\\' ){
						stairR=i;
						stairC=j;
					}
					else {
						enemies[pE] = new Enemy(original[i][j], i, j);
					}
			    }
            }
	    }
	    if (!pspawned) spawnPlayerStair();
		spawnEnemies();
	}
	else{
		spawnPlayerStair();
		spawnPotions();
		spawnGold();
		spawnEnemies();
	}
	action+= " Welcome to a new floor!";
	floornum++;
}
    
void Floor::copyToCurrent(bool num){
    for(int i = 0; i < Height; i++){
        for (int j=0; j < Width ; j++){
            current[i][j] = original[i][j];
			if (!num && original[i][j]!= ' '&& original[i][j]!= '+'&& original[i][j]!= '|'&& original[i][j]!= '-'&& original[i][j]!= '#'&& original[i][j]!= '.'){
				current[i][j] = '.';
			}
        }
    }
}

void Floor::CurrentChamberPrep(){
    
	for(int i = 0; i < Height; i++){
        for (int j=0; j < Width ; j++){
            if ((current[i][j] == '|')||(current[i][j] == '-')||(current[i][j] == '+')||(current[i][j] == '#')||(current[i][j] == ' ')) current[i][j] = ' ';
			else current[i][j] = '.';
        }
    }
}


void Floor::addToCham(int r, int c, char** cham){

    cham[r][c] = '.';
	current[r][c] = ' ';
    for (int i=c-1;i < c+2;i++) {
        if (current[r-1][i] == '.') addToCham(r-1,i,cham);
        if (current[r+1][i] == '.') addToCham(r+1,i,cham);
    }
    if (current[r][c-1] == '.') addToCham(r,c-1,cham);
    if (current[r][c+1] == '.') addToCham(r,c+1,cham);

}
char** Floor::createChamber(){
    char** cham = 0;
    
    for(int i = 0; i < Height; i++){
        for (int j=0; j < Width ; j++){
            
            if (current[i][j] == '.') {
                cham = new char*[Height];
                //initializing the new chamber;
                for(int z=0;z<Height; z++){
                    cham[z] = new char[Width];
                    for (int x = 0;x<Width;x++){
                        cham[z][x] = ' ';
                    }
                }
                addToCham(i,j,cham);
                return cham;
            }
        }
    }
    
    return cham;
}

void Floor::print(){
    
	updateCurrent();
	
    for(int i = 0; i < Height; i++){
        for(int j=0; j< Width; j++){
            cout << current[i][j];
        }
        cout << endl;
    }
    
    string ptype = nameStringMap[player->getType()];  // player type
    cout << "Race: " << ptype;
    cout << " Gold: " << player->getGold()/2;
    if (player->getGold() % 2 == 1){
        cout << ".5";
    }
    // print the right number of spaces
    for (int i = 0; i<( Width- ptype.length()- 12 - 9 - ((player->getGold())/10) - 1 );i++){
        cout << " ";
    }
    cout << "Floor: " << floornum<< endl;
    
    cout << "HP: " << player->getHP() << endl;
    cout << "Atk: " << player->getAtk() << endl;
    cout << "Def: " << player->getDef() << endl;
    cout << "Action: " << action << endl;
	
	action = "";
}

void Floor::increaseHP(int n){
    player->increaseHP(n);
}


void Floor::updateCurrent(){
    copyToCurrent(false);
    // add players coordinates
        current[player->getRow()][player->getColumn()] = '@';
    
    // add enemies coordinates
    for (int i = 0; i < numEnemies; i++){
        if (enemies[i])
		    current[enemies[i]->getRow()][enemies[i]->getColumn()] = enemies[i] -> getType();
    }
	
	for (int i = 0; i < numTreasure; i++){
		if (treasure[i])
		    current[treasure[i]->getRow()][treasure[i]->getColumn()] = 'G';
	}
	
    for (int i = 0; i < numPotion; i++){
        if (potion[i])
            current[potion[i]->getRow()][potion[i]->getColumn()] = 'P';
    }
    
    current[stairR][stairC] = '\\';
};

bool Floor::returnStatus(){
    return done;
}

Floor::~Floor(){
    for(int i = 0; i < Height; i++)
    {
        delete [] original[i];
        delete [] current[i];
    }
    delete [] original;
    delete [] current;
    
    for(int i = 0; i < maxChambers; i++){
        if (chamber[i]!=0){
			for(int j = 0; j < Height ; j++){
				delete [] chamber[i][j];
			}
		}
        if (chamber[i]!=0) delete [] chamber[i];
    }
    delete [] chamber;
    
    for(int i = 0; i < numEnemies; i++){
		delete enemies[i];
    }
    delete [] enemies;
    
    for(int i = 0; i < numPotion; i++){
        delete potion[i];
    }
    delete [] potion;
    
    for(int i = 0; i < numTreasure; i++){
        delete treasure[i];
    }
    delete [] treasure;
    
    HPPotion::setNotPosSeen();
    HPPotion::setNotNegSeen();
    TempPotion::setNotBaSeen();
    TempPotion::setNotWaSeen();
    TempPotion::setNotBdSeen();
    TempPotion::setNotWdSeen();
        
	Player::cleanup();
}

void Floor::usePotion(Character* p){
	char type = p->getType();
	p->action(player);
	int row = p->getRow();
	int col = p->getColumn();
	string potName = "";
	for(int i = 0; i < numPotion; i++){
		if (potion[i]!=0 && potion[i]->getRow() == row && potion[i]->getColumn() == col){
			potion[i] = NULL;
		}
	}
	if(type == 'I' || type == 'i'){
		delete p;
	}
	//Set seen to true for the specific potion:
	switch(type){
		case 'I':
			HPPotion::setPosSeen();
			potName = "RH";
			break;
		case 'i':
			HPPotion::setNegSeen();
			potName = "PH";
			break;
		case 'Q':
			TempPotion::setBaSeen();
			potName = "BA";
			break;
		case 'q':
			TempPotion::setWaSeen();
			potName = "WA";
			break;
		case 'R':
			TempPotion::setBdSeen();
			potName = "BD";
			break;
		case 'r':
			TempPotion::setWdSeen();
			potName = "WD";
			break;
	}
	action += ("PC uses " + potName + ".");
	turn();
}

void Floor::attackEnemy(Character *e){
    int damage = ceil((100 * player->getAtk() / (100 + e->getDef())));
	bool tmp = player->attack(*e);
	int health = e->getHP();
	ostringstream damageString;
	ostringstream healthString;
	damageString << damage;
	healthString << health;
	action += "PC deals ";
	action += damageString.str(); 
	action += " damage to ";
	action += e->getType();
	action += " (";
	action += healthString.str();
	action += " HP).";
	char type = e->getType();
	if(type == 'M'){
		for(int i = 0; i < numEnemies; i++){
			if(enemies[i])
			    enemies[i]->changeHostility();
		}
	}
	int row = e->getRow();
	int col = e->getColumn();
	if(type != 'D'){
		if(e->getHP() == 0){
			for(int i = 0; i < numEnemies; i++){
				if (enemies[i] && enemies[i]->getRow() == row && enemies[i]->getColumn() == col){
					enemies[i] = NULL;
				}
			}
			delete e;
			if(type == 'M'){
				for(int i = 0; i < numTreasure; i++){
					if(treasure[i] == NULL){
						treasure[i] = new Treasure(4, row, col);
						break;
					}
				}
			}
			else{
				for(int i = 0; i < numTreasure; i++){
					if(treasure[i] == NULL){
						treasure[i] = new Treasure(1, row, col);
						break;
					}
				}
			}
		}
	}
	else{
		if(e->getHP() == 0){
			Dragon* drag = static_cast<Dragon *>(e);
			drag->notifyHorde();				
	
			for(int i = 0; i < numEnemies; i++){
				if (enemies[i] && enemies[i]->getRow() == row && enemies[i]->getColumn() == col){
					enemies[i] = NULL;
				}
			}
			drag = 0;
			delete e;
		}
	}
	turn();
}

Character* Floor::findAtLocation(int r, int c){
	for(int i = 0; i < numEnemies; i++){
		if(enemies[i]!=0 && enemies[i]->getRow() == r && enemies[i]->getColumn() == c){
			return enemies[i];
		}
	}
	for(int i = 0; i < numPotion; i++){
		if(potion[i]!=0 && potion[i]->getRow() == r && potion[i]->getColumn() == c){
			return potion[i];
		}
	}

	return NULL;
}

Character* Floor::findAtLocation(char first, char second){
	int rowChange;
	int colChange;
	int r = player->getRow();
	int c = player->getColumn();

	if (first == 'n'){
		rowChange = -1;
		if(second == 'e')
			colChange = 1;
		else if (second == 'w')
			colChange = -1;
		else
			colChange = 0;
	}
	else if(first == 's'){
		rowChange = 1;
		if(second == 'e')
			colChange = 1;
		else if (second == 'w')
			colChange = -1;
		else
			colChange = 0;
	}
	else if(first == 'e'){
		colChange = 1;
		rowChange = 0;
	}
	else{
		colChange = -1;
		rowChange = 0;
	}

	r = r + rowChange;
	c = c + colChange;

	return findAtLocation(r,c);
}

void Floor::deleteAll(){
	for(int i = 0; i < numEnemies; i++){
		delete enemies[i];
		enemies[i] = NULL;
	}
	for(int i = 0; i < numPotion; i++){
		delete potion[i];
		potion[i] = NULL;
	}
	for(int i = 0; i < numTreasure; i++){
		delete treasure[i];
		treasure[i] = NULL;
	}
	if(player != Player::getInstance()){
	    delete player;
	}
}

void Floor::turn(){
    srand(time(NULL));
    int darray[numEnemies][2];
    bool flag = true;
    for(int i = 0; i < numEnemies; i++){
        if(enemies[i]){
            darray[i][0] = enemies[i]->getRow();
            darray[i][1] = enemies[i]->getColumn();
        }
        else{
            darray[i][0] = 100;
            darray[i][1] = 100;
        }
    }
    while(flag){
        flag = false;
        for(int i = 0; i < numEnemies - 1; i++){
            int tmp1;
            int tmp2;
            if(darray[i][0] > darray[i+1][0]){
                tmp1 = darray[i][0];
                tmp2 = darray[i][1];
                darray[i][0] = darray[i+1][0];
                darray[i][1] = darray[i+1][1];
                darray[i+1][0] = tmp1;
                darray[i+1][1] = tmp2;
                flag = true;
            }
            else if(darray[i][0] == darray[i+1][0]){
                if(darray[i][1] > darray[i+1][1]){
                    tmp2 = darray[i][1];
                    darray[i][1] = darray[i+1][1];
                    darray[i+1][1] = tmp2;
                    flag = true;
                }
            }
        }
    }
    for(int c = 0; c < numEnemies; c++){
        for(int i = 0; i < numEnemies; i++){
            int begHP = player->getHP();
            if(enemies[i]){
                if(enemies[i]->getRow() == darray[c][0] && enemies[i]->getColumn() == darray[c][1]){
                    if(enemies[i]->attack(*player)){
                        ostringstream out;
                        int endHP = player->getHP();
                        int damage = begHP - endHP;
                        out << damage;
                        if(damage == 0){
                            action += " ";
                            action += enemies[i]->getType();
                            action += " tries to attack...but misses!";
                        }
                        else{
                            action += " ";
                            action += enemies[i]->getType();
                            action += " deals ";
                            action += out.str();
                            action += " damage to PC.";
                        }
                    }
                    else{
                        int row = enemies[i]->getRow();
                        int col = enemies[i]->getColumn();
                        int rchange = 0;
                        int cchange = 0;
                        while(true)
                        {
                            if(enemies[i]->getType() == 'D'){
                                break;
                            }
                            int spot = rand() % 8 + 1;
                            switch(spot){
                                case 1:
                                    rchange = 1;
                                    cchange = 1;
                                    break;
                                case 2:
                                    rchange = 1;
                                    cchange = 0;
                                    break;
                                case 3:
                                    rchange = 1;
                                    cchange = -1;
                                    break;
                                case 4:
                                    rchange = 0;
                                    cchange = 1;
                                    break;
                                case 5:
                                    rchange = 0;
                                    cchange = -1;
                                    break;
                                case 6:
                                    rchange = -1;
                                    cchange = 1;
                                    break;
                                case 7:
                                    rchange = -1;
                                    cchange = 0;
                                    break;
                                case 8:
                                    rchange = -1;
                                    cchange = -1;
                                    break;
                            }
                            if(current[row + rchange][col + cchange] == '.'){
                                enemies[i]->setCoordinates(row + rchange, col + cchange);
                                updateCurrent();
                                break;
                            }
                            if(current[row + 1][col + 1] != '.' && current[row + 1][col] != '.' && current[row + 1][col - 1] != '.' && current[row][col + 1] != '.' && current[row][col - 1] != '.' && current[row - 1][col + 1] != '.' && current[row - 1][col] != '.' && current[row - 1][col - 1] != '.')
                                break; //if there is no possible move
                        }
                    }
                }
            }
        }
    }
    if(player->getHP() <= 0){
        updateCurrent();
        action += " PC is dead. Game over. Press r to restart.";
        done = true;
    }
    print();
    action = ""; //immediately after you print
}

void Floor::move(char first, char second){
    ostringstream out;
    int rowChange;
	int colChange;
	int r = player->getRow();
	int c = player->getColumn();
	string direction = "";
	char newLoc;
	int flag = 1;

	if (first == 'n'){
		rowChange = -1;
		if(second == 'e'){
			colChange = 1;
			direction = "Northeast";
		}
		else if (second == 'w'){
			colChange = -1;
			direction = "Northwest";
		}
		else{
			colChange = 0;
			direction = "North";
		}
	}
	else if(first == 's'){
		rowChange = 1;
		if(second == 'e'){
			colChange = 1;
			direction = "Southeast";
		}
		else if (second == 'w'){
			colChange = -1;
			direction = "Southwest";
		}
		else{
			colChange = 0;
			direction = "South";
		}
	}
	else if(first == 'e'){
		colChange = 1;
		rowChange = 0;
		direction = "East";
	}
	else{
		colChange = -1;
		rowChange = 0;
		direction = "West";
	}

	r = r + rowChange;
	c = c + colChange;
	
	newLoc = current[r][c];
	
	if(newLoc == '.' || newLoc == '+' || newLoc == 'G' || newLoc == '#' || newLoc == '\\'){
	    if(newLoc == '\\'){
	        if(floornum < 8) {
				deleteAll();
				generate();
				print();
			}
			else {
				action += "You have won! Your score is ";
				int score = player->getGold();
				if(player->getType() == 'h')
				    score = score + score/2;
				out << score/2;
                if (score % 2 == 1){
                     out << ".5";
                }
                action += out.str();
                action+= "!\n";
				action += "If you would like to restart a game, enter r and the race you desire, or 'q' to quit.";
				print();
			}
	    }
	    else{
	        ostringstream out;
	        action += ("PC moves " + direction);
	        flag = 1;
	        if(newLoc == 'G'){
	            int begGold = player->getGold();
	            for(int i = 0; i < numTreasure; i++){
	                if(treasure[i]!=0 && treasure[i]->getRow() == r && treasure[i]->getColumn() == c){
	                    if(treasure[i]->getPickedUp(player)){
	                        delete treasure[i];
	                        treasure[i] = NULL;
	                        int endGold = player->getGold();
	                        int change = (endGold - begGold);
            	            out << change/2;
            	            if(change % 2 == 1){
            	                out << ".5";
            	            }
            	            action += " and picks up ";
            	            action += out.str();
            	            action += " gold";
	                    }
	                    else{
	                        action += "...but it fails! PC must kill the dragon first to pick up this Dragon Hoard ";
	                        flag = 0;
	                    }
	                }
	            }
	        }
	        if(flag)
	            player->setCoordinates(r,c);
	        for(int i = 0; i < numPotion; i++){
	          if(potion[i]!=0){
				int row = potion[i]->getRow();
	            int col = potion[i]->getColumn();
	            int r = player->getRow();
	            int c = player->getColumn();
	            char t = potion[i]->getType();
	            
				if(abs(row - r) <= 1 && abs(col - c) <= 1){
				
	                if(t == 'I' && HPPotion::getPosSeen()){
	                    action += " and sees an RH potion";
	                }
	                else if(t == 'i' && HPPotion::getNegSeen()){
	                    action += " and sees an PH potion";
	                }
	                else if(t == 'Q' && TempPotion::getBaSeen()){
	                    action += " and sees an BA potion";
	                }
	                else if(t == 'q' && TempPotion::getWaSeen()){
	                    action += " and sees an WA potion";
	                }
	                else if(t == 'R' && TempPotion::getBdSeen()){
	                    action += " and sees an BD potion";
	                }
	                else if(t == 'r' && TempPotion::getWdSeen()){
	                    action += " and sees an WD potion";
	                }
	                else{
	                    action += " and sees an unknown potion";
	                }
	            }
			  }
	        }
	        action += ".";
	        updateCurrent();
	        turn();
	    }
	}
	else{
	    cout << "That is not a valid move. Re-enter command: ";
	}
}