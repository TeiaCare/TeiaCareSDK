#include <sdk/version.hpp>
#include <iostream>

int main()
{
    std::cout << tc::sdk::name << " v" << tc::sdk::version << std::endl;
    return 0;
}
