#include "world_type.h"

#ifndef SIMULATION_H
#define SIMULATION_H
int read_summaryfile_and_speciesfiles(world_t& theworld,const string& summary_file_name);
//EFFECT: read summaryfile and speciesfiles in the directory and error-checking

int read_world_file(world_t& theworld,const string& world_file_name);
//EFFECT: read worldfile and error-checking

int hop(world_t& theworld,int creature_num);
int turn_left(world_t& theworld,int creature_num);
int turn_right(world_t& theworld,int creature_num);
int infect(world_t& theworld,int creature_num);
bool ifempty(world_t& theworld,int n,int creature_num);
bool ifenemy(world_t& theworld,int n,int creature_num);
bool ifsame(world_t& theworld,int n,int creature_num);
bool ifwall(world_t& theworld,int n,int creature_num);
bool go(world_t& theworld,int n,int creature_num);

void print_terrain(const world_t& theworld);
//EFFECT:print out the information of different creatures on the grid

void print_all(world_t& theworld, int round_num, bool print_normal);
//MODIFIES: all the stuff in theworld
//EFFECT: print out all outputs

#endif//SIMULATION_H