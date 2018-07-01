#include <iostream>
#include <fstream>
#include <sstream>
#include "world_type.h"
#include "simulation.h"
using namespace std;
int main(int argc,char* argv[]){
    world_t theworld;
    //initilize pointer to nullptr
    for(int i=0;i<MAXHEIGHT;i++){
        for(int j=0;j<MAXWIDTH;j++){
            theworld.grid.squares[i][j]= nullptr;
        }
    }
    //check whether the number of arguments is less than three ok
    if(argc<4){
        cout << "Error: Missing arguments!" << endl;
        cout << "Usage: ./p3 <species-summary> <world-file> <rounds> [v|verbose]" << endl;
        return -1;
    }
    //check whether round supplied by user is negative ok
    if(atoi(argv[3])<0){
        cout << "Error: Number of simulation rounds is negative!" << endl;
        return -1;
    }
    if(read_summaryfile_and_speciesfiles(theworld,argv[1])==-1){
        return -1;
    }
    if(read_world_file (theworld,argv[2])==-1){
        return -1;
    }
    for(int round=1;round<=atoi(argv[3]);round++){
        for(int creature=0;creature<theworld.numCreatures;creature++){
            bool keep=true;
            while(keep) {
                switch (theworld.creatures[creature].species->program[theworld.creatures[creature].programID - 1].op) {
                    case HOP:
                        hop (theworld, creature);
                        keep=false;
                        break;
                    case LEFT:
                        turn_left (theworld, creature);
                        keep=false;
                        break;
                    case RIGHT:
                        turn_right (theworld, creature);
                        keep=false;
                        break;
                    case INFECT:
                        infect (theworld, creature);
                        keep=false;
                        break;
                    case IFEMPTY:
                        if(ifempty (theworld, theworld.creatures[creature].species->program->address, creature))keep=false;
                        break;
                    case IFENEMY:
                        if(ifenemy (theworld, theworld.creatures[creature].species->program->address, creature))keep=false;
                        break;
                    case IFSAME:
                        if(ifsame (theworld, theworld.creatures[creature].species->program->address, creature))keep=false;
                        break;
                    case IFWALL:
                        if(ifwall (theworld, theworld.creatures[creature].species->program->address, creature))keep=false;
                        break;
                    case GO:
                        if(go(theworld,theworld.creatures[creature].species->program->address, creature))keep=false;
                        break;
                }
            }
        }
    }
    return 0;
}