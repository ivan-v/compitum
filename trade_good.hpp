#ifndef TRADE_GOOD_INCLUDED
#define TRADE_GOOD_INCLUDED

#include <array>
#include <map>
#include <ostream>
#include <string>

enum class trade_good_id { food, water, wood, stone, marble };

constexpr std::array<trade_good_id, 5> trade_good_ids{{
    trade_good_id::food,
    trade_good_id::water,
    trade_good_id::stone,
    trade_good_id::marble,
    trade_good_id::wood
}};

std::string to_string(trade_good_id);

int get_price_constant(trade_good_id);

using trade_good_amount_map = std::map<trade_good_id, int>;

std::ostream& operator<<(
        std::ostream&,
        trade_good_amount_map::value_type const&);

#endif
