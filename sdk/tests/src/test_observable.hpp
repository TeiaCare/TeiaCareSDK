#pragma once

#include <gtest/gtest.h>
#include <sdk/observable.hpp>

namespace tc::sdk::tests
{
class foo_t
{
public:
    foo_t(const std::string& s) : _s{ s } {}
    bool operator==(foo_t const&) const = default;

    std::string get() const { return _s; }
    void set(const std::string& s) { _s = s; }
private:
    std::string _s;
};

class counter_t
{
public:
    bool operator==(counter_t const&) const = default;
    void update(int increment) { _value += increment; }
    int value() const { return _value; }
private:
    int _value = 0;
};

static double global_counter = 0.0;
void free_function(int increment)
{
    global_counter += increment;
}

}
