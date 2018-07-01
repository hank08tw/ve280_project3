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
    //check whether it is normal printing or verbose printing
    bool print_normal=true;
    if(argc>=5){
        if(argv[4]=="v"||argv[4]=="verbose"){
            print_normal=false;
        }
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
    //print out the answer
    cout << "Initial state" << endl;
    print_terrain (theworld);
    for(int round=1;round<=atoi(argv[3]);round++){
        cout << "Round " << round << endl;
        for(int creature=0;creature<theworld.numCreatures;creature++){
            //cout << "creature: " << creature << "species name:" << theworld.creatures[creature].species->name << endl;
            int last_program_id=theworld.creatures[creature].species->program[theworld.creatures[creature].programID - 1].op;
            string last_species_name=theworld.creatures[creature].species->name;
            enum direction_t last_direction=theworld.creatures[creature].direction;
            int last_r=theworld.creatures[creature].location.r;
            int last_c=theworld.creatures[creature].location.c;
            bool keep=true;
            /*
            if(round==2){
                cout << "222" << endl;
            }
             */
            while(keep) {
                //if(round==2){cout << theworld.creatures[creature].programID << endl;}
                switch (int(theworld.creatures[creature].species->program[theworld.creatures[creature].programID - 1].op)) {
                    case HOP:
                        //if(round==2){cout << "hop" << endl;}
                        last_program_id=hop (theworld, creature);
                        keep=false;
                        break;
                    case LEFT:
                        last_program_id=turn_left (theworld, creature);
                        keep=false;
                        break;
                    case RIGHT:
                        last_program_id=turn_right (theworld, creature);
                        keep=false;
                        break;
                    case INFECT:
                        last_program_id=infect (theworld, creature);
                        keep=false;
                        break;
                    case IFEMPTY:
                        if(ifempty (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case IFENEMY:
                        if(ifenemy (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case IFSAME:
                        if(ifsame (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case IFWALL:
                        if(ifwall (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case GO:
                        //cout << "go" << endl;
                        if(go(theworld,theworld.creatures[creature].species->program[ theworld.creatures[creature].programID-1].address, creature)){keep=false;/*cout << "false" << endl;*/}
                        //cout << theworld.creatures[creature].species->program[ theworld.creatures[creature].programID-1].address << endl;//theworld.species[i].program[theworld.species[i].programSize].address;
                        //cout << "true" << endl;
                        break;
                }

            }
            if(last_program_id!=-1&&0<=int(theworld.creatures[creature].species->program[last_program_id - 1].op)&&int(theworld.creatures[creature].species->program[last_program_id - 1].op)<=3&&(theworld.grid.terrain[theworld.creatures[creature].location.r][theworld.creatures[creature].location.c]!=HILL||theworld.creatures[creature].ability[FLY]||(!theworld.creatures[creature].hillActive))){
                cout << "Creature (" << last_species_name << " " << directName[last_direction] << " " << last_r << " " << last_c << ") takes action: " << opName[int(theworld.creatures[creature].species->program[last_program_id -1].op)] << endl;
            }
        }
        print_terrain (theworld);
    }
    return 0;
}