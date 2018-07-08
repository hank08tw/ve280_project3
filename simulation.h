#include "world_type.h"

#ifndef SIMULATION_H
#define SIMULATION_H
int read_summaryfile_and_speciesfiles(world_t& theworld,const string& summary_file_name);
//EFFECT: read summaryfile and speciesfiles in the directory and error-checking

int read_world_file(world_t& theworld,const string& world_file_name);
//EFFECT: read worldfile and error-checking

int hop(world_t& theworld,int creature_num);
//EFFECT: The creature moves forward. If moving forward would put the creature outside the boundaries of the grid, would cause the creature that cannot fly to land in a “lake” square, or would cause the creature to land on top of another creature, the hop instruction does nothing.
//MODIFIES: world.grid.square pointer, creature position, creature programID

int turn_left(world_t& theworld,int creature_num);
//EFFECT: The creature turns left 90 degrees to face in a new direction.
//MODIFIES: creature direction, creature programID

int turn_right(world_t& theworld,int creature_num);
//EFFECT: The creature turns right 90 degrees to face in a new direction.
//MODIFIES: creature direction, creature programID

int infect(world_t& theworld,int creature_num);
//EFFECT: If the square immediately in front of this creature is occupied by a creature of a different species (which we refer to as an “enemy”), that enemy creature is infected to become the same as the infecting species. When a creature is infected, it keeps its position and orientation, but changes its internal species indicator and begins executing the same program as the infecting creature, starting at step 1. If the square immediately in front of this creature is empty, outside the grid, being a forest square, or occupied by a creature of the same species, the infect instruction does nothing. If the creature has the archery ability, its infecting action is different
//MODIFIES: creature species, creature programID

bool ifempty(world_t& theworld,int n,int creature_num);
//EFFECT: If the square in front of the creature is inside the grid boundary and unoccupied, jump to step n of the program; otherwise, go on with the next instruction in sequence.
//MODIFIES: creature programID

bool ifenemy(world_t& theworld,int n,int creature_num);
//EFFECT: If the square the creature is facing is not a “forest” square and is occupied by a creature of an enemy species, jump to step n; otherwise, go on with the next instruction.
//MODIFIES: creature programID

bool ifsame(world_t& theworld,int n,int creature_num);
//EFFECT: If the square the creature is facing is not a “forest” square and is occupied by a creature of the same species, jump to step n; otherwise, go on with the next instruction.
//MODIFIES: creature programID

bool ifwall(world_t& theworld,int n,int creature_num);
//EFFECT: If the creature is facing the border of the grid (which we imagine as consisting of a huge wall), or the creature is facing a lake square and it cannot fly, jump to step n of the program; otherwise, go on with the next instruction in sequence.
//MODIFIES: creature programID

bool go(world_t& theworld,int n,int creature_num);
//EFFECT: This instruction always jumps to step n, independent of any condition.
//MODIFIES: creature programID

void print_terrain(const world_t& theworld);
//EFFECT:print out the information of different creatures on the grid

void print_all(world_t& theworld, int round_num, bool print_normal);
//MODIFIES: all the stuff in theworld
//EFFECT: print out all outputs

#endif//SIMULATION_H