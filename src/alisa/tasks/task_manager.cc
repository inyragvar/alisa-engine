#include "task_manager.h"

namespace alisa {
namespace tasks {

 // Constructor to initialize the thread pool with a given number of threads
TaskManager::TaskManager() {}

TaskManager::~TaskManager() {
    clear();
}

void TaskManager::init(unsigned int num_threads) {
    for (size_t i = 0; i < num_threads; i++) {
        workers_.emplace_back([this] {
            while (true) {
                task_lambda task;
                {
                    // Lock the task queue
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    // Wait until a task is available or stop is requested
                    condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    
                    // If stopping and no tasks left, exit
                    if (stop_ && tasks_.empty()) return;
                    
                    // Get the task
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                // Execute the task
                task();
            }
        });
    }
}

void TaskManager::clear() {
    // clear resources
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (stop_) {
            // already called once
            return;
        }
        stop_ = true;
    }

    condition_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

}
}

