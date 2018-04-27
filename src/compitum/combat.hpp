#ifndef COMPITUM_COMBAT_INCLUDED
#define COMPITUM_COMBAT_INCLUDED

#include "compitum/character.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

namespace compitum {

int roll_die(int faces);

int roll_dice(int faces, int times, int modifier);

enum class combat_action_id { unarmed_strike, tusk };

enum class damage_type { bludgeoning, piercing, slashing };

struct combat_action {
    combat_action_id id;
    //std::string name;
    damage_type type;
    int reach_distance; //in meters
    int num_targets;
    int stamina_cost;
    int roll_dice_faces;
    int roll_dice_times;
    int roll_dice_modifier;
};

std::ostream& operator<<(std::ostream&, combat_action const&);

int attempt_strike(character& self, character& target,
                    combat_action attack, int distance_between_characters);

int attempt_block(character& self);

int player_action(int milliseconds_allowed);

void enemy_action(character& self, character& enemy, 
                  int distance_between_characters, int block);

constexpr combat_action
    unarmed_strike{ combat_action_id::unarmed_strike, /*"unarmed strike",*/ damage_type::bludgeoning, 1, 1, 5, 4, 1, 0 },
    tusk{ combat_action_id::tusk, /*"tusk", */ damage_type::slashing, 1, 1, 5, 6, 1, 1 };

}



#endif