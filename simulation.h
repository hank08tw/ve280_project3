#include "world_type.h"

#ifndef SIMULATION_H
#define SIMULATION_H
int read_summaryfile_and_speciesfiles(world_t& theworld,const string& summary_file_name);
int read_world_file(world_t& theworld,const string& world_file_name);
void hop(world_t& theworld,int creature_num);
void turn_left(world_t& theworld,int creature_num);
void turn_right(world_t& theworld,int creature_num);
void infect(world_t& theworld,int creature_num);
bool ifempty(world_t& theworld,int n,int creature_num);
bool ifenemy(world_t& theworld,int n,int creature_num);
bool ifsame(world_t& theworld,int n,int creature_num);
bool ifwall(world_t& theworld,int n,int creature_num);
bool go(world_t& theworld,int n,int creature_num);
#endif//SIMULATION_H