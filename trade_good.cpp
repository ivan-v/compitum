#include "trade_good.hpp"

#include <cassert>
 
std::string compitum::to_string(trade_good_id id) {
    switch (id) {
        case trade_good_id::food:   return "food";
        case trade_good_id::water:  return "water";
        case trade_good_id::stone:  return "stone";
        case trade_good_id::marble: return "marble";
        case trade_good_id::wood:   return "wood";
    }
    assert(false);
    return "trade_good_id{" + std::to_string(static_cast<int>(id)) + "}";
}

int compitum::get_price_constant(trade_good_id id) {
    switch (id) {
        case trade_good_id::food:   return 1;
        case trade_good_id::water:  return 1;
        case trade_good_id::stone:  return 3;
        case trade_good_id::marble: return 5;
        case trade_good_id::wood:   return 2;
    }
    assert(false);
    return 0;
}

std::ostream& compitum::operator<<(
        std::ostream& out,
        trade_good_amount_map::value_type const& value) {
    return out << value.second << " units of " << to_string(value.first);
}
