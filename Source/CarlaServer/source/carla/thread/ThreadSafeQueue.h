// CARLA, Copyright (C) 2017 Computer Vision Center (CVC)

#pragma once

#include <condition_variable>
// #include <memory>
#include <mutex>
#include <queue>

namespace carla {
namespace thread {

  /// A thread safe queue.
  ///
  /// From "C++ Concurrency In Action", Anthony Williams, listing 4.5.
  template<typename T>
  class CARLA_API ThreadSafeQueue {
  public:

    ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue &other) {
      std::lock_guard<std::mutex> lock(other._mutex);
      _queue = other._queue;
    }

    void push(T new_value) {
      std::lock_guard<std::mutex> lock(_mutex);
      _queue.push(new_value);
      _condition.notify_one();
    }

    void wait_and_pop(T &value) {
      std::unique_lock<std::mutex> lock(_mutex);
      _condition.wait(lock, [this] {return !_queue.empty(); });
      value = _queue.front();
      _queue.pop();
    }

    // std::shared_ptr<T> wait_and_pop() {
    //   std::unique_lock<std::mutex> lock(_mutex);
    //   _condition.wait(lock, [this] {return !_queue.empty(); });
    //   std::shared_ptr<T> res(std::make_shared<T>(_queue.front()));
    //   _queue.pop();
    //   return res;
    // }

    bool try_pop(T &value) {
      std::lock_guard<std::mutex> lock(_mutex);
      if (_queue.empty()) {
        return false;
      }
      value = _queue.front();
      _queue.pop();
      return true;
    }

    // std::shared_ptr<T> try_pop() {
    //   std::lock_guard<std::mutex> lock(_mutex);
    //   if (_queue.empty()) {
    //     return std::shared_ptr<T>();
    //   }
    //   std::shared_ptr<T> res(std::make_shared<T>(_queue.front()));
    //   _queue.pop();
    //   return res;
    // }

    bool empty() const {
      std::lock_guard<std::mutex> lock(_mutex);
      return _queue.empty();
    }

  private:

    mutable std::mutex _mutex;

    std::queue<T> _queue;

    std::condition_variable _condition;
  };

} // namespace thread
} // namespace carla