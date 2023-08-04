#pragma once 

#include "gtest/gtest.h"
#include <sdk/task_scheduler.hpp>

namespace ssts
{

class test_task_scheduler : public ::testing::Test
{
protected:
    test_task_scheduler() { }

    ~test_task_scheduler() override { }

    void SetUp() override { }

    void TearDown() override { }

    void InitScheduler(const unsigned int n_threads)
    {
        log("Initializing Scheduler");
        s = std::make_unique<tc::sdk::task_scheduler>();
        s->start(n_threads);
        log("Scheduler initialized with", n_threads, "threads");
    }

    unsigned get_size() const
    {
        log("Getting Scheduler size");
        const unsigned size = s->tasks_size();
        log("Scheduler size is:", size);
        return size;
    }

    void StopScheduler() 
    {
        log("Stopping Scheduler");
        s->stop();
        log("Scheduler Stopped");
    }

    void Sleep(tc::sdk::clock::duration&& delay) const
    {
        const auto scaled_delay = scale_duration(delay);

        log("Sleeping for", get_seconds(scaled_delay), "seconds");        
        std::this_thread::sleep_for(scaled_delay);
        log("Slept for", get_seconds(scaled_delay), "seconds");
    }

    void StartAllTasksIn(tc::sdk::clock::duration&& delay)
    {
        log("Scheduling", n_tasks, "tasks", "using in() API");
        foreach_tasks([&](auto id){ s->in(std::string(id), std::move(delay), [id]{ std::cout << id << std::endl; }); });
        log("Scheduled", n_tasks, "tasks - in", get_seconds(delay), "seconds");
    }

    void StartAllTasksEvery(tc::sdk::clock::duration&& interval, tc::sdk::clock::duration&& task_sleep = tc::sdk::clock::duration(0))
    {
        log("Scheduling", n_tasks, "tasks", "using every() API");
        foreach_tasks([&](auto id) { s->every(std::string(id), std::move(interval), [id, task_sleep]{ std::cout << id << std::endl; std::this_thread::sleep_for(task_sleep); }); });
        log("Scheduled", n_tasks, "tasks - every", get_seconds(interval), "seconds");
    }

    void SetEnabledAllTasks(bool is_enabled)
    {
        log("Enabling", n_tasks, "tasks");
        foreach_tasks([&](auto id){ s->set_enabled(id, is_enabled); });
        log("Enabled", n_tasks, "tasks");
    }

    void RemoveAllTasks()
    {
        log("Removing", n_tasks, "tasks");
        foreach_tasks([&](auto id){ s->remove_task(id); });
        log("Removed", n_tasks, "tasks");
    }

    void UpdateAllTasksInterval(tc::sdk::clock::duration&& interval)
    {
        log("Updating", n_tasks, "tasks");
        foreach_tasks([&](auto id){ s->update_interval(id, interval); });
        log("Updated", n_tasks, "tasks - interval", get_seconds(interval), "seconds");
    }

    unsigned int CountScheduledTasks()
    {
        unsigned int count = 0;
        log("Counting Scheduled Tasks");
        foreach_tasks([&](auto id)
        {
            if(s->is_scheduled(id))
                ++count;
        });
        log("Counted Scheduled Tasks: ", count, "tasks scheduled");

        return count;
    }

    unsigned int CountEnabledTasks()
    {
        unsigned int count = 0;
        log("Counting Enabled Tasks");
        foreach_tasks([&](auto id)
        {
            if(s->is_enabled(id))
                ++count;
        });
        log("Counted Enabled Tasks: ", count, "tasks enabled");

        return count;
    }

    std::unique_ptr<tc::sdk::task_scheduler> s;
    unsigned int n_tasks = 4u;
    const std::string task_id_prefix = "task_id_"s;
    unsigned int sleep_duration_scale_factor = 3u;

private:
    template<typename Func, typename... Args>
    void foreach_tasks(Func&& func, Args&&... args) const
    {
        for (auto n = 0; n < n_tasks; ++n)
        {
            const auto task_id = task_id_prefix + std::to_string(n);
            std::invoke(std::forward<Func>(func), task_id, std::forward<Args>(args)...);
        }
    }

    template<typename... Args>
    void log(Args&&... args) const
    {
        ((std::cout << std::forward<Args>(args) << ' ') , ...) << std::endl; // << '\n'
    }

    template<typename T>
    tc::sdk::clock::duration scale_duration(T&& time_duration) const
    { 
        return time_duration * sleep_duration_scale_factor;
    }
    
    float get_seconds(const tc::sdk::clock::duration& delay) const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(delay).count() / 1000.f;
    }
};

}
