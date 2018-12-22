#ifndef COMPITUM_INFRASTRUCTURE_INCLUDED
#define COMPITUM_INFRASTRUCTURE_INCLUDED

#include <ostream>

namespace compitum {

enum class infrastructure_id { farm, well, tavern };

struct infrastructure {
    infrastructure_id id;
    //trade_good_id product;
    int base_production;
    double production_modifier;
    int maintenance;
    int build_cost;
};

std::ostream& operator<<(std::ostream&, infrastructure const&);

constexpr infrastructure
    farm{ infrastructure_id::farm, 15, 1, 20, 2000 },
    well{ infrastructure_id::well, 40, 1.00, 0, 100},
    tavern{ infrastructure_id::tavern, 100, 1.00, 100, 5000};

}

#endif
