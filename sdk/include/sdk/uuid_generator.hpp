#pragma once

#include <sdk/uuid.hpp>
#include <random>

namespace tc::sdk
{
class uuid_generator
{
public:
    static uuid_generator& instance();
    uuid create();
    uuid from_string(const std::string& s);
    uuid from_string(const char* raw);

private:
    uuid_generator();
    std::mt19937_64 _generator;
    std::uniform_int_distribution<uint64_t> _distribution;
};

}
