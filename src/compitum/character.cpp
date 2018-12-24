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


void compitum::remove_apparel(character& c, apparel a) {
    auto& v =  c.character_inventory.apparel_list;
    v.erase(std::find(v.begin(), v.end(), a)); 
}

void compitum::player_inventory_action(character& self) {
    std::string input;
    std::cin >> input;
    //if (input == "list apparel")
            //self.inventory.apparel_list;
    /*else*/ if (input == "equip " + pugilist_gloves.armor_name) {
        equip_armor(self, pugilist_gloves);
        std::cout << pugilist_gloves.armor_name << " equipped! \n";
    }

        //     return 2;
        // else if (input == "flee")
        //     return 3;
        // else if (input == "block")
        //     return 4;
}

void compitum::list_apparels_equipped(std::ostream& out, character& c) {
    if (true/*c.torso*/) {
        std::cout << "Equipped on torso: ";
        compitum::print(out, c.torso);      
        std::cout<< ".\n";
    } if (true/*c.feet*/) {
        std::cout << "Equipped on feet: ";
        compitum::print(out, c.feet);
        std::cout<< ".\n"; 
    } if (true/*c.hands*/) {
        std::cout << "Equipped on hands: ";
        compitum::print(out, c.hands);
        std::cout<< ".\n";
    } if (true/*c.head*/) {
        std::cout << "Equipped on head: ";
        compitum::print(out, c.head); 
        std::cout<< ".\n";
    }
}