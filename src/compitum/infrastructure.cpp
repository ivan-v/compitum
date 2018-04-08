#include "compitum/infrastructure.hpp"

std::ostream& compitum::operator<<(
        std::ostream& out,
        infrastructure const& value) {
    return out
        << "{ Base production = " << value.base_production
        << ", Production modifier = " << value.production_modifier
        << " }";
}
