#pragma once

#include <atomic>      // std::atomic
#include <chrono>      // std::chrono
#include <cstdint>     // std::int_fast64_t, std::uint_fast32_t
#include <functional>  // std::function
#include <future>      // std::future, std::promise
#include <iostream>    // std::cout, std::ostream
#include <memory>      // std::shared_ptr, std::unique_ptr
#include <mutex>       // std::mutex, std::scoped_lock
#include <queue>       // std::queue
#include <thread>      // std::this_thread, std::thread
#include <type_traits> // std::decay_t, std::enable_if_t, std::is_void_v, std::invoke_result_t
#include <utility>     // std::move, std::swap

class ThreadPool
{
    typedef std::uint_fast32_t ui32;

public:
    ThreadPool(const ui32 &_thread_count = std::thread::hardware_concurrency())
        : thread_count(_thread_count ? _thread_count : std::thread::hardware_concurrency()), threads(new std::thread[_thread_count ? _thread_count : std::thread::hardware_concurrency()])
    {
        create_threads();
    }

    ~ThreadPool()
    {
        wait_for_tasks();
        running = false;
        destroy_threads();
    }

    size_t get_tasks_queued() const
    {
        const std::scoped_lock lock(queue_mutex);
        return tasks.size();
    }

    ui32 get_tasks_running() const
    {
        return tasks_total - (ui32)get_tasks_queued();
    }

    ui32 get_tasks_total() const
    {
        return tasks_total;
    }

    ui32 get_thread_count() const
    {
        return thread_count;
    }

    template <typename T, typename F>
    void parallelize_loop(T first_index, T last_index, const F &loop, ui32 num_tasks = 0)
    {
        if (num_tasks == 0)
            num_tasks = thread_count;
        if (last_index < first_index)
            std::swap(last_index, first_index);
        size_t total_size = last_index - first_index + 1;
        size_t block_size = total_size / num_tasks;
        if (block_size == 0)
        {
            block_size = 1;
            num_tasks = (ui32)total_size > 1 ? (ui32)total_size : 1;
        }
        std::atomic<ui32> blocks_running = 0;
        for (ui32 t = 0; t < num_tasks; t++)
        {
            T start = (T)(t * block_size + first_index);
            T end = (t == num_tasks - 1) ? last_index : (T)((t + 1) * block_size + first_index - 1);
            blocks_running++;
            push_task([start, end, &loop, &blocks_running]
                      {
                          for (T i = start; i <= end; i++)
                              loop(i);
                          blocks_running--;
                      });
        }
        while (blocks_running != 0)
        {
            sleep_or_yield();
        }
    }

    template <typename F>
    void push_task(const F &task)
    {
        tasks_total++;
        {
            const std::scoped_lock lock(queue_mutex);
            tasks.push(std::function<void()>(task));
        }
    }

    template <typename F, typename... A>
    void push_task(const F &task, const A &...args)
    {
        push_task([task, args...]
                  { task(args...); });
    }

    void reset(const ui32 &_thread_count = std::thread::hardware_concurrency())
    {
        bool was_paused = paused;
        paused = true;
        wait_for_tasks();
        running = false;
        destroy_threads();
        thread_count = _thread_count ? _thread_count : std::thread::hardware_concurrency();
        threads.reset(new std::thread[thread_count]);
        paused = was_paused;
        create_threads();
        running = true;
    }

    template <typename F, typename... A, typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>>>>
    std::future<bool> submit(const F &task, const A &...args)
    {
        std::shared_ptr<std::promise<bool>> promise(new std::promise<bool>);
        std::future<bool> future = promise->get_future();
        push_task([task, args..., promise]
                  {
                      task(args...);
                      promise->set_value(true);
                  });
        return future;
    }

    template <typename F, typename... A, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>, typename = std::enable_if_t<!std::is_void_v<R>>>
    std::future<R> submit(const F &task, const A &...args)
    {
        std::shared_ptr<std::promise<R>> promise(new std::promise<R>);
        std::future<R> future = promise->get_future();
        push_task([task, args..., promise]
                  { promise->set_value(task(args...)); });
        return future;
    }

    void wait_for_tasks()
    {
        while (true)
        {
            if (!paused)
            {
                if (tasks_total == 0)
                    break;
            }
            else
            {
                if (get_tasks_running() == 0)
                    break;
            }
            sleep_or_yield();
        }
    }

    std::atomic<bool> paused = false;
    ui32 sleep_duration = 1000;

private:
    void create_threads()
    {
        for (ui32 i = 0; i < thread_count; i++)
        {
            threads[i] = std::thread(&ThreadPool::worker, this);
        }
    }

    void destroy_threads()
    {
        for (ui32 i = 0; i < thread_count; i++)
        {
            threads[i].join();
        }
    }

    bool pop_task(std::function<void()> &task)
    {
        const std::scoped_lock lock(queue_mutex);
        if (tasks.empty())
            return false;
        else
        {
            task = std::move(tasks.front());
            tasks.pop();
            return true;
        }
    }

    void sleep_or_yield()
    {
        if (sleep_duration)
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration));
        else
            std::this_thread::yield();
    }

    void worker()
    {
        while (running)
        {
            std::function<void()> task;
            if (!paused && pop_task(task))
            {
                task();
                tasks_total--;
            }
            else
            {
                sleep_or_yield();
            }
        }
    }

    mutable std::mutex queue_mutex;
    std::atomic<bool> running = true;
    std::queue<std::function<void()>> tasks;
    ui32 thread_count;
    std::unique_ptr<std::thread[]> threads;
    std::atomic<ui32> tasks_total = 0;
};
