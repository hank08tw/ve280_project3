#include "world_type.h"

#ifndef SIMULATION_H
#define SIMULATION_H
int read_summaryfile_and_speciesfiles(world_t& theworld,const string& summary_file_name);
int read_world_file(world_t& theworld,const string& world_file_name);
#endif//SIMULATION_H