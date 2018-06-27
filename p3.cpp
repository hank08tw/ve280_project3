#include <iostream>
#include <fstream>
#include <sstream>
#include "world_type.h"
#include "simulation.h"
using namespace std;
int main(int argc,char* argv[]){
    world_t theworld;
    //check whether the number of arguments is less than three
    if(argc<4){
        cout << "Error: Missing arguments!" << endl;
        cout << "Usage: ./p3 <species-summary> <world-file> <rounds> [v|verbose]" << endl;
        return -1;
    }

    //check whether round supplied by user is negative
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


}