#include "simulation.h"
#include "world_type.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include "world_type.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
int read_summaryfile_and_speciesfiles(world_t& theworld,const string& summary_file_name){
    ifstream iFile;
    iFile.open(summary_file_name);
    //if open creature summary file fails
    if(!iFile){
        cout << "Error: Cannot open file "<< summary_file_name << "!" << endl;
        return -1;
    }
    //store creature directory name
    string directory_name;
    getline(iFile,directory_name);
    //cout << "directory_name: " << directory_name << endl;
    //store species name
    theworld.numSpecies=0;
    string tmp_species_name;
    while(getline(iFile,tmp_species_name)){
        if(!tmp_species_name.empty()&&theworld.numSpecies<MAXSPECIES){
            theworld.species[theworld.numSpecies++].name=tmp_species_name;
            //cout << "species name: "<< theworld.species[theworld.numSpecies-1].name << endl;
        }else break;
        tmp_species_name.clear();
    }
    //check whether the number of species exceeds the maximal number ok
    if(theworld.numSpecies==MAXSPECIES&&!tmp_species_name.empty()){
        cout << "Error: Too many species!" << endl;
        cout << "Maximal number of species is " << MAXSPECIES<< "." << endl;
        return -1;
    }
    iFile.close ();
    //read instructions for different species
    for(int i=0;i<theworld.numSpecies;i++){
        string tmp_path=directory_name+"/"+theworld.species[i].name;
        iFile.open(tmp_path);
        //check whether fail to open file
        if(!iFile){
            cout << "Error: Cannot open file "<< tmp_path << "!" << endl;
            return -1;
        }
        theworld.species[i].programSize=0;
        string tmp_instruction;
        tmp_instruction.clear();
        while(getline(iFile,tmp_instruction)){
            if(tmp_instruction.empty()||theworld.species[i].programSize==MAXPROGRAM){
                break;
            }
            istringstream istream;
            istream.str(tmp_instruction);
            string first;
            istream >> first;
            bool invalid=true;
            int j;
            for(j=0;j<OP_SIZE;j++){
                if(first==opName[j]){
                    invalid=false;
                    break;
                }
            }
            if(invalid){
                cout << "Error: Instruction " << first <<" is not recognized!" << endl;
                return -1;
            }
            if(HOP<=j&&j<=INFECT){
                switch(j){
                    case HOP:
                        theworld.species[i].program[theworld.species[i].programSize++].op=HOP;
                        break;
                    case LEFT:
                        theworld.species[i].program[theworld.species[i].programSize++].op=LEFT;
                        break;
                    case RIGHT:
                        theworld.species[i].program[theworld.species[i].programSize++].op=RIGHT;
                        break;
                    case INFECT:
                        theworld.species[i].program[theworld.species[i].programSize++].op=INFECT;
                        break;
                    default:
                        return -1;
                }
            }else{
                istream >> theworld.species[i].program[theworld.species[i].programSize].address;
                switch(j){
                    case IFEMPTY:
                        theworld.species[i].program[theworld.species[i].programSize++].op=IFEMPTY;
                        break;
                    case IFENEMY:
                        theworld.species[i].program[theworld.species[i].programSize++].op=IFENEMY;
                        break;
                    case IFSAME:
                        theworld.species[i].program[theworld.species[i].programSize++].op=IFSAME;
                        break;
                    case IFWALL:
                        theworld.species[i].program[theworld.species[i].programSize++].op=IFWALL;
                        break;
                    case GO:
                        theworld.species[i].program[theworld.species[i].programSize++].op=GO;
                        break;
                    default:
                        return -1;
                }
            }
            tmp_instruction.clear();
        }
        //check whether number of instructions in a species exceeds maximal value ok
        if(theworld.species[i].programSize==MAXPROGRAM&&!tmp_instruction.empty()){
            cout << "Error: Too many instructions for species " << theworld.species[i].name << "!" << endl;
            cout << "Maximal number of instructions is " << MAXPROGRAM << "." << endl;
            return -1;
        }
        iFile.close ();
    }
    return 0;
}
int read_world_file(world_t& theworld,const string& world_file_name){
    //read in height and width of the terrain
    string height,width;
    int height_num,width_num;
    ifstream iFile2;
    iFile2.open(world_file_name);
    //check whether fail to open world file
    if(!iFile2){
        cout << "Error: Cannot open file "<< world_file_name << "!" << endl;
        return -1;
    }
    getline(iFile2,height);
    istringstream istream;
    istream.str(height);
    istream >> height_num;
    //check whether grid height is legal ok
    if(height_num<1||height_num>MAXHEIGHT){
        cout << "Error: The grid height is illegal!" << endl;
        return -1;
    }
    theworld.grid.height=(unsigned int)height_num;
    getline(iFile2,width);
    istringstream istream2;
    istream2.str(width);
    istream2 >> width_num;
    //check whether grid width is legal ok
    if(width_num<1||width_num>MAXWIDTH){
        cout << "Error: The grid width is illegal!" << endl;
        return -1;
    }
    theworld.grid.width=(unsigned int)width_num;
    //read in terrain type for each square
    for(int i=0;i<height_num;i++){
        string line;
        getline(iFile2,line);
        for(int j=0;j<line.length();j++){
            bool invalid = true;
            int k;
            for(k=0;k<TERRAIN_SIZE;k++){
                if(line[j]==terrainShortName[k][0]){
                    invalid=false;
                    break;
                }
            }
            //check whether array describing the terrain layout is correct ok
            if(invalid){
                cout << "Error: Terrain square (" << line[j] << " " << i << " " << j << ") is invalid!" << endl;
                return -1;
            }
            switch(k){
                case PLAIN:
                    theworld.grid.terrain[i][j]=PLAIN;
                    break;
                case LAKE:
                    theworld.grid.terrain[i][j]=LAKE;
                    break;
                case FOREST:
                    theworld.grid.terrain[i][j]=FOREST;
                    break;
                case HILL:
                    theworld.grid.terrain[i][j]=HILL;
                    break;
                default:
                    return -1;
            }
        }
    }
    //read in creature information
    theworld.numCreatures=0;
    string species_line;
    while(getline(iFile2,species_line)){
        if(species_line.empty()||theworld.numCreatures==MAXCREATURES){
            break;
        }
        istringstream istream3;
        istream3.str(species_line);
        string species_name;
        istream3 >> species_name;
        bool invalid_creature_name=true;
        //species_num represents the species
        int species_num;
        for(species_num=0;species_num<theworld.numSpecies;species_num++){
            if(species_name==theworld.species[species_num].name){
                invalid_creature_name=false;
                break;
            }
        }
        //check whether the species exists ok
        if(invalid_creature_name){
            cout << "Error: Species " << species_name << " not found!" << endl;
            return -1;
        }
        string direction;
        istream3 >> direction;
        bool invalid_direction=true;
        //direction_num represents the direction
        int direction_num;
        for(direction_num=0;direction_num<DIRECT_SIZE;direction_num++){
            if(direction==directName[direction_num]){
                invalid_direction=false;
                break;
            }
        }
        //check whether direction exists ok
        if(invalid_direction){
            cout << "Error: Direction " << direction << " is not recognized!" << endl;
            return -1;
        }

        int r,c;
        istream3 >> r;
        istream3 >> c;
        //check whether creature is out of boundary ok
        if(r>=theworld.grid.height||c>=theworld.grid.width){
            cout << "Error: Creature (" << species_name << " " << direction << " " << r << " " << c << ") is out of bound!" << endl;
            cout << "The grid size is " << theworld.grid.height << "-by-" << theworld.grid.width << "." << endl;
            return -1;
        }
        //check whether different creatures overlap together
        if(theworld.grid.squares[r][c]!= nullptr){
            cout << "Error: Creature (" << species_name << " " << direction << " " << r << " " << c <<") overlaps with creature ("<< theworld.grid.squares[r][c]->species->name << " " << directName[theworld.grid.squares[r][c]->direction] << " " << r << " " << c << ")!" << endl;
            return -1;
        }

        theworld.creatures[theworld.numCreatures].species=&theworld.species[species_num];
        theworld.creatures[theworld.numCreatures].location.r=r;
        theworld.creatures[theworld.numCreatures].location.c=c;
        theworld.grid.squares[r][c]=&theworld.creatures[theworld.numCreatures];
        switch(direction_num){
            case 0:
                theworld.creatures[theworld.numCreatures].direction=EAST;
                break;
            case 1:
                theworld.creatures[theworld.numCreatures].direction=SOUTH;
                break;
            case 2:
                theworld.creatures[theworld.numCreatures].direction=WEST;
                break;
            case 3:
                theworld.creatures[theworld.numCreatures].direction=NORTH;
                break;
            default:
                return -1;
        }
        //handle creature's abilities
        theworld.creatures[theworld.numCreatures].ability[ARCH]=false;
        theworld.creatures[theworld.numCreatures].ability[FLY]=false;

        string ability;
        while(!istream3.eof()){
            istream3 >> ability;
            //check whether there exists invalid ability ok
            if(ability.length()>1||((ability[0]!='f')&&ability[0]!='a')){
                cout << "Error: Creature (" << species_name << " " <<  direction << " " << theworld.creatures[theworld.numCreatures].location.r << " " << theworld.creatures[theworld.numCreatures].location.c << ") has an invalid ability " << ability << "!" << endl;
                return -1;
            }
            if(ability[0]=='a'){
                theworld.creatures[theworld.numCreatures].ability[ARCH]=true;
            }
            else{
                theworld.creatures[theworld.numCreatures].ability[FLY]=true;
            }
        }

        //check whether creature that cannot fly lies in lake terrain
        if(!theworld.creatures[theworld.numCreatures].ability[FLY]&&theworld.grid.terrain[r][c]==LAKE){
            cout << "Error: Creature (" << species_name << " " << direction << " " << r << " " << c << ") is in a lake square!" << endl;
            cout << "The creature cannot fly!" << endl;
            return -1;
        }
        theworld.creatures[theworld.numCreatures].programID=1;
        theworld.creatures[theworld.numCreatures].hillActive=true;
        //if the species cannot fly and it's in hill terrain, it can only move once for any two rounds.
        if(!theworld.creatures[theworld.numCreatures].ability[FLY]&&theworld.grid.terrain[r][c]==HILL){
            theworld.creatures[theworld.numCreatures].hillActive= false;
        }
        theworld.numCreatures++;
        species_line.clear();
    }
    if(theworld.numCreatures==MAXCREATURES&&!species_line.empty()){
        cout << "Error: Too many creatures!" << endl;
        cout << "Maximal number of creatures is " << MAXCREATURES << "." << endl;
        return -1;
    }
    iFile2.close ();
    return 0;
}
int hop(world_t& theworld,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return -1;
    }
    int last_program_id=thecreature.programID;
    switch(thecreature.direction){
        case EAST:
            if(thecreature.location.c+1<theworld.grid.width&&!(!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c+1]==LAKE)&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]==nullptr){
                theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]=&thecreature;
                theworld.grid.squares[thecreature.location.r][thecreature.location.c]=nullptr;
                thecreature.location.c+=1;

            }
            //thecreature.programID++;
            break;
        case SOUTH:
            if(thecreature.location.r+1<theworld.grid.height&&!(!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r+1][thecreature.location.c]==LAKE)&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]==nullptr){

                theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]=&thecreature;
                theworld.grid.squares[thecreature.location.r][thecreature.location.c]=nullptr;
                thecreature.location.r+=1;
                //thecreature.programID++;
            }
            //thecreature.programID++;
            break;
        case WEST:
            if(thecreature.location.c-1>=0&&!(!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c-1]==LAKE)&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]==nullptr){

                theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]=&thecreature;
                theworld.grid.squares[thecreature.location.r][thecreature.location.c]=nullptr;
                thecreature.location.c-=1;
                //thecreature.programID++;
            }
            //thecreature.programID++;
            break;
        case NORTH:
            if(thecreature.location.r-1>=0&&!(!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r-1][thecreature.location.c]==LAKE)&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]==nullptr){
                theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]=&thecreature;
                theworld.grid.squares[thecreature.location.r][thecreature.location.c]=nullptr;
                thecreature.location.r-=1;
                //thecreature.programID++;
            }
            //thecreature.programID++;
            break;
        default:
            return -1;
    }
    thecreature.programID++;
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    if(theworld.creatures[creature_num].hillActive&&!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c]==HILL){
        theworld.creatures[creature_num].hillActive=false;
    }
    return last_program_id;
}
int turn_left(world_t& theworld,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return -1;
    }
    int last_program_id=thecreature.programID;
    switch(thecreature.direction){
        case EAST:
            thecreature.direction=NORTH;
            break;
        case SOUTH:
            thecreature.direction=EAST;
            break;
        case WEST:
            thecreature.direction=SOUTH;
            break;
        case NORTH:
            thecreature.direction=WEST;
            break;
        default:
            return -1;
    }
    thecreature.programID++;
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    //this creature cannot act on next round
    if(theworld.creatures[creature_num].hillActive&&!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c]==HILL){
        theworld.creatures[creature_num].hillActive=false;
    }
    return last_program_id;
}
int turn_right(world_t& theworld,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!theworld.creatures[creature_num].hillActive){
        theworld.creatures[creature_num].hillActive=true;
        return -1;
    }
    int last_program_id=thecreature.programID;
    switch(thecreature.direction){
        case EAST:
            thecreature.direction=SOUTH;
            break;
        case SOUTH:
            thecreature.direction=WEST;
            break;
        case WEST:
            thecreature.direction=NORTH;
            break;
        case NORTH:
            thecreature.direction=EAST;
            break;
        default:
            return -1;
    }
    thecreature.programID++;
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    //this creature cannot act on next round
    if(theworld.creatures[creature_num].hillActive&&!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c]==HILL){
        theworld.creatures[creature_num].hillActive=false;
    }
    return last_program_id;
}
int infect(world_t& theworld,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!theworld.creatures[creature_num].hillActive){
        theworld.creatures[creature_num].hillActive=true;
        return -1;
    }
    int last_program_id=thecreature.programID;
    if(!thecreature.ability[ARCH]){
        switch(int(thecreature.direction)){
            case EAST:
                if(thecreature.location.c+1<theworld.grid.width&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c+1]!=FOREST&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r&&theworld.creatures[i].location.c==thecreature.location.c+1){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            case SOUTH:
                if(thecreature.location.r+1<theworld.grid.height&&theworld.grid.terrain[thecreature.location.r+1][thecreature.location.c]!=FOREST&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r+1&&theworld.creatures[i].location.c==thecreature.location.c){
                            //change its species and set program id to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            case WEST:
                if(thecreature.location.c-1>=0&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c-1]!=FOREST&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r&&theworld.creatures[i].location.c==thecreature.location.c-1){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            case NORTH:
                if(thecreature.location.r-1>=0&&theworld.grid.terrain[thecreature.location.r-1][thecreature.location.c]!=FOREST&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r-1&&theworld.creatures[i].location.c==thecreature.location.c){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            default:
                return -1;
        }
    }else{
        //if the creature has arch ability
        int length=1;
        switch(thecreature.direction){
            case EAST:
                while(!(thecreature.location.c+length<theworld.grid.width&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+length]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+length]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species)){
                    length++;
                }
                if(thecreature.location.c+length<theworld.grid.width&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+length]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+length]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r][thecreature.location.c+length]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r&&theworld.creatures[i].location.c==thecreature.location.c+length){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            case SOUTH:
                while(!(thecreature.location.r+length<theworld.grid.height&&theworld.grid.squares[thecreature.location.r+length][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r+length][thecreature.location.c]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species)){
                    length++;
                }
                if(thecreature.location.r+length<theworld.grid.height&&theworld.grid.squares[thecreature.location.r+length][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r+length][thecreature.location.c]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r+length][thecreature.location.c]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r+length&&theworld.creatures[i].location.c==thecreature.location.c){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            case WEST:
                while(!(thecreature.location.c-length>=0&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-length]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-length]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species)){
                    length++;
                }
                if(thecreature.location.c-length>=0&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-length]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-length]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r][thecreature.location.c-length]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r&&theworld.creatures[i].location.c==thecreature.location.c-length){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            case NORTH:
                while(!(thecreature.location.r-length>=0&&theworld.grid.squares[thecreature.location.r-length][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r-length][thecreature.location.c]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species)){
                    length++;
                }
                if(thecreature.location.r-length>=0&&theworld.grid.squares[thecreature.location.r-length][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r-length][thecreature.location.c]->species!=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species){
                    theworld.grid.squares[thecreature.location.r-length][thecreature.location.c]->species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                    //find the creature that was infected
                    for(int i=0;i<theworld.numCreatures;i++){
                        if(theworld.creatures[i].location.r==thecreature.location.r-length&&theworld.creatures[i].location.c==thecreature.location.c){
                            //change its species and set programid to one
                            theworld.creatures[i].species=theworld.grid.squares[thecreature.location.r][thecreature.location.c]->species;
                            theworld.creatures[i].programID=1;
                            break;
                        }
                    }
                    //thecreature.programID++;
                }
                break;
            default:
                return -1;
        }

    }
    thecreature.programID++;
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    if(theworld.creatures[creature_num].hillActive&&!thecreature.ability[FLY]&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c]==HILL){
        theworld.creatures[creature_num].hillActive=false;
    }
    return last_program_id;
}
bool ifempty(world_t& theworld,int n,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return true;
    }
    switch(int(thecreature.direction)){
        case EAST:
            if(thecreature.location.c+1<theworld.grid.width&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]==nullptr){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
        case SOUTH:
            if(thecreature.location.r+1<theworld.grid.height&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]==nullptr){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
        case WEST:
            if(thecreature.location.c-1>=0&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]==nullptr){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
        case NORTH:
            if(thecreature.location.r-1>=0&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]==nullptr){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
    }
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    return false;
}
bool ifenemy(world_t& theworld,int n,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return true;
    }
    switch(int(thecreature.direction)){
        case EAST:
            if(thecreature.location.c+1<theworld.grid.width&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c+1]!=FOREST&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]->species!=thecreature.species){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
        case SOUTH:
            if(thecreature.location.r+1<theworld.grid.height&&theworld.grid.terrain[thecreature.location.r+1][thecreature.location.c]!=FOREST&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]->species!=thecreature.species){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
        case WEST:
            if(thecreature.location.c-1>=0&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c-1]!=FOREST&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]->species!=thecreature.species){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
        case NORTH:
            if(thecreature.location.r-1>=0&&theworld.grid.terrain[thecreature.location.r-1][thecreature.location.c]!=FOREST&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]->species!=thecreature.species){
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
    }
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    return false;
}
bool ifsame(world_t& theworld,int n,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return true;
    }
    //cout << thecreature.species->name << endl;
    switch(int(thecreature.direction)){
        case EAST:
            if(thecreature.location.c+1<theworld.grid.width&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c+1]!=FOREST&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c+1]->species==thecreature.species){
                thecreature.programID=(unsigned int)n;
                //cout << "eeast" << endl;
            }else thecreature.programID++;
            break;
        case SOUTH:
            if(thecreature.location.r+1<theworld.grid.height&&theworld.grid.terrain[thecreature.location.r+1][thecreature.location.c]!=FOREST&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r+1][thecreature.location.c]->species==thecreature.species){
                thecreature.programID=(unsigned int)n;
                //cout << "ssame" << endl;
            }else thecreature.programID++;
            break;
        case WEST:
            if(thecreature.location.c-1>=0&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c-1]!=FOREST&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]!=nullptr&&theworld.grid.squares[thecreature.location.r][thecreature.location.c-1]->species==thecreature.species){
                thecreature.programID=(unsigned int)n;
                //cout << "wsame" << endl;
            }else thecreature.programID++;
            break;
        case NORTH:
            if(thecreature.location.r-1>=0&&theworld.grid.terrain[thecreature.location.r-1][thecreature.location.c]!=FOREST&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]!=nullptr&&theworld.grid.squares[thecreature.location.r-1][thecreature.location.c]->species==thecreature.species){
                //cout << "nsame" << endl;
                thecreature.programID=(unsigned int)n;
            }else thecreature.programID++;
            break;
    }
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    return false;
}
bool ifwall(world_t& theworld,int n,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return true;
    }
    //cout << thecreature.species->name << endl;
    switch(int(thecreature.direction)){
        case EAST:
            if(thecreature.location.c+1==theworld.grid.width||(thecreature.location.c<theworld.grid.width&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c+1]==LAKE&&!thecreature.ability[FLY])){
                thecreature.programID=(unsigned int)n;
                //cout << "ewall" << endl;
            }else thecreature.programID++;
            break;
        case SOUTH:
            if(thecreature.location.r+1==theworld.grid.height||(thecreature.location.r<theworld.grid.height&&theworld.grid.terrain[thecreature.location.r+1][thecreature.location.c]==LAKE&&!thecreature.ability[FLY])){
                thecreature.programID=(unsigned int)n;
                //cout << "swall" << endl;
            }else thecreature.programID++;
            break;
        case WEST:
            if(thecreature.location.c==0||(thecreature.location.c>0&&theworld.grid.terrain[thecreature.location.r][thecreature.location.c-1]==LAKE&&!thecreature.ability[FLY])){
                thecreature.programID=(unsigned int)n;
                //cout << "wwall" << endl;
            }else thecreature.programID++;
            break;
        case NORTH:
            if(thecreature.location.r==0||(thecreature.location.r>0&&theworld.grid.terrain[thecreature.location.r-1][thecreature.location.c]==LAKE&&!thecreature.ability[FLY])){
                thecreature.programID=(unsigned int)n;
                //cout << "nwall" << endl;
            }else thecreature.programID++;
            break;
    }
    if(thecreature.programID>thecreature.species->programSize)thecreature.programID=1;
    return false;
}
bool go(world_t& theworld,int n,int creature_num){
    creature_t& thecreature=theworld.creatures[creature_num];
    //if it is on a hill and it cannot fly and it is inactive at this round, then don't act at this round and set hillactive to true
    if(!thecreature.hillActive){
        thecreature.hillActive=true;
        return true;
    }
    thecreature.programID=(unsigned int)n;
    return false;
}
void print_terrain(const world_t& theworld){
    for(int i=0;i<theworld.grid.height;i++){
        for(int j=0;j<theworld.grid.width;j++){
            if(theworld.grid.squares[i][j]!=nullptr){
                cout << theworld.grid.squares[i][j]->species->name[0] << theworld.grid.squares[i][j]->species->name[1] << "_" << directShortName[theworld.grid.squares[i][j]->direction];
            }else{
                cout << "____";
            }
            cout << " ";
        }
        cout << endl;
    }
}
void print_all(world_t& theworld, int round_num,bool print_normal){
    //print out the answer
    cout << "Initial state" << endl;
    print_terrain (theworld);
    for(int round=1;round<=round_num;round++){
        cout << "Round " << round << endl;
        //for each round, creature behaves one by one
        for(int creature=0;creature<theworld.numCreatures;creature++){
            //store the last behavior of each creature for concise printing
            int last_program_id=theworld.creatures[creature].species->program[theworld.creatures[creature].programID - 1].op;
            string last_species_name=theworld.creatures[creature].species->name;
            enum direction_t last_direction=theworld.creatures[creature].direction;
            int last_r=theworld.creatures[creature].location.r;
            int last_c=theworld.creatures[creature].location.c;
            //if the instruction is IFEMPTY,IFENEMY,IFSAME,IFWALL,GO, thecreature keeps acting unless they are in hillINactive state
            bool keep=true;
            //only verbose printing should
            if(!print_normal){
                cout << "Creature (" << theworld.creatures[creature].species->name << " " << directName[(int)theworld.creatures[creature].direction] << " " << theworld.creatures[creature].location.r  << " " << theworld.creatures[creature].location.c << ") takes action:" << endl;
            }
            //keep acting unless they act HOP LEFT RIGHT INFECT or in hillINactive state
            while(keep) {
                switch (int(theworld.creatures[creature].species->program[theworld.creatures[creature].programID - 1].op)) {
                    case HOP:
                        //only verbose printing should print out details
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[HOP] << endl;
                        }
                        last_program_id=hop (theworld, creature);
                        keep=false;
                        break;
                    case LEFT:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[LEFT] << endl;
                        }
                        last_program_id=turn_left (theworld, creature);
                        keep=false;
                        break;
                    case RIGHT:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[RIGHT] << endl;
                        }
                        last_program_id=turn_right (theworld, creature);
                        keep=false;
                        break;
                    case INFECT:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[INFECT] << endl;
                        }
                        last_program_id=infect (theworld, creature);
                        keep=false;
                        break;
                    case IFEMPTY:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[IFEMPTY] << " " << theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address << endl;
                        }
                        if(ifempty (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case IFENEMY:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[IFENEMY] << " " << theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address << endl;
                        }
                        if(ifenemy (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case IFSAME:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[IFSAME] << " " << theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address << endl;
                        }
                        if(ifsame (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case IFWALL:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[IFWALL] << " " << theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address << endl;
                        }
                        if(ifwall (theworld, theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                    case GO:
                        if(!print_normal){
                            cout << "Instruction " << theworld.creatures[creature].programID << ": " << opName[GO] << " " << theworld.creatures[creature].species->program[theworld.creatures[creature].programID-1].address << endl;
                        }
                        if(go(theworld,theworld.creatures[creature].species->program[ theworld.creatures[creature].programID-1].address, creature))keep=false;
                        break;
                }
            }
            //print out for concise-printing,
            if(print_normal&&last_program_id!=-1&&0<=int(theworld.creatures[creature].species->program[last_program_id - 1].op)&&int(theworld.creatures[creature].species->program[last_program_id - 1].op)<=3&&(theworld.grid.terrain[theworld.creatures[creature].location.r][theworld.creatures[creature].location.c]!=HILL||theworld.creatures[creature].ability[FLY]||(!theworld.creatures[creature].hillActive))){
                cout << "Creature (" << last_species_name << " " << directName[last_direction] << " " << last_r << " " << last_c << ") takes action: " << opName[int(theworld.creatures[creature].species->program[last_program_id -1].op)] << endl;
            }
            //print out for verbose-printing
            if(!print_normal){
                print_terrain (theworld);
            }
        }
        //print out for concise-printing
        if(print_normal){
            print_terrain (theworld);
        }
    }
}