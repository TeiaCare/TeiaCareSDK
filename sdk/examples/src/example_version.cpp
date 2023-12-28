#include <sdk/version.hpp>
#include <iostream>

int main() 
{
    std::cout << tc::sdk::info::name << "\n";
    std::cout << "Version: " << tc::sdk::info::version << "\n";

    std::cout << "\n" << tc::sdk::info::project_description << "\n";
    std::cout << tc::sdk::info::project_url << "\n";
    
    std::cout << "\nBuild Type: " << tc::sdk::info::build_type << "\n";
    std::cout << "Compiler: " << tc::sdk::info::compiler_name << " " << tc::sdk::info::compiler_version << "\n";
    std::cout << "Compiler Flags: " << tc::sdk::info::cxx_flags << "\n";
    std::cout << "C++ Standard: " << tc::sdk::info::cxx_standard << "\n";

    std::cout << "\nOS: " << tc::sdk::info::os_name << "\n";
    std::cout << "OS Version: " << tc::sdk::info::os_version << "\n";
    std::cout << "Processor Architecture: " << tc::sdk::info::os_processor << "\n";

    return 0;
}
