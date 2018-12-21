#include "compitum/config.hpp"

#include <fstream>
#include <set>
#include <sstream>

using namespace compitum;
using namespace std::literals;

configuration compitum::config{
    75ms,   // quick delay
    125ms,  // short delay
    350ms   // long delay
};

namespace {

    struct config_entry {
        std::string key, value;
    };

}

static config_entry parse_entry(std::string const& line) {
    std::istringstream parse(line);
    std::string key, eq, value_head, value_tail;
    if (!(parse >> key >> eq >> value_head))
        throw "unexpected end of file"s;
    if (eq != "=")
        throw "expected '=', not " + eq;
    getline(parse, value_tail);
    return {key, value_head + value_tail};
}

// Returns true on success, and false if `key` is not recognized.  Throws on
// parse errors (e.g., invalid integers).
static bool assign(
        std::string const& key,
        std::string const& value, 
        configuration& config) {
#define IF(KEY, VALUE) if (key == #KEY) { config.KEY = VALUE; return true; }
    IF(short_delay, std::chrono::milliseconds(stoi(value)))
    IF(long_delay,  std::chrono::milliseconds(stoi(value)))
    return false;
#undef IF
}

void compitum::load_config_file(std::string const& path) try {
    std::ifstream in(path);
    if (!in)
        throw "cannot read file"s;
    std::set<std::string> seen;
    for (std::string line; getline(in, line);) {
        if (!line.empty() && line.front() != '#') {
            auto [key, value] = parse_entry(line);
            if (seen.count(key))
                throw "redundant key: " + key;
            if (!assign(key, value, config))
                throw "bad key: " + key;
        }
    }
} catch (std::exception const& err) {
    throw std::runtime_error("config: " + path + ": " + err.what());
} catch (std::string const& what) {
    throw std::runtime_error("config: " + path + ": " + what);
}
