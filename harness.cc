//#include "character.h"
#include "variable.h"
//#include "treasure.h"
#include "floor.h" 
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std; 



int main(int argc, char *argv[]){
    
	init();
    fstream layout;
    Floor* myfloor=0;
    
    char command, temp; // 
    
    
    if(argc > 1) {
		layout.open(argv[1],std::fstream::in);
	}
	else {
		layout.open("layout.txt",std::fstream::in);
	}
	cout << "Please select a race (h, d, e, o): ";
    cin >> command;
    if (command=='q') return 0;
    if (command == 'r') cin>>command;
    myfloor = new Floor(layout, command);
    layout.close();
    Character* myChar;
    
    while(true){
        cin >> command;
        if(!myfloor->returnStatus()){
            if (command == 'u') {
                cin >> command >> temp;
                myChar = myfloor->findAtLocation(command, temp);
                if (myChar!=0) myfloor->usePotion(myChar);
                else cout<< "There's nothing to use there! Re-enter command: ";
    //            myfloor->print();
            }
            else if (command == 'a') {
                cin >> command >> temp;
                myChar = myfloor->findAtLocation(command, temp);
                if (myChar!=0) myfloor->attackEnemy(myChar);
                else cout<< "There's nothing to attack there! Re-enter command: ";
    //            myfloor->print();
            }
            else if (command == 'r') {
    			if(argc > 1) {
    				layout.open(argv[1],std::fstream::in);
    			}
    			else {
    				layout.open("layout.txt",std::fstream::in);
    			}
                delete myfloor;
                cin >> command;
                myfloor = new Floor(layout, command);
    //            myfloor->print();
    			layout.close();
            }
            else if (command == 'q') {
                cout << "Game has ended." << endl;
                delete myfloor;
                break;
            }
            else {
                cin >> temp;
                myfloor->move(command, temp);
    //            myfloor->print();
            }
        }
        else{
            if (command == 'r') {
    			if(argc > 1) {
    				layout.open(argv[1],std::fstream::in);
    			}
    			else {
    				layout.open("layout.txt",std::fstream::in);
    			}
                delete myfloor;
                cout << "Please select a race (h, d, e, o): ";
                cin >> command;
                myfloor = new Floor(layout, command);
    //            myfloor->print();
    			layout.close();
            }
            else if (command == 'q') {
                cout << "Game has ended." << endl;
                delete myfloor;
                break;
            }
        }
        cin.clear();
    }
}