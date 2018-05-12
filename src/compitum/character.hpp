#ifndef COMPITUM_CHARACTER_INCLUDED
#define COMPITUM_CHARACTER_INCLUDED

#include "compitum/combat_action.hpp"
#include "compitum/apparel.hpp"
#include "compitum/inventory.hpp"

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
    apparel torso;
    apparel feet;
    apparel hands;
    apparel head;
    inventory character_inventory;
};

void set_hp(character& c, int p);

void drain_hp(character& c, int p);

void replenish_hp(character& c, int p);

void set_stamina(character& c, int p);

void drain_stamina(character& c, int p);

void replenish_stamina(character& c, int p);

void equip_armor(character& c, apparel a);

void unequip_hand_armor(character& c);

void pick_up_apparel(character& c, apparel a);

void remove_apparel(character&c, apparel a);

}

#endif