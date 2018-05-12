#include "compitum/character.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace compitum;

static apparel& get_apparel(character& c, body_part b) {
    switch (b) {
        case body_part::head: return c.head;
        case body_part::hands: return c.hands;
        case body_part::torso: return c.torso;
        case body_part::feet: return c.feet;
    }
    throw std::logic_error("bad body_part");
}

void compitum::set_hp(character& c, int p) {
    c.hp = p;
}

void compitum::drain_hp(character& c, int p) {
    c.hp -= p;
    if (c.hp < 0)
        c.alive = false;
}

void compitum::replenish_hp(character& c, int p) {
    if ((c.hp + p) <= c.max_hp)
        c.hp += p; 
    else c.hp = c.max_hp;
}

void compitum::set_stamina(character& c, int p) {
    c.stamina = p;
}

void compitum::drain_stamina(character& c, int p) {
    c.stamina = c.stamina >= p ?
    c.stamina - p : 0;

    // if ((c.stamina - p) >= 0)
    //     c.stamina -= p;
    // else c.stamina = 0;
}

void compitum::replenish_stamina(character& c, int p) {
    if ((c.stamina + p) <= c.max_stamina)
        c.stamina += p;
    else c.stamina = c.max_stamina;
}

void compitum::equip_armor(character& c, apparel a) {
    get_apparel(c, a.placement) = a;
}

void compitum::unequip_hand_armor(character& c) {
    c.hands = bare_hands;
}

void compitum::pick_up_apparel(character& c, apparel a) {
    c.character_inventory.apparel_list.push_back(a);
}


void compitum::remove_apparel(character&c, apparel a) {
    auto& v =  c.character_inventory.apparel_list;
    v.erase(std::find(v.begin(), v.end(), a)); 
}