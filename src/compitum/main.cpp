#include "compitum/character.hpp"
#include "compitum/combat.hpp"
#include "compitum/config.hpp"
#include "compitum/infrastructure.hpp"
#include "compitum/interactor.hpp"
#include "compitum/region.hpp"

#include "compitum/clock.hpp"

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

region simulate_turn(region reg) {
    //reg = starve_turn_tick(reg);
    return reg;
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
        sleep_for(config.long_delay * 1);
        enemy_action(enemy, player, distance_between_characters, block);
        block = 0;
        sleep_for(config.long_delay * .5);
        io.print_fast("Attack fast, strike true!");
        int player_command = player_action(player, difficulty_speed);
        if (player_command == -1) {
            io.print_fast("Command unrecognized!");
            replenish_stamina(player, 8);
            sleep_for(config.long_delay * 1);
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

    character c1 {
        "Player 1", 10, 10, 10, 20, true, unarmed_strike
    };

    while (true) {
        std::string input;
        getline(std::cin, input);
        if (input == "inv" || input == "inventory") {
            //list_inventory(c1);
        }
    }

    //io.print_slow("Welcome, player 1. Welcome...");
    sleep_for(config.long_delay * 3);

    character c2 {
        "Magic Boar", 10, 10, 10, 20, true, tusk
    };

    player_inventory_action(c1);

    int difficulty_speed = 20000; //TODO: make configurable

    fight_encounter(c1, c2, difficulty_speed, 1);

    region reg1 {
        { farm, well },                     // infrastructures
    };

    world w {
        reg1,
        {1352, 1, 2, 3, 4},
    };
   
    for (std::string line; getline(std::cin, line) && !is_quit(line);) {
        if (line.empty()) {
            //reg1 = simulate_turn(reg1);
            //prompt();
        } else {
            std::cout << "bad input: " << line;
        }
    }
    std::cout << "Goodbye!\n";
} catch (std::exception const& err) {
    std::clog << "error: " << err.what() << '\n';
    return 1;
}
