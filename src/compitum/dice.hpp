#ifndef COMPITUM_DICE_INCLUDED
#define COMPITUM_DICE_INCLUDED

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

namespace compitum {

int roll_die(int faces);

int roll_dice(int faces, int times, int modifier);

}

#endif