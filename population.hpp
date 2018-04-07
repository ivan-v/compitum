#ifndef POPULATION_INCLUDED
#define POPULATION_INCLUDED

#include "faction.hpp"

#include <map>

struct population {
    std::map<faction_id, faction> factions;

    int total() const;
};

#endif
