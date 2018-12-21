#ifndef COMPITUM_APPAREL_INCLUDED
#define COMPITUM_APPAREL_INCLUDED

#include "compitum/combat_action.hpp"

#include <string>

namespace compitum {

enum class body_part { head, hands, torso, feet };
	
struct apparel {
    std::string armor_name;
    damage_type damage_buff;
    body_part placement;
    int weight;
    int damage_sustained;
    int value;
};

std::ostream& print(std::ostream&, apparel const&);

bool operator==(apparel, apparel);

inline bool operator!=(apparel a, apparel b) { return !(a == b); }

//TODO: make special buffs to special attacks, i.e. pugilist gloves enhance unarmed strike
extern const apparel bare_hands, pugilist_gloves;

}


#endif