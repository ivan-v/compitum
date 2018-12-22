#ifndef COMPITUM_CLOCK_INCLUDED
#define COMPITUM_CLOCK_INCLUDED

#include "compitum/interactor.hpp"

#include <cassert>
#include <string>

namespace compitum {

struct world_time {
    int year = 1352;
    int month = 1;  // 1 through 12
    int day;    // 1 through 31
    int hour;   // 0 through 23
    int minute; // 0 through 59
};

std::string get_month_name(int month);

void print(interactor& io, world_time t);

void advance_time(world_time clock, int days);

void initialize_time();

void get_ingame_time();

}

#endif