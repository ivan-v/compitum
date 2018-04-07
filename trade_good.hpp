#ifndef TRADE_GOOD_INCLUDED
#define TRADE_GOOD_INCLUDED

#include <array>
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

struct trade_good {
    trade_good_id id;
    int amount;
};

std::ostream& operator<<(std::ostream&, trade_good const&);

#endif
