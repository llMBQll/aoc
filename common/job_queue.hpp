#pragma once

#include <future>
#include <queue>

namespace mbq
{
    class JobQueue
    {
    public:
        explicit JobQueue(size_t num_threads)
        {
            while (num_threads > 0)
            {
                num_threads -= 1;
                _workers.push_back(make_worker());
            }
            _running = true;
        }
        ~JobQueue()
        {
            stop();
        }

        void stop()
        {
            _running = false;
            _condition.notify_all();
            for (auto& worker : _workers)
            {
                worker.join();
            }
        }

        template <typename Fn, typename... Args>
        auto enqueue(Fn&& job, Args&&... args)
            requires std::invocable<Fn, Args...>
        {
            using ret_type = std::invoke_result_t<Fn, Args...>;

            std::promise<ret_type> promise;
            auto future = promise.get_future();

            {
                auto bound = std::bind(std::forward<Fn>(job), std::forward<Args>(args)...);

                std::scoped_lock lock(_job_mutex);

                if (!_running)
                {
                    throw std::runtime_error("Queue is stopped");
                }
                _jobs.emplace([promise = std::move(promise), bound = std::move(bound)] mutable {
                    try
                    {
                        if constexpr (std::same_as<void, ret_type>)
                        {
                            bound();
                            promise.set_value();
                        }
                        else
                        {
                            promise.set_value(bound());
                        }
                    }
                    catch (...)
                    {
                        promise.set_exception(std::current_exception());
                    }
                });
            }
            _condition.notify_one();

            return future;
        }
    private:
        std::thread make_worker()
        {
            return std::thread{[this] {
                while (true)
                {
                    std::move_only_function<void()> job;

                    {
                        std::unique_lock lock(_job_mutex);
                        _condition.wait(lock, [this] { return !this->_running || !this->_jobs.empty(); });

                        if (!this->_running && this->_jobs.empty())
                        {
                            return;
                        }

                        job = std::move(this->_jobs.front());
                        this->_jobs.pop();
                    }

                    job();
                }
            }};
        }
    private:
        std::vector<std::thread> _workers;
        std::queue<std::move_only_function<void()>> _jobs;
        std::mutex _job_mutex;
        std::condition_variable _condition;
        bool _running;
    };
} // namespace mbq