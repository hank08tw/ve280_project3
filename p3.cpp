#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string.h>
#include "world_type.h"
#include "simulation.h"
using namespace std;
int main(int argc,char* argv[]){
    world_t theworld;

    //check whether the number of arguments is less than three
    if(argc<4){
        cout << "Error: Missing arguments!" << endl;
        cout << "Usage: ./p3 <species-summary> <world-file> <rounds> [v|verbose]" << endl;
        return 0;
    }

    //check whether it is normal printing or verbose printing
    bool print_normal=true;

    if(argc>=5){
        if(strcmp(argv[4],"v")||strcmp(argv[4],"verbose")){
            print_normal=false;
        }
    }

    //check whether round supplied by user is negative
    if(atoi(argv[3])<0){
        cout << "Error: Number of simulation rounds is negative!" << endl;
        return 0;
    }

    //read summaryfile and speciesfiles in the directory
    if(read_summaryfile_and_speciesfiles(theworld,argv[1])==-1){
        return 0;
    }

    //read worldfile
    if(read_world_file (theworld,argv[2])==-1){
        return 0;
    }

    //print out all the output
    print_all(theworld,atoi(argv[3]),print_normal);
    return 0;
}