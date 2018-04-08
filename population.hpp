#ifndef COMPITUM_POPULATION_INCLUDED
#define COMPITUM_POPULATION_INCLUDED

#include "faction.hpp"

#include <map>

namespace compitum {

struct population {
    std::map<faction_id, faction> factions;

    int total() const;
};

}

#endif
