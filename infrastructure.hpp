#ifndef INFRASTRUCTURE_INCLUDED
#define INFRASTRUCTURE_INCLUDED

#include <array>
#include <ostream>

#include "trade_good.hpp"

enum class infrastructure_id { farm, well };

constexpr std::array<infrastructure_id, 2> infrastructure_ids{{
    infrastructure_id::farm,
    infrastructure_id::well
}};

struct infrastructure {
    infrastructure_id id;
    trade_good_id product;
    int base_production;
    double production_modifier;
    int maintenance;
    // trade_good build_cost;  TODO
};

std::ostream& operator<<(std::ostream&, infrastructure const&);

constexpr infrastructure
    farm{ infrastructure_id::farm, trade_good_id::food, 15, 1.05, 0 },
    well{ infrastructure_id::well, trade_good_id::water, 40, 1.00, 0 };

#endif
