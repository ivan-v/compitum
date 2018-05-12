#ifndef COMPITUM_CHARACTER_INCLUDED
#define COMPITUM_CHARACTER_INCLUDED

#include "compitum/combat_action.hpp"

#include <chrono>
#include <iostream>
#include <string>

namespace compitum {
	
struct character {
    std::string character_name;
    int stamina;
    int max_stamina;
    int hp;
    int max_hp;
    bool alive;
    combat_action attack_kind;
};

void set_hp(character& c, int p);

void drain_hp(character& c, int p);

void replenish_hp(character& c, int p);

void set_stamina(character& c, int p);

void drain_stamina(character& c, int p);

void replenish_stamina(character& c, int p);

}

#endif