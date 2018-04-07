#ifndef FACTION_INCLUDED
#define FACTION_INCLUDED

#include <map>

#include "trade_good.hpp"

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

constexpr std::array<faction_id, 8> faction_ids{{
    faction_id::bandits,
    faction_id::blacksmiths,
    faction_id::builders,
    faction_id::craftsmen,
    faction_id::farmers,
    faction_id::guards,
    faction_id::traders,
    faction_id::prostitutes
}};

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
