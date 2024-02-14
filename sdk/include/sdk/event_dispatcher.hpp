#pragma once 

#include <sdk/thread_pool.hpp>
#include <sdk/function_traits.hpp>
#include <sdk/non_copyable.hpp>
#include <sdk/non_moveable.hpp>

#include <functional>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

namespace tc::sdk
{
/*! 
 * \class event_dispatcher
 * \brief Event Dispatcher object that handles events and dispatch them with user defined handlers.
 *
 * This class is used to manage a queue of tasks using a fixed number of threads.  
 * The actual task execution is delgated to an internal tc::sdk::task_pool object.
 */
class event_dispatcher final : private non_copyable, private non_moveable
{
private:
    struct base_handler_t
    {
        base_handler_t(unsigned long id) noexcept : id{id} {}
        virtual ~base_handler_t() noexcept {}
        base_handler_t(const base_handler_t&) = delete;
        base_handler_t(base_handler_t&&) noexcept = delete;
        base_handler_t& operator=(const base_handler_t&) = delete;
        base_handler_t& operator=(base_handler_t&&) = delete;

        const unsigned long id;
    };

    template <typename... Args>
    class handler_t : public base_handler_t, private non_copyable, private non_moveable
    {
    public:
        handler_t(unsigned long id, std::function<void(Args...)>&& h) noexcept : base_handler_t(id), _handler{std::move(h)} {}
        ~handler_t() noexcept override = default;
        
        void call(Args... args) const
        {
            _handler(std::forward<Args>(args)...);
        }

    private:
        std::function<void(Args...)> _handler;
    };

public:
    /*!
     * \brief Constructor.
     * 
     * Creates a tc::sdk::event_dispatcher instance. 
     */
    event_dispatcher() noexcept;

    /*!
     * \brief Destructor.
     * 
     * Destructs this.
     */
    ~event_dispatcher() noexcept;

    /*! 
     * \brief Add a user defined handler to the specified event
     * \param event_name Event name
     * \tparam event_handler User defined event handler
     * \return Handler ID
     *
     * TODO!
     */
    template <typename... Args, typename HandlerT>
    auto add_handler(std::string event_name, HandlerT&& event_handler) -> unsigned long
    {
        using WrapperT = typename tc::sdk::function_traits<HandlerT>::wrapper_t;
        
        static_assert(std::is_same_v<void, std::invoke_result_t<HandlerT, Args...>>,
            "\nevent_handler must return void!");
        
        static_assert(std::is_same_v<std::function<void(std::decay_t<Args>...)>, WrapperT>, 
            "\nevent_handler arguments mismatch in add_handler()!"
            "\nplease match your handler arguments (input parameters) with your declaration (template specification)");

        const auto event_handler_id = get_event_handler_id<Args...>(std::move(event_name));

        std::unique_lock handlers_lock(_handlers_mutex);        
        _handlers[event_handler_id].emplace_back(std::make_unique<handler_t<Args...>>(++handler_id, std::move(event_handler)));
        return handler_id;
    }

    /*! 
     * \brief Add a user defined handler to the specified event.
     * \param event_name Event name
     * \tparam args event handler arguments
     * \return true if the event has been emitted succesfully and at least an event handler has been called.
     *
     * TODO!
     */
    template <typename... Args>
    auto emit(std::string event_name, Args... args) -> bool
    {
        const auto event_handler_id = get_event_handler_id<Args...>(std::move(event_name));

        std::unique_lock handlers_lock(_handlers_mutex);
        if (!_handlers.contains(event_handler_id))
            return false;

        std::vector<std::shared_ptr<base_handler_t>> event_handlers = _handlers.at(event_handler_id);
        handlers_lock.unlock();

        for (auto&& h : event_handlers)
        {
            const std::function<void()> event = [e = std::static_pointer_cast<handler_t<Args...>>(h), ... args = std::forward<Args>(args)]
            {
                return std::invoke(&handler_t<Args...>::call, e.get(), args...);
            };

            _tp.run(std::move(event));
        }

        return true;
    }

    /*! 
     * \brief remove the handler with the given handler_id
     * \param handler_id Handler ID
     * \return true if removed successfully
     */ 
    auto remove_handler(unsigned long handler_id) -> bool;
    
    /*! 
     * \brief remove the event with the given event_name and all its associated handlers
     * \param event_name Event name
     * \return true if removed successfully
     */ 
    auto remove_event(std::string event_name) -> bool;

    /*! 
     * \brief Starts the event_dispatcher and the underlying tc::sdk::thread_pool with the given number of threads
     * \param num_threads Number of threads of the underlying tc::sdk::thread_pool
     * \return true if started successfully
     */ 
    auto start(const unsigned int num_threads = 1) -> bool;

    /*! 
     * \brief Stop the event_dispatcher and the underlying tc::sdk::thread_pool
     * \return true if stopped successfully
     */ 
    auto stop() -> bool;
    
private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<base_handler_t>>> _handlers;
    std::mutex _handlers_mutex;
    tc::sdk::thread_pool _tp;
    static unsigned long handler_id;

    template <typename... Args>
    constexpr auto get_event_handler_id(std::string&& event_name) const
    {
        if constexpr(sizeof...(Args) == 0)
        {
            return std::move(event_name);
        }
        else
        {
            auto args_hash = std::to_string((typeid(std::decay_t<Args>).hash_code() + ... ));
            return std::move(event_name + args_hash);
        }
    }
};

}
