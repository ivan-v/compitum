#include "compitum/combat.hpp"

#include <cassert>

// using std::chrono::milliseconds;
// using std::this_thread::sleep_for;

int compitum::roll_die(int faces) {
   return rand() % faces + 1;
}

//TODO: fix: sometimes returns 0
int compitum::roll_dice(int faces, int times, int modifier) {
    srand(time(0)); //TODO: fix conversion warning
    int result = 0;
    for(int i = 0; i < times; i++) {
        result += roll_die(faces);
    }
    return result + modifier;
}

//TODO: make armor and defense checks
int compitum::attempt_block(character& self) {
    int stamina_cost = 10; //TODO: make dynamic
    //if (incoming_attack.damage_type == "")
    if (self.stamina >= stamina_cost) {
        return 1;
    }
    else
        return 0;
}

int compitum::attempt_strike(character& self, character& target,
                    combat_action attack, int distance_between_characters) {
    if (attack.reach_distance >= distance_between_characters 
                      && attack.stamina_cost <= self.stamina) {
        drain_stamina(self, attack.stamina_cost);
        int damage_dealt = roll_dice(attack.roll_dice_faces, 
                                   attack.roll_dice_times, 
                                   attack.roll_dice_modifier);
        drain_hp(target, damage_dealt);
        return damage_dealt;
    }
    return 0;
}

//player must type in attack quickly enough
int compitum::player_action(int milliseconds_allowed) {
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
        else if (input == "block")
            return 4;
        else 
            return -1; 
    }
    else 
        return 0;
}

//TODO: make enemy action functions, such as different attacks
void compitum::enemy_action(character& self, character& enemy, 
                  int distance_between_characters, int block) {
    if (self.hp < 10) {
        replenish_hp(self, 5);
        std::cout << self.character_name << " healed themselves to "
                  << self.hp << " health! \n";
    } else if (self.stamina >= 5) {
        if (block == 0) {
            std::cout << self.character_name << " strikes "<< enemy.character_name 
                      << ", dealing " << 
                      attempt_strike(self, enemy, tusk, distance_between_characters)
                      << " damage! \n";
            std::cout << enemy.character_name << " has "<< enemy.hp 
                      << " health left. \n";
        } else {
            std::cout << enemy.character_name << " successfully blocks "
                      << self.character_name << "'s attack! \n"; 
        } 
    } else {
        replenish_stamina(self, 3);
        std::cout << self.character_name << " repleninshes their stamina. \n";
    }
}

