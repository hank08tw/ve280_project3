#include <iostream>
#include <fstream>
#include <sstream>
#include "world_type.h"
#include "simulation.h"
using namespace std;
int main(int argc,char* argv[]){
    world_t theworld;
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

    ifstream iFile;
    iFile.open(argv[1]);
    //if open creature summary file fails
    if(!iFile){
        cout << "Error: Cannot open file "<< argv[1] << "!" << endl;
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
                        //cout << "hop" << endl;
                        break;
                    case IFSAME:
                        theworld.species[i].program[theworld.species[i].programSize++].op=IFSAME;
                        //cout << "hop" << endl;
                        break;
                    case IFWALL:
                        theworld.species[i].program[theworld.species[i].programSize++].op=IFWALL;
                        //cout << "hop" << endl;
                        break;
                    case GO:
                        theworld.species[i].program[theworld.species[i].programSize++].op=GO;
                        //cout << "hop" << endl;
                        break;
                    default:
                        return -1;
                }
            }
        }
        //check whether number of instructions in a species exceeds maximal value ok
        if(theworld.species[i].programSize==MAXPROGRAM&&!tmp_instruction.empty()){
            cout << "Error: Too many instructions for species " << theworld.species[i].name << "!" << endl;
            cout << "Maximal number of instructions is " << MAXPROGRAM << "." << endl;
            return -1;
        }
    }
    string height,width;
    int height_num,width_num;
    ifstream iFile2;
    iFile2.open(argv[2]);
    //check whether fail to open world file 
    if(!iFile2){
        cout << "Error: Cannot open file "<< argv[2] << "!" << endl;
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
    getline(iFile2,width);
    istringstream istream2;
    istream2.str(width);
    istream2 >> width_num;
    //check whether grid width is legal ok
    if(width_num<1||width_num>MAXWIDTH){
        cout << "Error: The grid width is illegal!" << endl;
        return -1;
    }

    iFile2.close ();
}