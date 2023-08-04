#include <sdk/uuid.hpp>
#include <sdk/uuid_generator.hpp>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

int main() 
{
    auto uuid_generator = tc::sdk::uuid_generator::instance();

    {
        spdlog::info("\n\ntc::sdk::uuid::uuid() [null/empty UUID]");
        auto null = tc::sdk::uuid();
        spdlog::info(null.str());
    }

    {
        spdlog::info("\n\ntc::sdk::uuid::bytes()");
        auto uuid = uuid_generator.create();
        spdlog::info(uuid.bytes());
    }

    {
        spdlog::info("\n\ntc::sdk::uuid::hash()");
        auto uuid = uuid_generator.create();
        spdlog::info(uuid.hash());
    }

    spdlog::info("\n\ntc::sdk::uuid_generator::from_string()");
    {
        auto uuid = uuid_generator.from_string("a142011e-58fa-4415-a0ea-e3ace6597fc8");
        spdlog::info(uuid.str());
    }

    {
        auto uuid = uuid_generator.from_string("ef25b786-d431-4612-8fb9-42afd24d22c6");
        spdlog::info(uuid.str());
    }

    spdlog::info("\n\ntc::sdk::uuid_generator::create()");
    for(int i = 0; i < 10; ++i)
    {
        auto uuid = uuid_generator.create();
        spdlog::info(uuid.str());
    }

    return 0;
}
