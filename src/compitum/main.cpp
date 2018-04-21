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


struct character {
    std::string character_name;
    int stamina;
    int max_stamina;
    int hp;
    int max_hp;
    bool alive;
};

void set_hp(character& c, int p) {
    c.hp = p;
}

void drain_hp(character& c, int p) {
    c.hp -= p;
    if (c.hp < 0)
        c.alive = false;
}

void replenish_hp(character& c, int p) {
    if ((c.hp + p) <= c.max_hp)
        c.hp += p; 
    else c.hp = c.max_hp;
}

void set_stamina(character& c, int p) {
    c.stamina = p;
}

void drain_stamina(character& c, int p) {
    c.stamina = c.stamina >= p ?
    c.stamina - p : 0;

    // if ((c.stamina - p) >= 0)
    //     c.stamina -= p;
    // else c.stamina = 0;
}

void replenish_stamina(character& c, int p) {
    if ((c.stamina + p) <= c.max_stamina)
        c.stamina += p;
    else c.stamina = c.max_stamina;
}

//TODO: make distance and probability
void attempt_strike(character& target, int damage_directed) {
    drain_hp(target, damage_directed);
}

void unarmed_strike(character& self, character& target) {
    drain_stamina(self, 5);
    attempt_strike(target, 2);
}

//player must type in attack quickly enough
int player_action(int milliseconds_allowed) {
    std::string input;
    auto start = std::chrono::steady_clock::now();
    std::cin >> input;
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now( ) - start);
    if (elapsed.count() < milliseconds_allowed) {
        if (input == "strike")
            return 1;
        else if (input == "heal")
            return 2;
        else if (input == "flee")
            return 3;
        else 
            return -1; 
    }
    else 
        return 0;
}

//TODO: make enemy action functions, such as different attacks
void enemy_action(character& self, character& enemy) {
    if (self.hp < 10) {
        replenish_hp(self, 5);
        std::cout << self.character_name << " healed themselves to "
                  << self.hp << " health! \n";
    } else if (self.stamina >= 5) {
        unarmed_strike(self, enemy);
        std::cout << self.character_name << " strikes "<< enemy.character_name 
                  << ", dealing "<< 4 << " damage! \n";
        std::cout << enemy.character_name << " has "<< enemy.hp 
                  << " health left. \n";
    } else 
        replenish_stamina(self, 3);
        std::cout << self.character_name << " repleninshes their stamina. \n";
}


void fight_encounter(character& player, character& enemy, int difficulty_speed) {
    interactor io{std::cin, std::cout};

    std::cout << "Encounter began. \n";
    io.print_slow("Hint: type 'strike' or 'heal' quickly.");
    
    //TODO: calculate initiative, then determine who goes first
    //if (enemy.initiative > player.initiative)

    while (player.alive && enemy.alive) {
        enemy_action(enemy, player);
        sleep_for(config.long_delay * 2);
        std::cout << "Attack fast, strike true! \n";
        int player_command = player_action(2000);
        if (player_command == -1) {
            std::cout<< "Command unrecognized! \n";
        } else if (player_command == 0) {
            std::cout<< "You were too slow to react! \n";
        } else if (player_command == 1) {
            drain_stamina(player, 5);
            attempt_strike(enemy, 5);
            std::cout << "The enemy is at " << enemy.hp << " health! \n";
        } else if (player_command == 2) {
            replenish_hp(player, 5);
            std::cout << "You healed yourself to " << player.hp << " health! \n";      
        } else if (player_command == 3) {
            io.print_slow("Being the coward that you are, you flee...");
            break; //Bad?
        }
    }
    if (!player.alive) {
        io.print_slow("You have died!");
    } if (!enemy.alive) {
        io.print_slow("You have slain the enemy!");
        io.print_slow("He had a wife and child, you monster...");
    }
}



int main(int argc, char** argv) try {
    parse_args(argc, argv);
    interactor io{std::cin, std::cout};

    io.print_slow("Welcome, player 1. Welcome...");
    sleep_for(config.long_delay * 3);

    character c1 {
        "Player 1", 10, 10, 10, 20, true
    };

    character c2 {
        "Magic Boar", 10, 10, 10, 20, true
    };

    int difficulty_speed = 2000; //TODO: make configurable

    fight_encounter(c1, c2, difficulty_speed);


    


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
