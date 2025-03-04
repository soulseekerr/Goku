#pragma once

#include <memory>
#include <mutex>  // since C++11 std::call_once

namespace soul {

/**
 * thread safe version of singleton which needs friend class 
 * on derived class
 */ 

template <typename T> 
class SingletonT {

// Protected constructor accessed from inherited classes
protected:
    SingletonT() {}
    ~SingletonT() {}
    SingletonT(SingletonT&) = delete;
    SingletonT& operator=(const SingletonT& o) = delete;

public:
    static T& getInstance() {
        std::call_once(initFlag, []() {
            instance_.reset(new T());
        });
        return *instance_;
    }

private:
    // Unique instance of the class
    static std::unique_ptr<T>   instance_;
    // Execute callable once
    static std::once_flag       initFlag;
};

#define MAKE_SINGLETON(Class) \
public: \
    friend class SingletonT<Class>; \
    Class() = default; \
    ~Class() = default;

template <typename T>
std::unique_ptr<T> SingletonT<T>::instance_ = nullptr;

template <typename T>
std::once_flag SingletonT<T>::initFlag;

} // namespace soul