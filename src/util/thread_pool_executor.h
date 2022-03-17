/**
 * src/util/thread_pool_executor.h
 *
 * Copyright (c) 2021-2022 Bartek Kryza <bkryza@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <deque>
#include <future>
#include <thread>
#include <vector>

namespace clanguml::util {
class thread_pool_executor {
public:
    thread_pool_executor()
        : thread_pool_executor{0}
    {
    }

    thread_pool_executor(unsigned int pool_size)
        : done_{false}
    {
        if (pool_size == 0U)
            pool_size = std::thread::hardware_concurrency();

        for (auto i = 0U; i < pool_size; i++) {
            threads_.push_back(
                std::thread{&thread_pool_executor::worker, this});
        }
    }

    ~thread_pool_executor() { stop(); }

    std::future<void> add(std::function<void()> &&task)
    {
        std::unique_lock<std::mutex> l(tasks_mutex_);

        std::packaged_task<void()> ptask{std::move(task)};
        auto res = ptask.get_future();

        tasks_.emplace_back(std::move(ptask));

        l.unlock();
        tasks_cond_.notify_one();

        return res;
    }

    void stop()
    {
        done_ = true;
        for (auto &thread : threads_) {
            if (thread.joinable())
                thread.join();
        }
    }

private:
    void worker()
    {
        try {
            while (!done_) {
                auto task = get();

                task();
            }
        }
        catch (std::runtime_error &e) {
        }
    }

    std::packaged_task<void()> get()
    {
        std::unique_lock<std::mutex> l(tasks_mutex_);

        while (tasks_.empty()) {
            if (done_)
                throw std::runtime_error("Thread pool closing...");

            tasks_cond_.wait_for(l, std::chrono::seconds(1));
        }

        auto res = std::move(tasks_.front());
        tasks_.pop_front();
        return res;
    }

    std::atomic_bool done_;

    std::deque<std::packaged_task<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable tasks_cond_;

    std::vector<std::thread> threads_;
};
}