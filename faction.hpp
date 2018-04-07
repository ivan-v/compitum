#ifndef FACTION_INCLUDED
#define FACTION_INCLUDED

#include "trade_good.hpp"

#include <map>

enum class faction_id {
    bandits,
    blacksmiths,
    builders,
    craftsmen,
    farmers,
    guards,
    traders,
    prostitutes
};

using opinion_map = std::map<faction_id, int>;

struct faction {
    int wealth;         // TODO
    int pop;
    opinion_map opinions;
    std::array<trade_good_id, 2> food_and_water{{
        trade_good_id::food,
        trade_good_id::water
    }};
};

#endif
