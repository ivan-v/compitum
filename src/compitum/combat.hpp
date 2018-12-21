#ifndef COMPITUM_COMBAT_INCLUDED
#define COMPITUM_COMBAT_INCLUDED

#include "compitum/character.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

namespace compitum {

int attempt_strike(character& self, character& target, 
                      int distance_between_characters);

int attempt_block(character& self);

int player_action(int milliseconds_allowed);

int player_action();

void enemy_action(character& self, character& enemy, 
                  int distance_between_characters, int block);

}



#endif