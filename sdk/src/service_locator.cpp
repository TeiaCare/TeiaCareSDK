#include <sdk/service_locator.hpp>

namespace tc::sdk
{
service_locator& service_locator::instance()
{
    static service_locator s;
    return s;
}

}
