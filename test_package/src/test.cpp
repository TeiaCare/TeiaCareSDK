#include <sdk/version.hpp>
#include <iostream>

int main()
{
    std::cout << tc::sdk::info::name << " v" << tc::sdk::info::version << std::endl;
    return 0;
}
