#include "compitum/apparel.hpp"

namespace compitum {

bool operator==(apparel a, apparel b) {
    return a.armor_name             == b.armor_name
        && a.damage_buff            == b.damage_buff
        && a.placement              == b.placement
        && a.weight                 == b.weight
        && a.damage_sustained       == b.damage_sustained
        && a.value                  == b.value;
}

//TODO: make special buffs to special attacks, i.e. pugilist gloves enhance unarmed strike
const apparel
    bare_hands{ "bare hands", damage_type::bludgeoning, body_part::hands, 0, 0, 0 },
    pugilist_gloves{ "pugilist gloves", damage_type::bludgeoning, body_part::hands, 2, 0, 5 };
 
 }
