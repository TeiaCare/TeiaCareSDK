#include <sdk/version.hpp>
#include <iostream>

int main(int, char**)
{
    std::cout << tc::sdk::name << " v" << tc::sdk::version << std::endl;
    return 0;
}
