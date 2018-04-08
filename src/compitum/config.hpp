#ifndef COMPITUM_CONFIG_INCLUDED
#define COMPITUM_CONFIG_INCLUDED

#include <chrono>
#include <string>

namespace compitum {

extern struct configuration {
    std::chrono::milliseconds short_delay, long_delay;
} config;

// Throws on error (unreadable file, unrecognized config key, etc.).
void load_config_file(std::string const& path);

}

#endif
