#include "compitum/character.hpp"
#include "compitum/combat.hpp"
#include "compitum/config.hpp"
#include "compitum/faction.hpp"
#include "compitum/infrastructure.hpp"
#include "compitum/interactor.hpp"
#include "compitum/population.hpp"
#include "compitum/region.hpp"
#include "compitum/trade_good.hpp"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace compitum;
using namespace std::literals;

using std::chrono::milliseconds;
using std::this_thread::sleep_for;

int wealth_change_of_farmers(region const& reg) {
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

struct world_time {
    int year;
    int month;  // 1 through 12
    int day;    // 1 through 31
    int hour;   // 0 through 23
    int minute; // 0 through 59
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
    assert(false);
    return "month{" + std::to_string(month) + "}";
}

void print(interactor& io, world_time t) {
    std::string minute_string =
        (t.minute < 10 ? "0" : "") + std::to_string(t.minute);
    io  << t.hour << ":" << minute_string
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

bool is_quit(std::string const& line) {
    return !line.empty() && line.front() == 'q';
}

void parse_args(int /* argc */, char** argv) {
    auto raise = [](std::string const& what) {
        throw std::runtime_error("argv: " + what);
    };
    while (auto arg = *++argv) {
        if (arg == "--config"s) {
            if (!(arg = *++argv))
                raise("--config requires an argument");
            load_config_file(arg);
        }
    }
}

void fight_encounter(character& player, character& enemy, int difficulty_speed,
                                               int distance_between_characters) {
    interactor io{std::cin, std::cout};

    io.print_fast("Encounter began.");
    io.print_slow("Hint: type 'strike', 'block' or 'heal' quickly.");
    sleep_for(config.long_delay * 2);
    
    //TODO: calculate initiative, then determine who goes first
    //if (enemy.initiative > player.initiative)
    int block = 0;
    while (player.hp >= 0 && enemy.hp >= 0) {
        sleep_for(config.long_delay * 3);
        enemy_action(enemy, player, distance_between_characters, block);
        block = 0;
        sleep_for(config.long_delay * 6);
        io.print_fast("Attack fast, strike true!");
        int player_command = player_action(difficulty_speed);
        if (player_command == -1) {
            io.print_fast("Command unrecognized!");
            replenish_stamina(player, 8);
            sleep_for(config.long_delay * 2);
            io.print_fast("Your stamina replenishes to " + std::to_string(player.stamina));
        } else if (player_command == 0) {
            io.print_fast("You were too slow to react!");
            replenish_stamina(player, 8);
            io.print_fast("Your stamina is now " + std::to_string(player.stamina) + ".");
        } else if (player_command == 1) {
            int strike = attempt_strike(player, enemy, 
                                        distance_between_characters);
            if (strike > 0) {
                io.print_fast(player.character_name + " strikes " 
                          + enemy.character_name + ", dealing " 
                          +  std::to_string(strike) + " damage!");
                io.print_fast("The enemy is now at " + std::to_string(enemy.hp) + " health!");
            } else {
                io.print_fast("You don't have enough stamina to attack!");
                replenish_stamina(player, 8);
                io.print_fast("Your stamina replenishes to " + std::to_string(player.stamina) 
                         + ".");
            }
        } else if (player_command == 2) {
            replenish_hp(player, 5);
            io.print_fast("You healed yourself to " + std::to_string(player.hp) + " health!");      
        } else if (player_command == 3) {
            io.print_slow("Being the coward that you are, you flee...");
            break; //Bad?
        }
        else if (player_command == 4) {
            if (attempt_block(player) == 0) {
                io.print_fast("You don't have enough stamina to block.");
                block = 0;
                replenish_stamina(player, 8);
                io.print_fast("Your stamina is now " + std::to_string(player.stamina) + ".");
            } else {
                block = 1; 
            }
        }
    }
    if (!player.alive) {
        io.print_slow("You have died!");
    } if (!enemy.alive) {
        io.print_slow("You have slain the enemy!");
        io.print_slow("He had a wife and child, you monster...");
    }
}

void defend_against_wave(character& player, character& enemy, int difficulty_speed,
                                              int distance_between_characters) {
    while (player.hp >= 0) {
        fight_encounter(player, enemy, difficulty_speed, 
                            distance_between_characters);
    }
}

int main(int argc, char** argv) try {
    parse_args(argc, argv);
    interactor io{std::cin, std::cout};

    //std::cout << unarmed_strike; TODO: fix (compile error)

    io.print_slow("Welcome, player 1. Welcome...");
    sleep_for(config.long_delay * 3);

    character c1 {
        "Player 1", 10, 10, 10, 20, true, unarmed_strike
    };

    character c2 {
        "Magic Boar", 10, 10, 10, 20, true, tusk
    };

    player_inventory_action(c1);

    int difficulty_speed = 2000; //TODO: make configurable

    fight_encounter(c1, c2, difficulty_speed, 1);


    


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

    auto prompt = [&] {
        print(io, w.t1);
        io << reg1 << "\nEnter blank line or quit.\n";
    };

    prompt();
    for (std::string line; getline(std::cin, line) && !is_quit(line);) {
        if (line.empty()) {
            reg1 = simulate_turn(reg1);
            prompt();
        } else {
            std::cout << "bad input: " << line;
        }
    }
    std::cout << "Goodbye!\n";
} catch (std::exception const& err) {
    std::clog << "error: " << err.what() << '\n';
    return 1;
}
