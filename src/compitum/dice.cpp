#include "compitum/dice.hpp"

// using std::chrono::milliseconds;
// using std::this_thread::sleep_for;

int compitum::roll_die(int faces) {
   return rand() % faces + 1;
}

//TODO: fix: sometimes returns 0
int compitum::roll_dice(int faces, int times, int modifier) {
    srand(time(0)); //TODO: fix lossy conversion
    int result = 0;
    for(int i = 0; i < times; i++) {
        result += roll_die(faces);
    }
    return result + modifier;
}