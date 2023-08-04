#include <sdk/task.hpp>

namespace tc::sdk
{
task::task(task&& other) noexcept 
    : _impl{ std::move(other._impl) } 
{
}

void task::operator()()
{
    _impl->invoke();
}

void task::invoke()
{
    _impl->invoke();
}

}
