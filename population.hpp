#ifndef POPULATION_INCLUDED
#define POPULATION_INCLUDED

#include <map>

#include "faction.hpp"

struct population {
    std::map<faction_id, faction> factions;

    int total() const;
};

#endif
