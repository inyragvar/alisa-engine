
#ifndef ALISA_UTILS_SINGLETON_H_
#define ALISA_UTILS_SINGLETON_H_

#include <mutex>

namespace alisa {
namespace utils {

template <typename T>
class Singleton {
public:
    static T& get() {
        std::call_once(init_instance_flag_, &Singleton::initSingleton);
        return *instance_;
    }

    // Delete copy constructor and assignment operator to prevent copying
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    static void initSingleton() {
        instance_ = new T();
    }

    static std::once_flag init_instance_flag_;
    static T* instance_;
};

// Initialize static members
template <typename T>
std::once_flag Singleton<T>::init_instance_flag_;

template <typename T>
T* Singleton<T>::instance_ = nullptr;

}
}

#endif