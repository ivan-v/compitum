#include "faction.hpp"
#include "infrastructure.hpp"
#include "population.hpp"
#include "trade_good.hpp"
#include "region.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>

using namespace std::literals;

using std::this_thread::sleep_for;

int wealth_change_of_farmers(region const& reg){
    return get_price(reg, trade_good_id::food) - 2;
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

region update_food_amount(region reg) {
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
    return reg;
}

region update_water_amount(region reg) {
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

region starve_turn_tick(region reg) {
    std::cout
        << "Regional price of food: "
        << get_price(reg, trade_good_id::food)
        << '\n';
    reg = update_food_amount(reg);
    reg = update_water_amount(reg);
    return reg;
}

region simulate_turn(region reg) {
    reg = starve_turn_tick(reg);
    return reg;
}

void print_slow(std::string text){
    for (char& c : text){
        std::cout << c << std::flush;
        sleep_for(125ms);
        if (c == '.')
            sleep_for(225ms);
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
    sleep_for(1s);

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
