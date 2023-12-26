#include <sdk/event_dispatcher.hpp>

namespace tc::sdk
{
unsigned long event_dispatcher::handler_id = 0;

event_dispatcher::event_dispatcher() noexcept
{
}

event_dispatcher::~event_dispatcher() noexcept
{
    stop();
    handler_id = 0;
}

auto event_dispatcher::remove_handler(unsigned long remove_handler_id) -> bool
{
    std::unique_lock handlers_lock(_handlers_mutex);

    for(auto&& event_handlers : _handlers)
    {
        if(std::erase_if(event_handlers.second, [remove_handler_id](auto&& h){ return h->id == remove_handler_id; }))
        {
            if(event_handlers.second.empty())
                _handlers.erase(event_handlers.first);

            return true;
        }
    }

    return false;
}

auto event_dispatcher::remove_event(std::string event_name) -> bool
{
    std::unique_lock handlers_lock(_handlers_mutex);        

    if(std::erase_if(_handlers, [event_name](auto&& handler_iterator){ return handler_iterator.first.starts_with(event_name); }))
        return true;

    return false;
}

auto event_dispatcher::start(const unsigned int num_threads) -> bool
{
    return _tp.start(num_threads);
}

auto event_dispatcher::stop() -> bool
{
    {
        std::scoped_lock handlers_lock(_handlers_mutex);
        _handlers.clear();
    }

    return _tp.stop();
}

}