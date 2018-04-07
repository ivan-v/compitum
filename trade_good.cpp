#include "trade_good.hpp"
 
std::string to_string(trade_good_id id) {
    switch (id) {
        case trade_good_id::food:   return "food";
        case trade_good_id::water:  return "water";
        case trade_good_id::stone:  return "stone";
        case trade_good_id::marble: return "marble";
        case trade_good_id::wood:   return "wood";
    }
}

int get_price_constant(trade_good_id id) {
    switch (id) {
        case trade_good_id::food:   return 1;
        case trade_good_id::water:  return 1;
        case trade_good_id::stone:  return 3;
        case trade_good_id::marble: return 5;
        case trade_good_id::wood:   return 2;
    }
}

std::ostream& operator<<(std::ostream& out, trade_good const& value) {
    return out << value.amount << " units of " << to_string(value.id);
}
