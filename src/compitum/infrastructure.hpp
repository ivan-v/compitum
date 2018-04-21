#ifndef COMPITUM_INFRASTRUCTURE_INCLUDED
#define COMPITUM_INFRASTRUCTURE_INCLUDED

#include "compitum/trade_good.hpp"

#include <ostream>

namespace compitum {

enum class infrastructure_id { farm, well, tavern };

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
    well{ infrastructure_id::well, trade_good_id::water, 40, 1.00, 0 },
    tavern{ infrastructure_id::tavern, trade_good_id::groschen, 100, 1.00, 50 };

}

#endif
