#include "simulation.h"
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
    //store species name
    theworld.numSpecies=0;
    string tmp_species_name;
    while(getline(iFile,tmp_species_name)){
        if(!tmp_species_name.empty()&&theworld.numSpecies<MAXSPECIES){
            theworld.species[theworld.numSpecies++].name=tmp_species_name;
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
                if(first==opName[i]){
                    invalid=false;
                    break;
                }
            }

            //check whether instructions are valid ok
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
    //read in terrain tyoe for each square
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
        int i;
        for(i=0;i<theworld.numSpecies;i++){
            if(species_name==theworld.species[i].name){
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
        for(i=0;i<DIRECT_SIZE;i++){
            if(direction==directName[i]){
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

        theworld.creatures[theworld.numCreatures].species=&theworld.species[i];
        theworld.creatures[theworld.numCreatures].location.r=r;
        theworld.creatures[theworld.numCreatures].location.c=c;
        theworld.grid.squares[r][c]=&theworld.creatures[theworld.numCreatures];
        switch(i){
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
        theworld.creatures[theworld.numCreatures].hillActive=false;
        if(!theworld.creatures[theworld.numCreatures].ability[FLY]&&theworld.grid.terrain[r][c]==HILL){
            theworld.creatures[theworld.numCreatures].hillActive= true;
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