#include "compitum/clock.hpp"

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

void print(compitum::interactor& io, compitum::world_time t) {
    std::string minute_string =
        (t.minute < 10 ? "0" : "") + std::to_string(t.minute);
    io  << t.hour << ":" << minute_string
        << " on " << get_month_name(t.month)
        << " " << t.day
        << ", the year of our lord " << t.year
        << "\n";
}

void advance_time(compitum::world_time clock, int days) {
    if ((clock.month == 1 || clock.month == 3 || clock.month == 5 || 
        clock.month == 7 || clock.month == 8 || clock.month == 10) 
        && clock.day + days > 31) {
        ++clock.month;
        clock.day = days + clock.day - 31;
    } else if (clock.month == 12 && clock.day + days > 31) {
        clock.day = days + clock.day - 31;
        ++clock.year;
    } else if (clock.month == 2 && clock.day + days > 28) {
        clock.day = days + clock.day - 28;
        ++clock.month;
    } else if ((clock.month == 2 || clock.month == 4 || clock.month == 6 
        || clock.month == 9 || clock.month == 11) 
    && clock.day + days > 30) {
        ++clock.month;
        clock.day = days + clock.day - 30;
    } else {
        clock.day += days;
    }
}

void initialize_time() {
    // TODO
}

void get_ingame_time() {
    // TODO
}