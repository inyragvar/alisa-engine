#ifndef ALISA_TASKS_TASK_MANAGER_H_
#define ALISA_TASKS_TASK_MANAGER_H_

#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>

#include "alisa/utils/singleton.h"

namespace alisa {
namespace tasks {

typedef std::function<void()> task_lambda;

class TaskManager : public utils::Singleton<TaskManager> {
    friend class utils::Singleton<TaskManager>;  // Allow Singleton to access private constructor

public:
    ~TaskManager();

public:
    void init(unsigned int num_threads = 4);
    void clear();

public:
    // This is for void-returning jobs
    template <typename F, typename SuccessCb, typename ErrorCb>
    typename std::enable_if<std::is_void<typename std::result_of<F()>::type>::value>::type
    submit(F&& job, SuccessCb&& success_callback, ErrorCb&& error_callback) {
        // Enqueue the job in the thread pool
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Lambda that handles void-returning jobs
            auto task_void = [job, success_callback, error_callback]() {
                try {
                    job();  // Call the job
                    success_callback();  // Call success callback with no arguments
                } catch (const std::exception& e) {
                    error_callback(e.what());  // Call error callback with the exception message
                }
            };

            tasks_.emplace(task_void);  // For void-returning jobs
        }

        // Notify one worker thread
        condition_.notify_one();
    }

    // This is for non-void-returning jobs
    template <typename F, typename SuccessCb, typename ErrorCb>
    typename std::enable_if<!std::is_void<typename std::result_of<F()>::type>::value>::type
    submit(F&& job, SuccessCb&& success_callback, ErrorCb&& error_callback) {
        using ReturnType = typename std::result_of<F()>::type;

        // Enqueue the job in the thread pool
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Lambda that handles non-void-returning jobs
            auto task_non_void = [job, success_callback, error_callback]() {
                try {
                    ReturnType result = job();  // Call the job
                    success_callback(result);   // Call success callback with the result of job
                } catch (const std::exception& e) {
                    error_callback(e.what());  // Call error callback with the exception message
                }
            };

            tasks_.emplace(task_non_void);  // For non-void-returning jobs
        }

        // Notify one worker thread
        condition_.notify_one();
    }

private:
    TaskManager();

private:
    std::vector<std::thread> workers_; 
    std::queue<task_lambda> tasks_; // Task queue
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

}
}


#endif