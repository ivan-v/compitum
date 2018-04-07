#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include <thread>
#include <chrono>   

#include "faction.hpp"
#include "infrastructure.hpp"
#include "population.hpp"
#include "trade_good.hpp"

using namespace std::literals;

template <class Duration>
void wait(Duration d) { // unrelated to the in-game time
    std::this_thread::sleep_for(d);
}

struct region {
    // pop = population
    int provincial_production_value;
    double goods_produced_mod;
    population pop;
    trade_good_amount_map trade_good_amounts;
    std::vector<infrastructure> infras;
};

std::ostream& operator<<(std::ostream& out, region const& value) {
    out << "Provincial production value: " << value.provincial_production_value
        << "\nGoods produced mod:          " << value.goods_produced_mod
        << "\nPopulation:                  " << value.pop.total();
    for (auto [id, amount] : value.trade_good_amounts)
        out << "\n"
            << std::setw(29) << std::left
            << (to_string(id) + ":") << amount;
    // out << "\nBuildings:                   " << value.infras;
    return out;
}

struct collect_infra_result {
    int base_production;
    double production_modifier;
};

collect_infra_result collect_infra(
        std::vector<infrastructure> const& infras,
        trade_good_id product) {
    collect_infra_result result{0, 1.0};
    for (auto const& infra : infras) {
        if (infra.product == product) {
            result.base_production += infra.base_production;
            result.production_modifier *= infra.production_modifier;
        }
    }
    return result;
}

int gross_production(region const& reg, trade_good_id product) {
    auto infra = collect_infra(reg.infras, product);
    auto goods_produced = reg.provincial_production_value 
                        * reg.goods_produced_mod
                        * infra.production_modifier 
                        + infra.base_production;
    return static_cast<int>(std::round(goods_produced));
}

int net_production(region const& reg, trade_good_id product) {
    auto goods_produced = gross_production(reg, product);
    auto local_demand = reg.pop.total();
    auto net_supply =
        static_cast<int>(std::round(goods_produced - local_demand));
    return net_supply;
}

int get_price(region const& reg, trade_good_id product){
    auto goods_produced = gross_production(reg, product);
    auto local_demand = reg.pop.total();
    auto constant = get_price_constant(product);
    auto price = std::max(
        static_cast<double>(local_demand) / goods_produced * constant,
        1.0);
    return static_cast<int>(std::round(price));        
}

int wealth_change_of_farmers(region const& reg){
    return get_price(reg, trade_good_id::food)-2;
}

// double tax_farmers(region const& reg, double tax_rate){
//      return reg.pop.faction_id::farmers.pop;
// }

// TODO: Don't always round fractional populations up.
population kill_people(population pop, int count) {
    double survival_rate =
        static_cast<double>(pop.total() - count) / pop.total();
    int killed = 0;
    for (auto pos = begin(pop.factions);
            pos != end(pop.factions) && killed < count;
            ++pos) {
        auto& fac = pos->second;
        int new_pop = static_cast<int>(std::floor(fac.pop * survival_rate));
        killed += fac.pop - new_pop;
        fac.pop = new_pop;
    }
    return pop;
}

region starve_turn_tick(region reg) {
    std::cout
        << "Regional price of food: "
        << get_price(reg, trade_good_id::food)
        << '\n';

    // Update food amount
    int food_produced = gross_production(reg, trade_good_id::food);
    int& food_amount = reg.trade_good_amounts[trade_good_id::food];
    food_amount += food_produced;
    int old_total = reg.pop.total();
    if (food_amount < 0) {
        reg.pop = kill_people(reg.pop, -food_amount);
        int const starved = old_total - reg.pop.total();
        std::cout
            << "The people starve!\n"
            << starved << " people have died.\n";
    }
    if (food_produced > 0) {
        std::cout
            << "A surplus of " << food_produced
            << " food has been produced.\n";  
    }

    // Update water amount
    int& water_amount = reg.trade_good_amounts[trade_good_id::water];
    water_amount += gross_production(reg, trade_good_id::water);
    if (water_amount < 0) {
        int old_total = reg.pop.total();
        reg.pop = kill_people(reg.pop, -water_amount);
        int const starved = old_total - reg.pop.total();
        std::cout
            << "The people die of thirst!\n"
            << starved << " people have died.\n";
    }

    return reg;
}

region simulate_turn(region reg) {
    reg = starve_turn_tick(reg);
    return reg;
}

void print_slow(std::string text){
    for (char& c : text){
        std::cout << c << std::flush;
        wait(125ms);
        if (c == '.') 
            wait(225ms);
    }
    std::cout << "\n";
}

struct world_time {
    int year;  //1352 through +++
    int month; //1 through 12
    int day;   //1 through 30, for now
    int hour;  //0 through 23
    int minute;//0 through 59
};

std::string get_month_name(int month) {
    switch (month) {
        case 1: return "January";
        case 2: return "February";
        case 3: return "March";
        case 4: return "April";
        case 5: return "May";
        case 6: return "June";
        case 7: return "July";
        case 8: return "August";
        case 9: return "September";
        case 10: return "October";
        case 11: return "November";
        case 12: return "December";
    }
    return "-1";
}

void print_world_time(world_time t) {
    std::string minute_string =
        (t.minute < 10 ? "0" : "") + std::to_string(t.minute);
    std::cout
        << t.hour << ":" << minute_string
        << " on " << get_month_name(t.month)
        << " " << t.day
        << ", the year of our lord " << t.year
        << "\n"; 
}

void initialize_time() {
    // TODO
}

void get_ingame_time() {
    // TODO 
}

struct world {
    region reg1; // TODO: Make vector of regions
    world_time t1;
};

int main() {
    print_slow("Welcome, player 1. Welcome...");
    wait(1s);

    region reg1 {
        30,         // provincial production value
        1.00,       // "goods produced" modifier
        {           // population
            {       // factions:  wealth, population, opinions
                { faction_id::bandits,     { 0, 10, {} } },
                { faction_id::blacksmiths, { 0, 20, {} } },
                { faction_id::builders,    { 0,  0, {} } },
                { faction_id::craftsmen,   { 0,  0, {} } },
                { faction_id::guards,      { 0,  0, {} } },
                { faction_id::traders,     { 0, 10, {} } },
                { faction_id::prostitutes, { 0, 10, {} } },
            }
        },
        {
            { trade_good_id::food, 10 },    // goods: price constant, amount
            { trade_good_id::water, 10 },
        },
        { farm, well },                     // infrastructures
    };

    world w {
        reg1,
        {1352, 1, 2, 3, 4}, 
    };

    std::cout << reg1 << "\n";

    print_world_time(w.t1);

    reg1 = simulate_turn(reg1);

    std::cout << reg1 << "\n";
}
