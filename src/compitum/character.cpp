#include "compitum/character.hpp"

#include <cassert>

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
